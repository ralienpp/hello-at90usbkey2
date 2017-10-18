#include <avr/io.h>
#include <avr/wdt.h>


int main() {

	MCUSR &= ~_BV(WDRF);
	wdt_disable();

	DDRD |= _BV(DDD4);
	PORTD |= _BV(PD4);

	while(1);

	return 0;
}
