#include "buttons.h"
#include "leds.h"

// Comment this line out when you are done debugging. 
//#define DEBUG

uint8_t button_mask = 0;

IO_struct _buttonA = { &DDRB, &PORTB, BUTTONA, &PINB };
IO_struct _buttonC = { &DDRB, &PORTB, BUTTONC, &PINB };

INTERRUPT_struct _interruptA = 
{ PCINT_BUTTONA, (1<<BUTTONA), 0, (1<<BUTTONA), empty_function, empty_function };

INTERRUPT_struct _interruptC = 
{ PCINT_BUTTONC, (1<<BUTTONC), 0, (1<<BUTTONC), empty_function, empty_function };

void empty_function() {}


/* Configure the system to use the button. */
void initialize_button(IO_struct * button) {
  CLEAR_BIT(*button->ddr, button->pin);
  // enable pull-up resistor
  SET_BIT(*button->port, button->pin);
}

void enable_pcint(INTERRUPT_struct *state) {
	
	// PCICR: Pin Change Interrupt Control Register
	// PCIE0: Pin Change Interrupt Enable Bit: 
	// Any change on any enabled PCINT7..0 can fire ISR.
	PCICR |= (1 << PCIE0);
	
	// PCMSK0: Pin Change Mask for Interrupt0, which is for all pins 0 through 7
	// Enable interrupts on Button A (PCINT3) and Button C (PCINT0)
	PCMSK0 |= (1 << state->pcint);
	
	state->enabled = 1;
	button_mask |= state->mask; 
	state->prev_state = PINB & state->mask;
}

/* Set up callback function for any of the buttons on the board. */
void setup_button_action(INTERRUPT_struct *state, int release, void(*callback)(void)) {
	if (release) {
		state->release_fn = callback;
	} else {
		state->press_fn = callback;
	}
}

/* Check if the user specified button is pressed. Return 1 if so, 0 if not. 
 * The bit value = 0 when pressed, 1 when not pressed. 
 */
int is_button_pressed(IO_struct * button) {
  return (0 == (*button->portin & (1 << button->pin)));
}

ISR(PCINT0_vect) {
	
	// read port B, mask for enabled buttons
	uint8_t pinb_now = (PINB & button_mask);
	
	// check that the pin change persists. ignore if not. 
	_delay_ms(10);
	if (pinb_now ^ (PINB & button_mask)) {
		return;
	}

	#ifdef DEBUG 
	/* If debugging, it will be good to flash an led to know that you made 
	 * it into the ISR. Remember that RED and buttonC share the port, so don't 
	 * use the red led. If you soldered your headers, you can also set up 
	 * an LED in general IO to give yourself more options. 
	 */
	TOGGLE_BIT(*(&_green)->port, _green.pin);
	_delay_ms(150);
	TOGGLE_BIT(*(&_green)->port, _green.pin);
	#endif

	// ************************************************************/
	// ***********    COMPLETE THIS PART BELOW ********************/
	// ************************************************************/

	// record which button(s) changed state from last interrupt
	// remember that pinb_now holds state for only button A and C
	uint8_t stateA = pinb_now & _interruptA.mask;
	uint8_t stateC = pinb_now & _interruptC.mask;
	
	if (_interruptA.enabled) {
		
		// if there was a state change
		if (0 != (stateA ^ _interruptA.prev_state)) {
			
			// if it was pressed, call the press_fn()
			if (0 != stateA) {
				_interruptA.press_fn();
			// else, call the release_fn()
			} else {
				_interruptA.release_fn();
			}
			// save state as prev_state
			_interruptA.prev_state = stateA;
		}
	}
	
	// Repeat for button C 
	if (_interruptC.enabled) {
		
		if (0 != (stateC ^ _interruptC.prev_state)) {
			
			if (0 != stateC) {
				_interruptC.press_fn();
			} else {
				_interruptC.release_fn();
			}
			_interruptC.prev_state = stateC;
		}
	}

}
