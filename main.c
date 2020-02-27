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

	// Initalize only buttonA and buttonC because they are connected to PCINT
	// NOTE: button C and the RED led are on the same line.
	initialize_button(&_buttonA);
	initialize_button(&_buttonC);
	
	// Enable the PCINT for buttons A and C. 
	// This sets up for PCINT ISR. 
	enable_pcint(&_interruptA);
	enable_pcint(&_interruptC);
}

/* CALLBACK FUNCTIONS */

uint8_t releaseA = 0;
uint8_t releaseC = 0;

// Button A
void flagA() {
	releaseA = 1;
}

// Button C
void flagC() {
	releaseC = 1;
}

/******************************************************************
	MAIN
******************************************************************/

int main(void) {
	// This prevents the need to reset after flashing
	USBCON = 0;

	initialize_system();
	
	// Enables all interrupts
	sei();	
	
	// Connect the buttons to the appropriate interrupt actions
	// Release button A
	setup_button_action(&_interruptA, 1, flagA);
	// Release button C
	setup_button_action(&_interruptC, 1, flagC);
	
	// for A and C button state machines
	uint8_t a_state = 1;
	uint8_t c_state = 1;
	
	// timer variable declarations
	uint16_t yms_tick = 0;
	uint16_t gms_tick = 0;
	uint16_t yellow_timer = YELLOW_PERIOD;
	uint8_t ytimer_expired = 0; // false
	uint16_t green_timer = GREEN_PERIOD;
	uint8_t gtimer_expired = 0; // false
	
	
	//***********************************************************//
	//******         THE CYCLIC CONTROL LOOP            *********//
	//***********************************************************//

	while(1) {
		// BUTTON A SM 
		switch (a_state) {
			case (1):
				if(releaseA) {
					led_on(&_yellow, 0);
					a_state = 2;
					releaseA = 0;
				}
				break;
			case (2):
				if(ytimer_expired) {
					led_toggle(&_yellow); // Toggle LED 
					ytimer_expired = 0;   // false
				}
				if(releaseA) {
					a_state = 3;
					releaseA = 0;
				}	
				break;
			case (3): 
				if(releaseA) {
					led_off(&_yellow, 0);
					a_state = 1;
					releaseA = 0;
				}
				break;
		} // end A SM
		
		// BUTTON C SM 
		switch (c_state) {
			case (1):
				if(releaseC) {
					led_on(&_green, INVERTED);
					c_state = 2;
					releaseC = 0;
				}
				break;
			case (2):
				if(gtimer_expired) {
					led_toggle(&_green); // Toggle LED 
					gtimer_expired = 0;   // false
				}
				if(releaseC) {
					c_state = 3;
					releaseC = 0;
				}	
				break;
			case (3): 
				if(releaseC) {
					led_off(&_green, INVERTED);
					c_state = 1;
					releaseC = 0;
				}
				break;
		} // end C SM
		
		// Yellow Timer
		if (yms_tick >= yellow_timer) {
			ytimer_expired = 1; // true
			yellow_timer += YELLOW_PERIOD;
		}
		//Green Timer
		if (gms_tick >= green_timer) {
			gtimer_expired = 1; // true
			green_timer += GREEN_PERIOD;
		}
		_delay_ms(TICK_PERIOD); 
		yms_tick += TICK_PERIOD;
		gms_tick += TICK_PERIOD;
		
	} // end while(1)

} /* end main() */