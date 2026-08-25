#include <stdint.h>

#ifndef BIT_FIELDS_H
#define BIT_FIELDS_H

typedef struct {
	uint32_t GPIOAEN:1;
	uint32_t GPIOBEN:1;
	uint32_t GPIOCEN:1;
	uint32_t GPIODEN:1;
	uint32_t GPIOEEN:1;
	uint32_t GPIOFEN:1;
	uint32_t GPIOGEN:1;
	uint32_t GPIOHEN:1;
	uint32_t GPIOIEN:1;
	uint32_t RESERVED_1:3;
	uint32_t CRCEN:1;
	uint32_t RESERVED2:5;
	uint32_t BKPSRAMEN:1;
	uint32_t RESERVED3:1;
	uint32_t CCMDATARAMEN:1;
	uint32_t DMA1EN:1;
	uint32_t DMA2EN:1;
	uint32_t RESERVED4:2;
	uint32_t ETHMACEN:1;
	uint32_t ETHMACTXEN:1;
	uint32_t ETHMACRXEN:1;
	uint32_t ETHMACPTPEN:1;
	uint32_t OTGHSEN:1;
	uint32_t OTGHSULPIEN:1;
	uint32_t RESERVED5:1;

} RCC_AHB1ENR_t;

typedef struct {
	uint32_t ODR0:1;
	uint32_t ODR1:1;
	uint32_t ODR2:1;
	uint32_t ODR3:1;
	uint32_t ODR4:1;
	uint32_t ODR5:1;
	uint32_t ODR6:1;
	uint32_t ODR7:1;
	uint32_t ODR8:1;
	uint32_t ODR9:1;
	uint32_t ODR10:1;
	uint32_t ODR11:1;
	uint32_t ODR12:1;
	uint32_t ODR13:1;
	uint32_t ODR14:1;
	uint32_t ODR15:1;
	uint32_t RESERVED:16;

} GPIOx_ODR_t;


typedef struct {
	uint32_t PIN_0:2;
	uint32_t PIN_1:2;
	uint32_t PIN_2:2;
	uint32_t PIN_3:2;
	uint32_t PIN_4:2;
	uint32_t PIN_5:2;
	uint32_t PIN_6:2;
	uint32_t PIN_7:2;
	uint32_t PIN_8:2;
	uint32_t PIN_9:2;
	uint32_t PIN_10:2;
	uint32_t PIN_11:2;
	uint32_t PIN_12:2;
	uint32_t PIN_13:2;
	uint32_t PIN_14:2;
	uint32_t PIN_15:2;

} GPIOx_MODER_t;

#endif
