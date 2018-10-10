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


	uint8_t pitch, volume, waveform, pitchex, volumeex, waveformex;
	uint16_t samplerate, samplerateex;
	while (1) {
		pitch = adc_read(0) >> 3;
		volume = adc_read(1) >> 2;
		samplerate = adc_read(2) * 39;
		waveform = adc_read(3) / 256;

		if (pitch != pitchex) {
			note_start(1, 0, pitch);
			pitchex = pitch;
		}
		if (waveform != waveformex) {
			waveform_set(1, waveform);
			waveformex = waveform;
		}
		if (volume != volumeex) {
			volume_set(1, 0, volume);
			volumeex = volume;
		}
		if (samplerate != samplerateex) {
			samplerate_set(1, samplerate);
			note_start(1, 0, pitch);
			samplerateex = samplerate;
		}
		
		
		_delay_ms(30);
	}

	return 0;
}