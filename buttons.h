#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "common.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTONA 3
#define BUTTONC 0

#define PCINT_BUTTONA PCINT3
#define PCINT_BUTTONC PCINT0

typedef struct {
	uint8_t pcint;
	uint8_t mask;
	uint8_t enabled;
	uint8_t prev_state;
	void (*release_fn)(void);
	void (*press_fn)(void);
} INTERRUPT_struct;

extern IO_struct _buttonA;
extern IO_struct _buttonC;

extern INTERRUPT_struct _interruptA;
extern INTERRUPT_struct _interruptC;

/* Enable the PCINT for any of the buttons on the board.
 * This will set up for a PCINT ISR. Don't forget to call sei() in main.
 * param[in] INTERRUPT_struct *state: for A or C as defined above 
 */
void enable_pcint(INTERRUPT_struct *state);

/* Set up callback function for any of the buttons on the board. 
 * param[in] INTERRUPT_struct *state: for A or C as defined above. 
 * param[in] release: 1 (true) = callback on release, else callback on press
 * param[in] callback: function to be called when event happens
 */
void setup_button_action(INTERRUPT_struct *state, int release, void(*callback)(void));

/* Used as a callback placeholder when button is not enabled */
void empty_function();

/* Initialize the specified on-board button. 
 * The corresponding pin is set as input. 
 * THe pull-up resistor is enabled. 
 * 
 * Setup is for A and C only because Button B does not have corresponding 
 * PCINT for easy interrupt programming. 
 * 
 * WARNING: ButtonC and RED_LED share the pin, do not use both. 
 */
void initialize_button(IO_struct * button); 

/* Check if the user-specified button is pressed. Return 1 if so, 0 if not. 
 * The bit value in the PINB register is 0 when pressed, 1 when not pressed. 
 */
int is_button_pressed(IO_struct * button);

#endif