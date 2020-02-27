/*
To find locations (i.e. ports) of I/O, consult the AStar pinout.
The information has been summarized in the AStar DataCheatSheet document
on Github.
Control of the I/O can be found in the AtMega Datasheet.
*/

/*
This is a demonstration of interacting with the on-board led.
Green LED = Port D, pin 5
And a button.
Button A = Port B, pin 3
*/

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

int main() {

  // This prevents the need to reset after flashing
  //USBCON = 0;

  // Configures green LED pin as output
  // DDRD: Data Direction Register for Port D
  // DDC5: Data Direction pin/bit mask of Port D, pin 5
  DDRD |= ( 1 << DDD5 );
  
  // Configures yellow LED pin as output 
  // DDRC: Data Direction Register for Port D
  // DDC7: Data Direction pin/bit mask of Port C, pin 7
  DDRC |= ( 1 << DDC7 );

  // Configure button A: Port B, pin 3 as input
  DDRB &= ~(1<<DDB3);
  
  // Configure button C: Port B, pin 0 as input
  DDRB &= ~(1<<DDB0);

  // Enable Button pull-up resistor for Button A
  PORTB |= (1<<PORTB3);
  
  // Enable Button pull-up resistor for Button C
  PORTB |= (1<<PORTB0);

  // Perform sanity check.
  // Is the LED configured properly?
  // Is the board reset and running main?

  // Green ON then off quick for sanity check
  // Yellow ON then off quick for sanity check
  // Green is turned on by setting port low!! 
  // Yellow is turned on by setting port high.
  int i;
  for (i=0; i<3; i++) {
    // Green ON
    PORTD &= ~( 1 << PORTD5 );
	// Yellow ON 
	PORTC |= ( 1 << PORTC7 );
    _delay_ms(250);

    // Green OFF
    PORTD |= ( 1 << PORTD5 );
	// Yellow OFF
	PORTC &= ~( 1 << PORTC7 );	
    _delay_ms(250);
  }

  while(1) {
    // If button A is pressed, green led is on.
    if (0 == (PINB & (1 << 3))) {
      PORTD &= ~( 1 << PORTD5 );
    } else {
      PORTD |= ( 1 << PORTD5 );
    }
	// If button C is pressed, yellow led is on. 
	if (0 == (PINB & (1 << 0))) {
	  PORTC |= ( 1 << PORTC7 );
	} else {
	  PORTC &= ~( 1 << PORTC7 );	
	}	
  } // end while
} // end main()