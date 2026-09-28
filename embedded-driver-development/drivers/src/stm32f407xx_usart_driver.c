#include "stm32f407xx_usart_driver.h"

#include "stm32f407xx_usart_driver.h"

void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{
    uint32_t PCLKx;
    uint32_t usartdiv;
    uint32_t M_part, F_part;
    uint32_t tempreg = 0;

    if (pUSARTx == USART1 || pUSARTx == USART6)
    {
        PCLKx = RCC_GetPCLK2Value();
    }
    else
    {
        PCLKx = RCC_GetPCLK1Value();
    }

    // Check for OVER8 configuration bit
    if (pUSARTx->CR1 & (1 << USART_CR1_OVER8))
    {
        // Oversampling by 8
        usartdiv = ((25 * PCLKx) / (2 * BaudRate));
    }
    else
    {
        // Oversampling by 16
        usartdiv = ((25 * PCLKx) / (4 * BaudRate));
    }

    // Mantissa part
    M_part = usartdiv / 100;
    tempreg |= M_part << 4;

    // Fractional part
    F_part = usartdiv - (M_part * 100);

    if (pUSARTx->CR1 & (1 << USART_CR1_OVER8))
    {
        F_part = (((F_part * 8) + 50) / 100) & ((uint8_t)0x07);
    }
    else
    {
        F_part = (((F_part * 16) + 50) / 100) & ((uint8_t)0x0F);
    }

    tempreg |= F_part;
    pUSARTx->BRR = tempreg;
}

void USART_Init(USART_Handle_t *pUSARTHandle)
{
    uint32_t tempreg = 0;

    USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

    /* CR1 Configuration */
    if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
    {
        tempreg |= (1 << USART_CR1_RE);
    }
    else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
    {
        tempreg |= (1 << USART_CR1_TE);
    }
    else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
    {
        tempreg |= ((1 << USART_CR1_RE) | (1 << USART_CR1_TE));
    }

    tempreg |= pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M;

    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
    {
        tempreg |= (1 << USART_CR1_PCE);
    }
    else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD)
    {
        tempreg |= (1 << USART_CR1_PCE);
        tempreg |= (1 << USART_CR1_PS);
    }

    pUSARTHandle->pUSARTx->CR1 = tempreg;

    /* CR2 Configuration */
    tempreg = 0;
    tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;
    pUSARTHandle->pUSARTx->CR2 = tempreg;

    /* CR3 Configuration */
    tempreg = 0;
    if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
    {
        tempreg |= (1 << USART_CR3_CTSE);
    }
    else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
    {
        tempreg |= (1 << USART_CR3_RTSE);
    }
    else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
    {
        tempreg |= (1 << USART_CR3_CTSE);
        tempreg |= (1 << USART_CR3_RTSE);
    }

    pUSARTHandle->pUSARTx->CR3 = tempreg;

    /* Baudrate Configuration */
    USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
}

void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t Cmd)
{
    if (Cmd == ENABLE)
    {
        pUSARTx->CR1 |= (1 << 13);
    }
    else
    {
        pUSARTx->CR1 &= ~(1 << 13);
    }
}

void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
        if (pUSARTx == USART1)
        {
            USART1_PCCK_EN();
        }
        else if (pUSARTx == USART2)
        {
            USART2_PCCK_EN();
        }
        else if (pUSARTx == USART3)
        {
            USART3_PCCK_EN();
        }
        else if (pUSARTx == UART4)
        {
            UART4_PCCK_EN();
        }
    }
    else
    {
        // TODO: Peripheral clock disable implementation
    }
}

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName)
{
    if (pUSARTx->SR & StatusFlagName)
    {
        return SET;
    }
    return RESET;
}

void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    uint16_t *pdata;

    for (uint32_t i = 0; i < Len; i++)
    {
        while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE));

        if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            pdata = (uint16_t*)pTxBuffer;
            pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

            if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                pTxBuffer += 2;
            }
            else
            {
                pTxBuffer++;
            }
        }
        else
        {
            pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t)0xFF);
            pTxBuffer++;
        }
    }

    while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC));
}

void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    for (uint32_t i = 0; i < Len; i++)
    {
        while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE));

        if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                *((uint16_t*)pRxBuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);
                pRxBuffer += 2;
            }
            else
            {
                *pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
                pRxBuffer++;
            }
        }
        else
        {
            if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
            }
            else
            {
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
            }
            pRxBuffer++;
        }
    }
}

uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    uint8_t txstate = pUSARTHandle->TxBusyState;

    if (txstate != USART_BUSY_IN_TX)
    {
        pUSARTHandle->TxLen = Len;
        pUSARTHandle->pTxBuffer = pTxBuffer;
        pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
    }

    return txstate;
}

uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    uint8_t rxstate = pUSARTHandle->RxBusyState;

    if (rxstate != USART_BUSY_IN_RX)
    {
        pUSARTHandle->RxLen = Len;
        pUSARTHandle->pRxBuffer = pRxBuffer;
        pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

        (void)pUSARTHandle->pUSARTx->DR;

        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
    }

    return rxstate;
}

void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName)
{
    pUSARTx->SR &= ~StatusFlagName;
}

void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
        if (IRQNumber <= 31)
        {
            *NVIC_ISER0 |= (1 << IRQNumber);
        }
        else if (IRQNumber > 31 && IRQNumber < 64)
        {
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        }
        else if (IRQNumber >= 64 && IRQNumber < 96)
        {
            *NVIC_ISER3 |= (1 << (IRQNumber % 64));
        }
    }
    else
    {
        if (IRQNumber <= 31)
        {
            *NVIC_ICER0 |= (1 << IRQNumber);
        }
        else if (IRQNumber > 31 && IRQNumber < 64)
        {
            *NVIC_ICER1 |= (1 << (IRQNumber % 32));
        }
        else if (IRQNumber >= 64 && IRQNumber < 96)
        {
            *NVIC_ICER3 |= (1 << (IRQNumber % 64));
        }
    }
}

void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;
    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{
    uint32_t temp1, temp2, temp3;
    uint16_t *pdata;

    /* Check for TC flag */
    temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_TC);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_TCIE);

    if (temp1 && temp2)
    {
        if (pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
        {
            if (!pUSARTHandle->TxLen)
            {
                pUSARTHandle->pUSARTx->SR &= ~(1 << USART_SR_TC);
                pUSARTHandle->TxBusyState = USART_READY;
                pUSARTHandle->pTxBuffer = NULL;
                pUSARTHandle->TxLen = 0;

                USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_TX_CMPLT);
            }
        }
    }

    /* Check for TXE flag */
    temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_TXE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_TXEIE);

    if (temp1 && temp2)
    {
        if (pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
        {
            if (pUSARTHandle->TxLen > 0)
            {
                if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
                {
                    pdata = (uint16_t*)pUSARTHandle->pTxBuffer;
                    pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

                    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
                    {
                        pUSARTHandle->pTxBuffer += 2;
                        pUSARTHandle->TxLen -= 2;
                    }
                    else
                    {
                        pUSARTHandle->pTxBuffer++;
                        pUSARTHandle->TxLen -= 1;
                    }
                }
                else
                {
                    pUSARTHandle->pUSARTx->DR = (*pUSARTHandle->pTxBuffer & (uint8_t)0xFF);
                    pUSARTHandle->pTxBuffer++;
                    pUSARTHandle->TxLen -= 1;
                }
            }

            if (pUSARTHandle->TxLen == 0)
            {
                pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_TXEIE);
            }
        }
    }

    /* Check for RXNE flag */
    temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_RXNE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_RXNEIE);

    if (temp1 && temp2)
    {
        if (pUSARTHandle->RxBusyState == USART_BUSY_IN_RX)
        {
            if (pUSARTHandle->RxLen > 0)
            {
                if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
                {
                    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
                    {
                        *((uint16_t*)pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);
                        pUSARTHandle->pRxBuffer += 2;
                        pUSARTHandle->RxLen -= 2;
                    }
                    else
                    {
                        *pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
                        pUSARTHandle->pRxBuffer++;
                        pUSARTHandle->RxLen -= 1;
                    }
                }
                else
                {
                    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
                    {
                        *pUSARTHandle->pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
                    }
                    else
                    {
                        *pUSARTHandle->pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
                    }

                    pUSARTHandle->pRxBuffer++;
                    pUSARTHandle->RxLen -= 1;
                }
            }

            if (!pUSARTHandle->RxLen)
            {
                pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_RXNEIE);
                pUSARTHandle->RxBusyState = USART_READY;
                USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_RX_CMPLT);
            }
        }
    }

    /* Check for CTS flag */
    temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_CTS);
    temp2 = pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_CTSE);
    temp3 = pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_CTSIE);

    if (temp1 && temp2 && temp3)
    {
        pUSARTHandle->pUSARTx->SR &= ~(1 << USART_SR_CTS);
        USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_CTS);
    }

    /* Check for IDLE detection flag */
    temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_IDLE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_IDLEIE);

    if (temp1 && temp2)
    {
        pUSARTHandle->pUSARTx->SR &= ~(1 << USART_SR_IDLE);
        USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_IDLE);
    }

    /* Check for Overrun detection flag */
    temp1 = pUSARTHandle->pUSARTx->SR & USART_SR_ORE;
    temp2 = pUSARTHandle->pUSARTx->CR1 & USART_CR1_RXNEIE;

    if (temp1 && temp2)
    {
        USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_ORE);
    }

    /* Check for Error Flags in multibuffer communication */
    temp2 = pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_EIE);

    if (temp2)
    {
        temp1 = pUSARTHandle->pUSARTx->SR;

        if (temp1 & (1 << USART_SR_FE))
        {
            USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_FE);
        }

        if (temp1 & (1 << USART_SR_NE))
        {
            USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_NE);
        }

        if (temp1 & (1 << USART_SR_ORE))
        {
            USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_ORE);
        }
    }
}

__attribute__((weak)) void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t event)
{
    // Weak implementation to be overridden by application
}
