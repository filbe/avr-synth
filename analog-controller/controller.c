/*
 * avr-synth - Controller unit
 *
 * Authors:
 *  Ville-Pekka Lahti <vp@wdr.fi>
 */


#include <avr/io.h>
#include "controller.h"

void adc_init()
{
	DDRC &= ~(0x7);
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADMUX = 0;
	ADMUX  |= (1 << REFS0);
	DIDR0 = 0b00011111;
}

uint16_t adc_read(uint8_t ch)
{
	ADMUX = (ch);

	ADCSRA |= (1 << ADSC);

	while (ADCSRA & (1 << ADSC)); {}

	return ADCW;
}

int main(void)
{
	twi_init();
	adc_init();
	sei();
	uint16_t recv = 1; // address

	_delay_ms(1000);
	samplerate_set(1, 20000);
	waveform_set(1, 2);
	_delay_ms(1000);


	uint8_t  waveform, waveformex;
	uint16_t samplerate, samplerateex;
	uint32_t flanger_speed, flanger_depth, flanger_speedex, flanger_depthex;
	while (1) {
		flanger_speed = adc_read(0) * 4;
		flanger_depth = adc_read(1) / 4;
		samplerate = adc_read(2) * 39;
		waveform = adc_read(3) / 256;

		if (flanger_speed != flanger_speedex) {
			flanger_speed_set(flanger_speed);
			flanger_speedex = flanger_speed;
		}
		if (waveform != waveformex) {
			for (uint8_t i = 1; i <= 8; i++) {
				waveform_set(i, waveform);
			}

			waveformex = waveform;
		}
		if (flanger_depth != flanger_depthex) {
			flanger_depth_set(flanger_depth);
			flanger_depthex = flanger_depth;

		}
		if (samplerate != samplerateex) {
			for (uint8_t i = 1; i <= 8; i++) {
				samplerate_set(i, samplerate);

			}
			samplerateex = samplerate;
		}


		_delay_ms(30);
	}

	return 0;
}