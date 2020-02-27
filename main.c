#include "common.h"

#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"
#include "gpio.h"
#include "buttons.h"


/******************************************************************
	TIMER FUNCTIONS
******************************************************************/

// Global variable for timer
volatile uint32_t ms_ticks = 0;

// Red period is 1000 ms, Green period is 400 ms
volatile uint32_t red_timer = RED_PERIOD;
volatile uint32_t green_timer = GREEN_PERIOD;

// PWM variables and constants
#define TOP 0x00ff
volatile uint32_t increment = TOP / 0x000f;
volatile uint32_t new_duty_cycle = TOP / 2;
volatile uint8_t direction = 0;

/*
 * Timer 0 is used for the RED led. 
 * Interrupt fires every 1 ms, 1000 Hz.
 */
void setup_timer_0(void) {
	// Set WGM bits for CTC mode - TIMER 0
	CLEAR_BIT(TCCR0B, WGM02);	// 0
	SET_BIT(TCCR0A, WGM01);		// 1
	CLEAR_BIT(TCCR0A, WGM00);	// 0
	
	// Set CS Bits for prescalar 64 - TIMER 0
	CLEAR_BIT(TCCR0B, CS02);	// 0
	SET_BIT(TCCR0B, CS01);		// 1
	SET_BIT(TCCR0B, CS00);		// 1
	
	// Set match to 250 - TIMER 0
	OCR0A = (_16M / 64) / _kHz;
	
	// Enable Interrupt (mask) - TIMER 0
	SET_BIT(TIMSK0, OCIE0A);
}

/*
 * Timer 1 is used for the GREEN2 led. 
 * Controls PWM signal to adjust frequency
 */
void setup_timer_1(void) {
	// Set top value to 0xffff - TIMER 1
	ICR1 = TOP;
	
	// Set WGM bits for PVM, phase correct - TIMER 1
	SET_BIT(TCCR1B, WGM13);		// 1
	CLEAR_BIT(TCCR1B, WGM12);	// 0
	SET_BIT(TCCR1A, WGM11);		// 1
	CLEAR_BIT(TCCR1A, WGM10);	// 0
	
	// Set CS Bits for prescalar 1024 - TIMER 1
	SET_BIT(TCCR1B, CS12);		// 1
	CLEAR_BIT(TCCR1B, CS11);	// 0
	SET_BIT(TCCR1B, CS10);		// 1
	
	// Set COM Bits for clear  on compare match - TIMER 1
	SET_BIT(TCCR1A, COM1B1); 	// 1
	CLEAR_BIT(TCCR1A, COM1B0);	// 0
	
	// Set duty cycle - TIMER 1
	// this is set by button C release
	// Should fluctuate between 0 and 0xffff
	OCR1B = new_duty_cycle;
	
}

/*
 * Timer 3 is used for the YELLOW led. 
 * Interrupt fires every 250 ms, 2 Hz.
 */
void setup_timer_3(void) {
	// Set WGM bits for CTC mode - TIMER 3
	CLEAR_BIT(TCCR3B, WGM33);	// 0
	SET_BIT(TCCR3B, WGM32);		// 1
	CLEAR_BIT(TCCR3A, WGM31);	// 0
	CLEAR_BIT(TCCR3A, WGM30);	// 0
	
	// Set CS Bits for prescalar 256 - TIMER 3
	SET_BIT(TCCR3B, CS32);		// 1
	CLEAR_BIT(TCCR3B, CS31);	// 0
	CLEAR_BIT(TCCR3B, CS30);	// 0
	
	// Set match 31,250- TIMER 3
	OCR3A = (_16M / 256) / YELLOW_HZ;
	
	// Enable Interrupt (mask) - TIMER 3
	SET_BIT(TIMSK3, OCIE3A);
}

/* Timer 0: 
 * 1000 Hz, every 1 ms 
 * RED led
 */
ISR(TIMER0_COMPA_vect) {
	ms_ticks++;
}

/* Timer 3: 
 * 2 Hz, every 250 ms 
 * YELLOW led
 */
ISR(TIMER3_COMPA_vect) {
	led_toggle(&_yellowl);
	gpio_toggle(&_yellowg);
}


/******************************************************************
	BUTTON FUNCTIONS
******************************************************************/

// Flags
uint8_t aPress = 0;
uint8_t cPress = 0;


/* Button A Release */
void flagA(void) {
	aPress = 1;
}

/* Button C Release */
void flagC(void) {
	cPress = 1;
}

void setup_buttonA(void) {
	/* Initalize buttonA */
	initialize_button(&_buttonA);
	
	/* Enable the PCINT for buttonA */
	enable_pcint(&_interruptA);
	
	/* Connect buttonA to the appropriate interrupt actions */
	// Release 
	setup_button_action(&_interruptA, RELEASE, flagA);
}

void setup_buttonC(void) {	
	/* Initalize buttonC */
	initialize_button(&_buttonC);
	
	/* Enable the PCINT for buttonC */
	enable_pcint(&_interruptC);
	
	/* Connect the buttonC to the appropriate interrupt actions */
	// Release  C
	setup_button_action(&_interruptC, RELEASE, flagC);
}


/******************************************************************
	ALL INITIALIZATION
******************************************************************/

/* Set WGM bits in TCCRnA & TCCRnB, 
 * Set Prescalar and Match, and
 * Enable Interrupt for Timer 1 & 3 
 */
void initialize_timers(void) {
	setup_timer_0();
	setup_timer_1();
	setup_timer_3();
}

/* Initalize only buttonA and buttonC because they are connected to PCINT.
 * NOTE: button C and the RED led are on the same line. 
 * Enable the PCINT for buttons A and C. This sets up for PCINT ISR. 
 * Connect the buttons to the appropriate interrupt actions.
 */
 // Only need Button A for lab 4...!
void initialize_buttons(void) {
	setup_buttonA();
	setup_buttonC();
}

void initialize_system(void) {
	/* LEDS */
	//initialize_led(YELLOWL);	
	//initialize_led(GREENL);
	//initialize_led(REDL);
	/* GPIOS */
	initialize_gpio(GREEN2);
	initialize_gpio(YELLOWG);	
	initialize_gpio(GREENG);
	initialize_gpio(REDG);

	/* LED sanity check */
	//light_show_led();
	light_show_gpio();

	/* Set up Timer 0 and Timer 3 */
	initialize_timers();
	
	/* Set up Button A and Button C */
	initialize_buttons();
}

 
/******************************************************************
	MAIN
******************************************************************/

int main(void) {
	// This prevents the need to reset after flashing
	USBCON = 0;

	// Set up LEDs, timers, and buttons
	initialize_system();

	// Enables all interrupts
	sei();	

	while(1) {
		
		// RED TIMER - toggle every 500 ms
		if (ms_ticks >= red_timer) {
			//led_toggle(&_redl);
			gpio_toggle(&_redg);
			red_timer = ms_ticks + RED_PERIOD;
		}
		
		// BUTTON A - toggle green gpio 5 times
		if (aPress) {
			int i = 0;
			while (i < 10) {
				if (ms_ticks >= green_timer) {
					//led_toggle(&_greenl);
					gpio_toggle(&_greeng);
					green_timer = ms_ticks + GREEN_PERIOD;
					i++;
				}
			}
			aPress = 0;
		}
		
		// BUTTON C - adjust PWM frequency
		if (cPress) {
			if (direction) {
				new_duty_cycle += INCREMENT;
				if (new_duty_cycle >= TOP) {
					direction = 0;
					new_duty_cycle = TOP;
				}
				cPress = 0;
			} else {
				new_duty_cycle += -INCREMENT;
				if (new_duty_cycle <= 0 || new_duty_cycle > TOP) {
					direction = 1;
					new_duty_cycle = 0;
				}
				cPress = 0;
			}
		}
		OCR1B = new_duty_cycle;
		
	} // end while(1)
	
} /* end main() */