#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "common.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTONA 3
#define BUTTONC 0

// These must be defined using initialize_button()
IO_struct _buttonA;
IO_struct _buttonC; 

/* Initialize the specified on-board button. 
 * The corresponding pin is set as input. 
 * THe pull-up resistor is enabled. 
 */
void initialize_button(int button); 

/* Check if the user-specified button is pressed. Return 1 if so, 0 if not. 
 * The bit value in the PINB register is 0 when pressed, 1 when not pressed. 
 */ 
int is_button_pressed(IO_struct * button);

#endif