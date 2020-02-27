#ifndef LEDS_H_
#define LEDS_H_

#include "common.h"
#include <util/delay.h>
#include <inttypes.h>

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

// These function as enumerated types (using pin numbers as values) 
#define YELLOWL 7
#define GREENL 5
#define REDL 0

// Global variables. They MUST be defined using initialize_led(<COLOR>).
IO_struct _yellowl;
IO_struct _greenl; 
IO_struct _redl;

/* configure the data direction for the specified on-board led.
 */
void configure_led(IO_struct * color);

/* initialize the data struct for the specific on-board led. */
void initialize_led(int color);

/* Turn on specified on-board LED. Indicate if inverted (0 turns led on) */ 
void led_on(IO_struct * color, int inverted); 

/* Turn off specified on-board LED. Indicate if inverted (1 turns led off); */ 
void led_off(IO_struct * color, int inverted); 

/* Toggle the led between on and off */ 
void led_toggle(IO_struct * color); 

/* Flash the designated on-board led for 250ms on, then 250ms off. 
 * Assumes led is initialized. */
void flash_led(IO_struct * color, int inverted);

/* Flash all the initialized leds for a sanity check light show */ 
void light_show_led(); 

#endif