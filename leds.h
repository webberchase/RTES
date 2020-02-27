#ifndef LEDS_H_
#define LEDS_H_

#include "common.h"
#include <util/delay.h>
#include <inttypes.h>

// These function as enumerated types (using pin numbers as values) 
#define GREEN 5
#define RED 0
#define YELLOW 7

// Timer constants
#define YELLOW_PERIOD 250
#define GREEN_PERIOD 1250
#define TICK_PERIOD 100

//#define delay250 _delay_ms(250)
//#define delay1250 _delay_ms(1250)

// Global variables. They MUST be defined using initialize_led(<COLOR>).
IO_struct _green; 
IO_struct _red;
IO_struct _yellow;

/* initialize the data struct for the specific on-board led. */
void initialize_led(int color);

/* Flash the designated on-board led for 250ms on, then 250ms off. 
 * Assumes led is initialized. */
void flash_led(IO_struct * color, int inverted);

/* Flash all the initialized leds for a sanity check light show */ 
void light_show(); 

/* Turn on specified on-board LED. Indicate if inverted (0 turns led on) */ 
void led_on(IO_struct * color, int inverted); 

/* Turn off specified on-board LED. Indicate if inverted (1 turns led off); */ 
void led_off(IO_struct * color, int inverted); 

/* Toggle the led between on and off */ 
void led_toggle(IO_struct * color); 

#endif