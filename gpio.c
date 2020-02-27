#include "gpio.h"

/*** CONVENTION :
 * an "l" next to a color refers to the on-board LEDs;
 * a "g" next to a color refers to the GPIO LEDs;
 * if an L or G is not present, assume on-board LED. 
 */

/*** GPIO LEDS : 
 * green2	PB6
 * yellow 	PD1
 * green 	PD4
 * red 		PD2
 */

/* Scope is limited to this file. Used to indicate if GPIO LED is initialized. */
static uint8_t gGreen2Init = 0;
static uint8_t gYellowInit = 0;
static uint8_t gGreenInit = 0;
static uint8_t gRedInit = 0;

/* configure the data direction for the specified GPIO led. */
void configure_gpio(IO_struct * color) {
	SET_BIT(*color->ddr, color->pin);
}

void initialize_gpio(int color) {
	switch(color) {
		case (GREEN2) :
			_green2 = (IO_struct) { &DDRB, &PORTB, GREEN2, &PINB };
			configure_gpio(&_green2);
			gGreen2Init = 1;
			break;
		case (YELLOWG) :
			_yellowg = (IO_struct) { &DDRD, &PORTD, YELLOWG, &PIND };
			configure_gpio(&_yellowg);
			gYellowInit = 1;
			break;
		case (GREENG) :
			_greeng = (IO_struct) { &DDRD, &PORTD, GREENG, &PIND };
			configure_gpio(&_greeng);
			gGreenInit = 1;
			break;  
		case(REDG):
			_redg = (IO_struct) { &DDRD, &PORTD, REDG, &PIND };
			configure_gpio(&_redg);
			gRedInit = 1;
			break;		
	}
}

void gpio_on(IO_struct * color) {
	SET_BIT(*color->port, color->pin);
}

void gpio_off(IO_struct * color) {
	CLEAR_BIT(*color->port, color->pin);
}

void gpio_toggle(IO_struct * color) {
	TOGGLE_BIT(*color->port, color->pin);
}

/* Flash the designated GPIO led for 250ms on, then 250ms off.
 * Assumes led is initialized */
void flash_gpio(IO_struct * color) {
	SET_BIT(*color->port, color->pin);
	_delay_ms(250);
	TOGGLE_BIT(*color->port, color->pin);
	_delay_ms(250);
}

/* Flash all the initialized GPIO leds for a sanity check light show */ 
void light_show_gpio() {
	int i;
	for (i = 0; i < 2; i++) {
		if (gGreen2Init) flash_gpio(&_green2);
		if (gYellowInit) flash_gpio(&_yellowg);
		if (gGreenInit) flash_gpio(&_greeng);
		if (gRedInit) flash_gpio(&_redg);
	}
}