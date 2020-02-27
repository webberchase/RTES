#include "common.h"

#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"
#include "buttons.h"

#define INVERTED 1

/******************************************************************
   ALL INITIALIZATION
******************************************************************/
void initialize_system(void) {
	// initalize green and yellow only.
	// initialization defines the IO_structs and sets DDR
	initialize_led(GREEN);
	initialize_led(YELLOW);

	// The "sanity check".
	// When you see this pattern of lights you know the board has reset
	light_show();

	// initalize only buttonA and buttonC because they are connected to PCINT
	// NOTE: button C and the RED led are on the same line.
	initialize_button(BUTTONA);
	initialize_button(BUTTONC);
}

/******************************************************************
   MAIN
******************************************************************/

int main(void) {
	// This prevents the need to reset after flashing
	USBCON = 0;

	initialize_system();
	
	uint8_t green_on = 1;	
	
	//******  THESE FUNCTIONS SIMPLIFY THE WHILE LOOP!  *********//
	
	// Toggles the green led IF green_on is true 
	// Otherwise, ensures that green led is off
	void toggle_greenif() {
		if (green_on) {
			led_toggle(&_green);
		} else {
			led_off(&_green, INVERTED);
		}
	}
	
	// Checks both Button A and Button C 
	// If there is any button press detected, this alters green_on
	void check_buttons() {
		// Button A turns green ON 
		if (is_button_pressed(&_buttonA)) {
			led_on(&_green, INVERTED);
			green_on = 1;
		}
		// Button C turns green OFF
		if (is_button_pressed(&_buttonC)) {
			led_off(&_green,INVERTED); 
			green_on = 0;
		}
	}

	//*************************************************************//
	//*******         THE CYCLIC CONTROL LOOP            **********//
	//*************************************************************//

	// Assume both buttons start in a not pressed state.

  while(1) {
		
		/* 
		NOTE: due to timing flexibility...
		Hold down the button for one second if green does not immediately toggle. 
		 */
		
		led_toggle(&_yellow);	// time = 0	
		check_buttons();
		toggle_greenif();
		_delay_ms(500); 		// time = 500
		
		check_buttons();
		toggle_greenif();	
		_delay_ms(500); 		// time = 1000
		
	} // end while(1)

} /* end main() */