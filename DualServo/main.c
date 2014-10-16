


#include <avr/io.h>
#include <util/delay.h>
#define LED PB4
#define RPOT 3  		// right pot adc channel
#define LPOT 1  		// left pot adc channel
#define RSERVO PB0 		// right servo output pin (also OC0A)
#define LSERVO PB1		// left servo output pin (also OC0B)

#define LED_ON()			PORTB |= _BV(LED)
#define LED_OFF()			PORTB &= ~_BV(LED)


int main (void)
{
	DDRB = _BV(LED) | _BV(RSERVO) | _BV(LSERVO);  // set data direction bits to output

	DIDR0 = (1<<ADC1D)|(1<<ADC3D); // must match RPOT AND LPOT above

	ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0)	//Prescaler at 64 so we have an 125Khz clock source
					|(0<ADATE); 	// Auto trigger off

	ADMUX = (0<<REFS1) | (0<<REFS0);			//Avcc(+5v) as voltage reference

	ADCSRB = (1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0);	//ADC in free-running mode

	ADCSRA |= (1<<ADEN);                //Power up the ADC

	int mode = 0;

	while(1)
	{

		while(1)
		{
			int rvalue;
			int lvalue;

			ADCSRA |= _BV(ADIF);				// this clears conversion complete (just in case)
			ADMUX = (ADMUX & 0xf0) | RPOT;	// choose RPOT adc channel
			ADCSRA |= (1<<ADSC);            //Start converting
			while( (ADCSRA & _BV(ADIF)) == 0);  // loop while waiting for ADC
			rvalue = ADC;

			ADCSRA |= _BV(ADIF);				// this clears conversion complete (just in case)
			ADMUX = (ADMUX & 0xf0) | LPOT;	// choose RPOT adc channel
			ADCSRA |= (1<<ADSC);            //Start converting
			while( (ADCSRA & _BV(ADIF)) == 0);  // loop while waiting for ADC
			lvalue = ADC;

			PORTB ^= _BV(LED);		// toggle led


			if (mode == 255)
			{
				long xval = (long)rvalue * 2000L / 1024L + 100L; // compute value giving 500uS to 2500uS
				xval = xval * 8 / 4; // compute cpu cycles for that value (8 Mhz / 4 cycles per cnt)
				int v1 = xval;
				xval = (long)lvalue * 2000L / 1024L + 100L; // compute value giving 500uS to 2500uS
				xval = xval * 8 / 4; // compute cpu cycles for that value (8 Mhz / 4 cycles per cnt)
				int v2 = xval;
				xval = (long)(1024-rvalue) * 2000L / 1024L + 100L; // compute value giving 500uS to 2500uS
				xval = xval * 8 / 4; // compute cpu cycles for that value (8 Mhz / 4 cycles per cnt)
				int v3 = xval;
				xval = (long)(1024-lvalue) * 2000L / 1024L + 100L; // compute value giving 500uS to 2500uS
				xval = xval * 8 / 4; // compute cpu cycles for that value (8 Mhz / 4 cycles per cnt)
				int v4 = xval;

				PORTB |= _BV(RSERVO);	// right servo bit on
				PORTB &= ~_BV(LSERVO);	// left servo bit on
				_delay_us(400);
				if ((PINB & _BV(RSERVO)) == 0) break;
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_loop_2(v1);		// delay
				if ((PINB & _BV(LSERVO)) == 0) break;
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(400);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_loop_2(v2);		// delay
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(400);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_loop_2(v3);		// delay
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(400);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_loop_2(v4);		// delay
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(400);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(1100);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(400);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(1100);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(400);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(1100);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(400);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(1100);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_us(400);
				PORTB ^= _BV(RSERVO) | _BV(LSERVO); //flip
				_delay_ms(11);

			}
			else
			{
				PORTB |= _BV(RSERVO);	// right servo bit on
				if (mode & _BV(RSERVO)) rvalue = 1024 - rvalue;
				long xval = (long)rvalue * 2000L / 1024L + 500L; // compute value giving 500uS to 2500uS
				xval = xval * 8 / 4; // compute cpu cycles for that value (8 Mhz / 4 cycles per cnt)
				_delay_loop_2(xval);		// delay
				if ((PINB & _BV(RSERVO)) == 0) break;
				PORTB &= ~_BV(RSERVO);	// right servo off

				PORTB |= _BV(LSERVO);	// left servo bit on
				if (mode & _BV(LSERVO)) lvalue = 1024 - lvalue;
				xval = (long)lvalue * 2000L / 1024L + 500L; // compute value giving 500uS to 2500uS
				xval = xval * 8 / 4; // compute cpu cycles for that value
				_delay_loop_2(xval);		// delay
				if ((PINB & _BV(LSERVO)) == 0) break;
				PORTB &= ~_BV(LSERVO);	// left servo off

				_delay_ms(18);
			}

		}
		// we pushed a button
		LED_OFF();

		PORTB &= ~(_BV(LSERVO) | _BV(RSERVO));  // servos off
		_delay_ms(20);
		PORTB |= (_BV(LSERVO) | _BV(RSERVO));  // servos on to check both buttons
		_delay_us(500);  // minumum pulse
		int buttons = (~PINB) & (_BV(LSERVO) | _BV(RSERVO));

		while((PINB & (_BV(LSERVO) | _BV(RSERVO))) != (_BV(LSERVO) | _BV(RSERVO)) )
		{
			// loop until buttons let go
			// while serving a minimum pulse
			PORTB &= ~(_BV(LSERVO) | _BV(RSERVO));  // servos off
			_delay_ms(20);
			PORTB |= (_BV(LSERVO) | _BV(RSERVO));  // servos on to check both buttons
			_delay_us(500);  // minumum pulse
			buttons |= (~PINB) & (_BV(LSERVO) | _BV(RSERVO));
		}
		PORTB &= ~(_BV(LSERVO) | _BV(RSERVO));  // servos off

		if (mode == 255) mode = 0;  // from ppm mode to normal
		else
			if ((buttons & (_BV(LSERVO) | _BV(RSERVO))) == (_BV(LSERVO) | _BV(RSERVO)))
				mode = 255;  // ppm mode when both buttons pushed
			else
				mode = mode ^ buttons;  // flip modes based on buttons

		if (mode == 255)
		{
			// morse P .--.
			_delay_ms(300);
			LED_ON(); _delay_ms(100); LED_OFF();
			_delay_ms(100);
			LED_ON(); _delay_ms(300); LED_OFF();
			_delay_ms(100);
			LED_ON(); _delay_ms(300); LED_OFF();
			_delay_ms(100);
			LED_ON(); _delay_ms(100); LED_OFF();
			_delay_ms(300);
		}
		else
		{
			if ((mode & buttons) == 0)
			{
				// morse N .--
				_delay_ms(300);
				LED_ON(); _delay_ms(100); LED_OFF();
				_delay_ms(100);
				LED_ON(); _delay_ms(300); LED_OFF();
				_delay_ms(100);
				LED_ON(); _delay_ms(300); LED_OFF();
				_delay_ms(300);
			}
			else
			{
				// morse R .-.
				_delay_ms(300);
				LED_ON(); _delay_ms(100); LED_OFF();
				_delay_ms(100);
				LED_ON(); _delay_ms(300); LED_OFF();
				_delay_ms(100);
				LED_ON(); _delay_ms(100); LED_OFF();
				_delay_ms(300);
			}

		}
	}

}
