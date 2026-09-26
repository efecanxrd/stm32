#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"
#include <stdint.h>
#include <stddef.h>

/*
 * Configuration structure for SPIx peripheral
 */
typedef struct {
    uint8_t SPI_DeviceMode;
    uint8_t SPI_BusConfig;
    uint8_t SPI_SclkSpeed;
    uint8_t SPI_DFF;
    uint8_t SPI_CPOL;
    uint8_t SPI_CPHA;
    uint8_t SPI_SSM;
} SPI_Config_t;

/*
 * Handle structure for SPIx peripheral
 */
typedef struct {
    SPI_RegDef_t *pSPIx;       /*!< Base address of SPIx peripheral */
    SPI_Config_t SPIConfig;    /*!< Configuration settings */
    uint8_t      *pTxBuffer;   /*!< Transmission buffer address */
    uint8_t      *pRxBuffer;   /*!< Reception buffer address */
    uint32_t     TxLen;        /*!< Transmit length (uint32_t prevents truncation) */
    uint32_t     RxLen;        /*!< Receive length (uint32_t prevents truncation) */
    uint8_t      TxState;      /*!< Transmit state */
    uint8_t      RxState;      /*!< Receive state */
} SPI_Handle_t;

/*
 * SPI Device Modes
 */
#define SPI_DEVICE_MODE_SLAVE            0
#define SPI_DEVICE_MODE_MASTER           1

/*
 * SPI Bus Configurations
 */
#define SPI_BUS_CONFIG_FD                1
#define SPI_BUS_CONFIG_HD                2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY    3

/*
 * SPI Serial Clock Speeds (Baud Rate Prescaler)
 */
#define SPI_SCLK_SPEED_DIV2              0
#define SPI_SCLK_SPEED_DIV4              1
#define SPI_SCLK_SPEED_DIV8              2
#define SPI_SCLK_SPEED_DIV16             3
#define SPI_SCLK_SPEED_DIV32             4
#define SPI_SCLK_SPEED_DIV64             5
#define SPI_SCLK_SPEED_DIV128            6
#define SPI_SCLK_SPEED_DIV256            7

/*
 * SPI Data Frame Formats
 */
#define SPI_DFF_8BITS                    0
#define SPI_DFF_16BITS                   1

/*
 * Clock Polarity (CPOL)
 */
#define SPI_CPOL_LOW                     0
#define SPI_CPOL_HIGH                    1

/*
 * Clock Phase (CPHA)
 */
#define SPI_CPHA_LOW                     0
#define SPI_CPHA_HIGH                    1

/*
 * Software Slave Management (SSM)
 */
#define SPI_SSM_DI                       0
#define SPI_SSM_EN                       1

/*
 * Status Register (SR) Flags
 */
#define SPI_RXNE_FLAG                    (1 << SPI_SR_RXNE)
#define SPI_TXE_FLAG                     (1 << SPI_SR_TXE)
#define SPI_CHSIDE_FLAG                  (1 << SPI_SR_CHSIDE)
#define SPI_UDR_FLAG                     (1 << SPI_SR_UDR)
#define SPI_CRCERR_FLAG                  (1 << SPI_SR_CRCERR)
#define SPI_MODF_FLAG                    (1 << SPI_SR_MODF)
#define SPI_OVR_FLAG                     (1 << SPI_SR_OVR)
#define SPI_BUSY_FLAG                    (1 << SPI_SR_BSY)
#define SPI_FRE_FLAG                     (1 << SPI_SR_FRE)

/*
 * Application States
 */
#define SPI_READY                        0
#define SPI_BUSY_IN_RX                   1
#define SPI_BUSY_IN_TX                   2

/*
 * Application Events
 */
#define SPI_EVENT_TX_CMPLT               1
#define SPI_EVENT_RX_CMPLT               2
#define SPI_EVENT_OVR_ERR                3
#define SPI_EVENT_CRC_ERR                4

/*
 * Peripheral Clock Control
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t status);

/*
 * Init and De-Init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 * Blocking Data Transfer APIs
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len);

/*
 * Non-Blocking (Interrupt-Based) Transfer APIs
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len);

/*
 * IRQ Configuration and ISR Handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t status);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/*
 * Peripheral Control and Utility APIs
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t status);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t status);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t status);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

/*
 * Application Callback
 */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv);

#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
