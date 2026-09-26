#include "stm32f407xx_spi_driver.h"
#include <stddef.h>

//Peripheral Clock Setup
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t status) {
	if(status == ENABLE) {
		if(pSPIx == SPI1) {
			SPI1_PCLK_EN();
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_EN();
		} else if (pSPIx == SPI3) {
			SPI3_PCLK_EN();
		}
	} else {
		if(pSPIx == SPI1) {
			SPI1_PCLK_DI();
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_DI();
		} else if (pSPIx == SPI3) {
			SPI3_PCLK_DI();
		}
	}
}

//Init and De-init
void SPI_Init(SPI_Handle_t *pSPIHandle) {
	//peripheral clock enable
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	// 1. Configure the SPI_CR1 reg
	uint32_t tempreg = 0;

	// 1. Configure device mode.
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	// 2. Configure the bus config
	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD) {
		// bidi mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);

	} else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD) {
		// bidi mode should be set
		tempreg |= (1 << SPI_CR1_BIDIMODE);

	} else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY) {
		// BIDI mode should be cleared.
		// RXONLY bit must be set.
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		tempreg |= (1 << SPI_CR1_RXONLY);
	}

	// 3. Configure the spi serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	// 4. Configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	// 5. Configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	// 6. Configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 = tempreg;
}


void SPI_DeInit(SPI_RegDef_t *pSPIx);

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName) {
	if(pSPIx->SR & FlagName) {
		return FLAG_SET;
	}
	return FLAG_RESET;
}

//Data Send And Receive
//This is blocking call (polling based code)
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len) {
	while (len > 0) {
		//1. wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		//2. Check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
			//16 BIT DFF
			//1. load the data into the DR
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			len -= 2;
			pTxBuffer += 2;
		} else {
			//8 BIT DFF
			pSPIx->DR = *pTxBuffer;
			len--;
			pTxBuffer++;
		}
	}
}


void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len) {
	while (len > 0) {
		//1. wait until rxne is set
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		//2. Check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
			//16 BIT DFF
			//1. load the data from DR to RX buffer
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			len -= 2;
			pRxBuffer += 2;
		} else {
			//8 BIT DFF
			*(pRxBuffer) = pSPIx->DR;
			len--;
			pRxBuffer++;
		}
	}
}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t status) {
	if(status == ENABLE) {
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	} else {
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t status) {
	if(status == ENABLE) {
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	} else {
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}
//IRQ Configuration and ISR Handling
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(IRQNumber <= 31)
        {
            *NVIC_ISER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64)
        {
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96)
        {
            *NVIC_ISER2 |= (1 << (IRQNumber % 32));
        }
    }
    else
    {
        if(IRQNumber <= 31)
        {
            *NVIC_ICER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64)
        {
            *NVIC_ICER1 |= (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96)
        {
            *NVIC_ICER2 |= (1 << (IRQNumber % 32));
        }
    }
}


void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority) {
	//1. first lets find out the IPR register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

static void spi_txe_interrupt_handler(SPI_Handle_t *pSPIHandle) {
		//Check the DFF bit in CR1
		if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
			//16 BIT DFF
			//1. load the data into the DR
			pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
			pSPIHandle->TxLen -= 2;
			pSPIHandle->pTxBuffer += 2;
		} else {
			//8 BIT DFF
			pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
			pSPIHandle->TxLen--;
			pSPIHandle->pTxBuffer++;
		}

		if(!pSPIHandle->TxLen) {
			//txlen is zero, so close the spi communication and inform the app that tx is over
			SPI_CloseTransmission(pSPIHandle);
			SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
		}
}


static void spi_rxne_interrupt_handler(SPI_Handle_t *pSPIHandle) {
	//Check the DFF bit in CR1
	if(pSPIHandle->pSPIx->CR1 & (1 << 11)) {
		//16 BIT DFF
		//1. load the data into the DR
		*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen -= 2;
		pSPIHandle->pRxBuffer += 2;
	} else {
		//8 BIT DFF
		*(pSPIHandle->pRxBuffer) = (uint8_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}

	if(!pSPIHandle->RxLen) {
		//txlen is zero, so close the spi communication and inform the app that tx is over
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
	}
}

static void spi_ovr_err_interrupt_handler(SPI_Handle_t *pSPIHandle) {

	uint8_t temp;
	//1. clear the ovr flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX) {
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;

	}
	(void) temp;

	//2. inform the app
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}

void SPI_IRQHandling(SPI_Handle_t *pHandle) {
	uint8_t temp1, temp2;
	//First lets check for TXE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2) {
		//handle TXE
		spi_txe_interrupt_handler(pHandle);
	}

	//check for RXNE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 && temp2) {
		//handle TXE
		spi_rxne_interrupt_handler(pHandle);
	}

	// check for OVR flagg
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 && temp2) {
		//handle TXE
		spi_ovr_err_interrupt_handler(pHandle);
	}

}

void  SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |=  (1 << SPI_CR2_SSOE);
	}else
	{
		pSPIx->CR2 &=  ~(1 << SPI_CR2_SSOE);
	}


}

//Data Send And Receive / Interrupt Based
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len) {
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX) {
	//1. Save the TX Buffer address and len information on some global var
	pSPIHandle->pTxBuffer = pTxBuffer;
	pSPIHandle->TxLen = len;

	//2. Mark the SPI state as busy in transmission so that no other code can take over
	//same SPI peripheral until transmission is over.
	pSPIHandle->TxState = SPI_BUSY_IN_TX;

	//3. Enable the TXEIE control bit to get interrupt whenever TX flag is set in SR flag
	pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
	}

	return state;
}

uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len) {
	uint8_t state = pSPIHandle->RxState;

	if(state != SPI_BUSY_IN_RX) {
	//1. Save the TX Buffer address and len information on some global var
	pSPIHandle->pRxBuffer = pRxBuffer;
	pSPIHandle->RxLen = len;

	//2. Mark the SPI state as busy in transmission so that no other code can take over
	//same SPI peripheral until transmission is over.
	pSPIHandle->RxState = SPI_BUSY_IN_RX;

	//3. Enable the TXEIE control bit to get interrupt whenever TX flag is set in SR flag
	pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
	}

	return state;
}

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle) {
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle) {
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx) {
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void) temp;
}

__attribute__((weak)) void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv) {
	//weak implementation. can override.
}
