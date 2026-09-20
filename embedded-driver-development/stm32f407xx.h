#include <stdint.h>

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

/*
	Base addresses of Flash and SRAM memories
*/

#define FLASH_BASEADDR		0x08000000U
#define SRAM1_BASEADDR		0x20000000U
#define SRAM2_BASEADDR 0x2001C000U
#define ROM_BASEADDR			0x1FFF0000U
#define SRAM 				SRAM1_BASEADDR

/*
	AHBx and APBx Bus Peripheral base addresses
*/

#define PERIPH_BASE			0x40000000U
#define APB1PERIPH_BASE		PERIPH_BASE
#define APB2PERIPH_BASE		0x40010000U
#define AHB1PERIPH_BASE 		0x40020000U
#define AHB2PERIPH_BASE		0x50000000U

//Base addresses if peripherals which are hanging on AHB1 Bus
#define GPIOA_BASEADDR		0x40020000U
#define GPIOB_BASEADDR		0x40020400U
#define GPIOC_BASEADDR		0x40020800U
#define GPIOD_BASEADDR		0x40020C00U
#define GPIOE_BASEADDR		0x40021000U
#define GPIOF_BASEADDR		0x40021400U
#define GPIOG_BASEADDR		0x40021800U
#define GPIOH_BASEADDR		0x40021C00U
#define GPIOI_BASEADDR		0x40022000U
#define RCC_BASEADDR			0x40023800U

//Base addresses if peripherals which are hanging on APB1 Bus
#define I2C1_BASEADDR		0x40005400U
#define I2C2_BASEADDR		0x40005800U
#define I2C3_BASEADDR		0x40005C00U
#define SPI2_BASEADDR		0x40003800U
#define	SPI3_BASEADDR		0x40003C00U
#define	USART2_BASEADDR		0x40004400U
#define	USART3_BASEADDR		0x40004800U
#define UART4_BASEADDR		0x40004C00U
#define UART5_BASEADDR		0x40005000U

//Base addresses if peripherals which are hanging on APB2 Bus
#define SPI1_BASEADDR		0x40013000U
#define	USART1_BASEADDR		0x40011000U
#define	USART6_BASEADDR		0x40011400U
#define	EXTI_BASEADDR		0x40013C00U
#define	SYSCFG_BASEADDR		0x40013800U


//Peripheral Register Definition Structure for GPIO
typedef struct {
    volatile uint32_t MODER;    /*!< GPIO port mode register, 0x00 */
    volatile uint32_t OTYPER;   /*!< GPIO port output type register, 0x04 */
    volatile uint32_t OSPEEDR;  /*!< GPIO port output speed register, 0x08 */
    volatile uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register, 0x0C */
    volatile uint32_t IDR;      /*!< GPIO port input data register, 0x10 */
    volatile uint32_t ODR;      /*!< GPIO port output data register, 0x14 */
    volatile uint32_t BSRR;     /*!< Bit set: bits [15:0], Bit reset: bits [31:16], 0x18 */
    volatile uint32_t LCKR;     /*!< GPIO port configuration lock register, 0x1C */
    volatile uint32_t AFR[2];   /*!< GPIO alternate function low (0x20) and high (0x24) registers */
} GPIO_RegDef_t;

typedef struct {
	volatile uint32_t CR;            /*!< RCC clock control register, 0x00 */
	volatile uint32_t PLLCFGR;       /*!< RCC PLL configuration register, 0x04 */
	volatile uint32_t CFGR;          /*!< RCC clock configuration register, 0x08 */
	volatile uint32_t CIR;           /*!< RCC clock interrupt register, 0x0C */
	volatile uint32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register, 0x10 */
	volatile uint32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register, 0x14 */
	volatile uint32_t AHB3RSTR;      /*!< RCC AHB3 peripheral reset register, 0x18 */
	volatile uint32_t RESERVED_0;    /*!< Reserved, 0x1C */
	volatile uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register, 0x20 */
	volatile uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register, 0x24 */
	volatile uint32_t RESERVED_1[2]; /*!< Reserved, 0x28-0x2C */
	volatile uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock enable register, 0x30 */
	volatile uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock enable register, 0x34 */
	volatile uint32_t AHB3ENR;       /*!< RCC AHB3 peripheral clock enable register, 0x38 */
	volatile uint32_t RESERVED_3;    /*!< Reserved, 0x3C */
	volatile uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register, 0x40 */
	volatile uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register, 0x44 */
	volatile uint32_t RESERVED_4[2]; /*!< Reserved, 0x48-0x4C */
	volatile uint32_t AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, 0x50 */
	volatile uint32_t AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, 0x54 */
	volatile uint32_t AHB3LPENR;     /*!< RCC AHB3 peripheral clock enable in low power mode register, 0x58 */
	volatile uint32_t RESERVED_5;    /*!< Reserved, 0x5C */
	volatile uint32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, 0x60 */
	volatile uint32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, 0x64 */
	volatile uint32_t RESERVED_6[2]; /*!< Reserved, 0x68-0x6C */
	volatile uint32_t BDCR;          /*!< RCC Backup domain control register, 0x70 */
	volatile uint32_t CSR;           /*!< RCC clock control & status register, 0x74 */
	volatile uint32_t RESERVED_7[2]; /*!< Reserved, 0x78-0x7C */
	volatile uint32_t SSCGR;         /*!< RCC spread spectrum clock generation register, 0x80 */
	volatile uint32_t PLLI2SCFGR;    /*!< RCC PLLI2S configuration register, 0x84 */
} RCC_RegDef_t;

#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH ((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI ((GPIO_RegDef_t*)GPIOI_BASEADDR)

#define RCC	  ((RCC_RegDef_t*)RCC_BASEADDR)


/*
	Clock Enable Macros for GPIOx Peripherals
*/

#define GPIOA_PCLK_EN()	(RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()	(RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()	(RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()	(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()	(RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()	(RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()	(RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()	(RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN()	(RCC->AHB1ENR |= (1 << 8))


/*
	Clock Enable Macros for I2Cx Peripherals
*/
#define I2C1_PCLK_EN()	(RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()	(RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()	(RCC->APB1ENR |= (1 << 23))

/*
	Clock Enable Macros for SPIx Peripherals
*/
#define SPI1_PCLK_EN()	(RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()	(RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()	(RCC->APB1ENR |= (1 << 15))

/*
	Clock Enable Macros for USARTx Peripherals
*/
#define USART1_PCLK_EN()	(RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()	(RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()	(RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()	(RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()	(RCC->APB1ENR |= (1 << 20))
#define USART6_PCLK_EN()	(RCC->APB2ENR |= (1 << 5))

/*
	Clock Enable Macros for SYSCFG Peripheral
*/
#define SYSCFG_PCLK_EN() (RCC->APB2ENR |= (1 << 14))

/////////////////////////////////////////////////////

/*
	Clock Disable Macros for GPIOx Peripherals
*/

#define GPIOA_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 8))


/*
	Clock Disable Macros for I2Cx Peripherals
*/
#define I2C1_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 23))

/*
	Clock Disable Macros for SPIx Peripherals
*/
#define SPI1_PCLK_DI()	(RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 15))

/*
	Clock Disable Macros for USARTx Peripherals
*/
#define USART1_PCLK_DI()	(RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 20))
#define USART6_PCLK_DI()	(RCC->APB2ENR &= ~(1 << 5))

/*
	Clock Disable Macros for SYSCFG Peripheral
*/
#define SYSCFG_PCLK_DI() (RCC->APB2ENR &= ~(1 << 14))

#define ENABLE 			1
#define DISABLE 			0
#define SET				ENABLE
#define RESET			DISABLE
#define GPIO_PIN_SET		SET
#define GPIO_PIN_RESET	RESET


/*
	Clock reset macros for GPIOx peripherals
*/
#define GPIOA_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 0));	(RCC->AHB1RSTR &= ~(1 << 0)); } while(0)
#define GPIOB_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 1));	(RCC->AHB1RSTR &= ~(1 << 1)); } while(0)
#define GPIOC_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 2));	(RCC->AHB1RSTR &= ~(1 << 2)); } while(0)
#define GPIOD_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 3));	(RCC->AHB1RSTR &= ~(1 << 3)); } while(0)
#define GPIOE_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 4));	(RCC->AHB1RSTR &= ~(1 << 4)); } while(0)
#define GPIOF_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 5));	(RCC->AHB1RSTR &= ~(1 << 5)); } while(0)
#define GPIOG_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 6));	(RCC->AHB1RSTR &= ~(1 << 6)); } while(0)
#define GPIOH_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 7));	(RCC->AHB1RSTR &= ~(1 << 7)); } while(0)
#define GPIOI_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 8));	(RCC->AHB1RSTR &= ~(1 << 8)); } while(0)


#endif /* INC_STM32F407XX_H_ */

