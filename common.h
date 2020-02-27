#ifndef COMMON_H_
#define COMMON_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <inttypes.h>

#define INVERTED 1
#define RELEASE 1

#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7

#define SET_BIT(reg, pin) (reg |= (1<<pin))
#define CLEAR_BIT(reg, pin) (reg &= ~(1<<pin))
#define TOGGLE_BIT(reg, pin) (reg ^= (1<<pin))

#define CONFIG_OUTPUT(port, pin) (SET_BIT(port, pin))
#define CONFIG_INPUT(port, pin) (CLEAR_BIT(port, pin))

typedef struct {
	volatile uint8_t * ddr;
	volatile uint8_t * port;
	uint8_t pin;
	volatile uint8_t * portin;
} IO_struct;

#endif