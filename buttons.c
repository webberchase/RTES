#include "buttons.h"

/* configure the data direction for the specified on-board button.
 */
void configure_button(IO_struct * button) {
  CLEAR_BIT(*button->ddr, button->pin);
  // enable pull-up resistor
  SET_BIT(*button->port, button->pin);
}

/* Initialize the specified on-board button. Button B is not here because it has no interrupt, but you could add it.
 * The corresponding pin is set as input. The pullup resistor is enabled.
 */
void initialize_button(int button) {
  if (BUTTONA == button) {
    _buttonA = (IO_struct) { &DDRB, &PORTB, BUTTONA, &PINB };
    configure_button(&_buttonA);
  } else if (BUTTONC == button) {
    _buttonC = (IO_struct) { &DDRB, &PORTB, BUTTONC, &PINB };
    configure_button(&_buttonC);
  }
}

/* Check if the user specified button is pressed. Return 1 if so, 0 if not.
 * The bit value = 0 when pressed, 1 when not pressed.
 */
int is_button_pressed(IO_struct * button) {
  return (0 == (*button->portin & (1 << button->pin)));
}