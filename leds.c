#include "leds.h"

/*** CONVENTION :
 * an "l" next to a color refers to the on-board LEDs;
 * a "g" next to a color refers to the GPIO LEDs;
 * if an L or G is not present, assume on-board LED. 
 */
 
 /*** LEDS : 
 * yellow 	PC7
 * green 	PD5		INVERTED
 * red 		PB0		INVERTED
 */

/* Scope is limited to this file. Used to indicate if LED is initialized. */
static uint8_t lYellowInit = 0;
static uint8_t lGreenInit = 0;
static uint8_t lRedInit = 0;

/* configure the data direction for the specified on-board led.
 */
void configure_led(IO_struct * color) {
  SET_BIT(*color->ddr, color->pin);
}

void initialize_led(int color) {
	switch(color) {
		case (YELLOWL) :
			_yellowl = (IO_struct) { &DDRC, &PORTC, YELLOWL, &PINC };
			configure_led(&_yellowl);
			lYellowInit = 1;
			break;
		case (GREENL) :
			_greenl = (IO_struct) { &DDRD, &PORTD, GREENL, &PIND };
			configure_led(&_greenl);
			lGreenInit = 1;
			break;    
		case(REDL):
			_redl = (IO_struct) { &DDRB, &PORTB, REDL, &PINB };
			configure_led(&_redl);
			lRedInit = 1;
			break;
	}
}

void led_on(IO_struct * color, int inverted) {
	if (!inverted) { 
		SET_BIT(*color->port, color->pin);
	} else {
		CLEAR_BIT(*color->port, color->pin);
	}
}

void led_off(IO_struct * color, int inverted) {
	if (!inverted) {
		CLEAR_BIT(*color->port, color->pin);
	} else {
		SET_BIT(*color->port, color->pin);
	}
}

void led_toggle(IO_struct * color) {
	TOGGLE_BIT(*color->port, color->pin);
}

/* Flash the designated on-board led for 250ms on, then 250ms off.
 * Assumes led is initialized */
void flash_led(IO_struct * color, int inverted) {
	if (!inverted) {
		SET_BIT(*color->port, color->pin);
	} else {
		CLEAR_BIT(*color->port, color->pin);
	}
	_delay_ms(250);
	TOGGLE_BIT(*color->port, color->pin);
	_delay_ms(250);
}

/* Flash all the initialized leds for a sanity check light show */
void light_show_led() {
	int i;
	for (i = 0; i < 2; i++) {
		if (lYellowInit) flash_led(&_yellowl, !INVERTED);
		if (lGreenInit) flash_led(&_greenl, INVERTED);
		if (lRedInit) flash_led(&_redl, INVERTED);
	}
}