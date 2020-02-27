#ifndef GPIO_H_
#define GPIO_H_

#include "common.h"
#include <util/delay.h>
#include <inttypes.h>

/*** CONVENTION :
 * an "l" next to a color refers to the on-board LEDs;
 * a "g" next to a color refers to the GPIO LEDs;
 * if an L or G is not present, assume on-board LED. 
 */

/*** GPIO LEDS : 
 * green2	PB6		PWM		(pin 10)
 * yellow 	PD1		2.0 Hz	(pin 2)
 * green 	PD4		2.5 Hz	(pin 4)
 * red 		PD2		1.0 Hz	(pin 0)
 */

#define GREEN2 6
#define YELLOWG 1	
#define GREENG 4
#define REDG 2

/* Timer constants */
#define _kHz 1000
#define _16M 16000000

#define YELLOW_HZ 2
//#define YELLOW_PERIOD 500

//#define GREEN_HZ 2.5
#define GREEN_PERIOD 400

//#define RED_HZ 1
#define RED_PERIOD 1000

// Global variables. MUST be defined using initialize_gpio(<COLOR>).
IO_struct _green2;
IO_struct _yellowg;
IO_struct _greeng;
IO_struct _redg;

/* configure the data direction for the specified GPIO led.
 */
 void configure_gpio(IO_struct * color);

/* initialize the data struct for the specific GPIO led. */
void initialize_gpio(int color);

/* Turn on specified GPIO LED. */ 
void gpio_on(IO_struct * color);

/* Turn off specified GPIO LED. */ 
void gpio_off(IO_struct * color);

/* Toggle the gpio led between on and off */
void gpio_toggle(IO_struct * color);

/* Flash the designated GPIO led for 250ms on, then 250ms off. 
 * Assumes led is initialized. */
void flash_gpio(IO_struct * color);

/* Flash all the initialized GPIO leds for a sanity check light show */ 
void light_show_gpio();

#endif