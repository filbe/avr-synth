/*
 * avr-synth - Controller unit
 *
 * Authors:
 *  Ville-Pekka Lahti <vp@wdr.fi>
 */

#include "controller.h"
#include "../../avr-i2c-audio/src/chords.h"

int main(void)
{
	for (float i = 0; i < MIDI_NOTES_COUNT; i += 1.0) {
		midi[(uint8_t)(i)] = pow(2.0, (i - 69.0) * 0.083333) * 440.0;
	}
	DDRC = 255;
	twi_init();
	sei();
	uint16_t recv = 1; // address

	samplerate_set(1, 33000);

	uint8_t n[8];
	n[0] = 60;
	n[1] = 62;
	n[2] = 63;
	n[3] = 67;
	n[4] = 63;
	n[5] = 62;
	n[6] = 60;
	n[7] = 55;

	samplerate_set(1, 20000);
	samplerate_set(2, 20000);
	samplerate_set(3, 20000);
	samplerate_set(4, 20000);
	samplerate_set(5, 20000);
	samplerate_set(6, 20000);
	samplerate_set(7, 20000);
	samplerate_set(8, 20000);

	waveform_set(1, 1);


	for (uint16_t i = 0; i < 256; i++) {
		uint16_t bv = 0;



		// ORGAN
		bv += sin(2 * i / 256.0 * M_PI) * 127 + 127;
		bv += sin(4 * i / 256.0 * M_PI) * 63 + 127;
		bv += sin(8 * i / 256.0 * M_PI) * 127 + 127;
		bv += sin(16 * i / 256.0 * M_PI) * 63 + 127;
		bv += sin(32 * i / 256.0 * M_PI) * 127 + 127;
		bv += sin(64 * i / 256.0 * M_PI) * 127 + 127;
		bv += sin(128 * i / 256.0 * M_PI) * 63 + 127;
		bv += sin(256 * i / 256.0 * M_PI) * 127 + 127;
		bv += sin(512 * i / 256.0 * M_PI) * 63 + 127;





		buffervalue_set(1, i, bv / 7);
		buffervalue_set(2, i, bv / 7);
		_delay_ms(1);
	}

	while (1) {


		chord_set(2, MAJOR, 45);

		note_start(1, 0, 69); _delay_ms(100);
		note_start(1, 0, 67); _delay_ms(100);
		note_start(1, 0, 69); _delay_ms(1300);
		note_start(1, 0, 67); _delay_ms(100);
		note_start(1, 0, 65); _delay_ms(100);
		note_start(1, 0, 64); _delay_ms(100);
		note_start(1, 0, 62); _delay_ms(100);


		note_start(1, 0, 61); _delay_ms(400);

		chord_set(2, MINOR, 38);
		note_start(1, 0, 62); _delay_ms(1000);


		note_start(1, 0, 62); _delay_ms(400);
		_delay_ms(600);


		chord_set(2, MAJOR, 45);
		note_start(1, 0, 69); _delay_ms(100);
		note_start(1, 0, 67); _delay_ms(100);
		note_start(1, 0, 69); _delay_ms(1300);
		note_start(1, 0, 64); _delay_ms(200);
		note_start(1, 0, 65); _delay_ms(200);

		note_start(1, 0, 61); _delay_ms(400);
		chord_set(2, MINOR, 38);
		note_start(1, 0, 62); _delay_ms(1200);

		note_start(1, 0, 0); _delay_ms(2000);





		chord_set(2, MAJOR, 45);

		note_start(1, 0, 69 - 12); _delay_ms(100);
		note_start(1, 0, 67 - 12); _delay_ms(100);
		note_start(1, 0, 69 - 12); _delay_ms(1300);


		note_start(1, 0, 67 - 12); _delay_ms(100);
		note_start(1, 0, 65 - 12); _delay_ms(100);
		note_start(1, 0, 64 - 12); _delay_ms(100);
		note_start(1, 0, 62 - 12); _delay_ms(100);
		note_start(1, 0, 61 - 12); _delay_ms(400);

		chord_set(2, MINOR, 38);
		note_start(1, 0, 62 - 12); _delay_ms(1000);

		note_start(1, 0, 0); _delay_ms(2300);


		chord_set(2, DIM7, 37);
		for (uint8_t i = 0; i < 6; i++) {
			note_start(1, 0, 49 + (i * 3)); _delay_ms(530);
		}

		_delay_ms(1200);
		chord_set(2, SUS4, 38);
		note_start(1, 0, 64);

		_delay_ms(2000);
		chord_set(2, MAJOR, 38);
		note_start(1, 0, 66);

		_delay_ms(1000);

		_delay_ms(3500);


	}

	return 0;
}