/*
 * avr-synth - Controller unit
 *
 * Authors:
 *  Ville-Pekka Lahti <vp@wdr.fi>
 */


#include <avr/io.h>
#include "controller.h"
#include "../../avr-i2c-audio/src/chords.h"

#define BAUD 31250
#define MIDIUBRR F_CPU/16/BAUD-1

#define USART_RX_BUF_SIZE	16
volatile uint8_t usart_rx_buf[USART_RX_BUF_SIZE];
volatile uint8_t usart_rx_buf_read_cursor = 0;
volatile uint8_t usart_rx_buf_write_cursor = 0;
volatile uint8_t usart_rx_buf_available = 0;

volatile uint8_t fe_reset = 0;
volatile uint8_t fe_first = 0;

struct stack {
	uint8_t data[24];
	int8_t top;
} available_channel_stack;


volatile int8_t keyboard_note_channel[140];

void polyphony_init()
{
	available_channel_stack.top = -1;
	for (uint8_t i = 0; i < 24; i++) {
		polyphony_put_free_channel(23 - i);
	}
	for (uint8_t i = 0; i < 140; i++) {
		keyboard_note_channel[i] = -1;
	}
}

int polyphony_get_free_channel()
{
	if (available_channel_stack.top == -1)
		return -1;
	uint8_t ret = available_channel_stack.data[available_channel_stack.top];
	available_channel_stack.top--;
	return ret;
}

void polyphony_put_free_channel(uint8_t channel)
{
	available_channel_stack.top++;
	available_channel_stack.data[available_channel_stack.top] = channel;
}

void polyphone_free_channel_by_note(uint8_t note)
{
	note_stop(keyboard_note_channel[note] / 3 + 1, keyboard_note_channel[note] % 3);
	polyphony_put_free_channel(keyboard_note_channel[note]);
	keyboard_note_channel[note] = -1;
}

void polyphony_play_note(uint8_t note, uint8_t volume)
{
	uint8_t freechan = polyphony_get_free_channel();
	if (freechan != -1) {
		keyboard_note_channel[note] = freechan;
		note_start(freechan / 3 + 1, freechan % 3, note);
		volume_set(freechan / 3 + 1, freechan % 3, volume);
	}

}

void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	/* UBRR0H contains the 4 most significant bits of the
	baud rate. UBRR0L contains the 8 least significant
	bits.*/
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
	UCSR0C |= (1 << UCSZ00 | (1 << UCSZ01) | (1 << UCPOL0));
}


ISR(USART_RX_vect)
{
	while (UCSR0A & (1 << RXC0)) {
		usart_rx_buf[usart_rx_buf_write_cursor] = UDR0;
		usart_rx_buf_write_cursor = (usart_rx_buf_write_cursor + 1) % USART_RX_BUF_SIZE;
		usart_rx_buf_available++;
	}

}


uint8_t midi_available()
{
	return usart_rx_buf_available >= 2;

}
volatile uint8_t midistatus = 0;
void midi_read_buffer(uint8_t * out)
{
	if ((usart_rx_buf[usart_rx_buf_read_cursor] >= 0x90 && usart_rx_buf[usart_rx_buf_read_cursor] < 0xc0) || (usart_rx_buf[usart_rx_buf_read_cursor] >= 0xe0 && usart_rx_buf[usart_rx_buf_read_cursor] < 0xf0)) {
		midistatus = usart_rx_buf[usart_rx_buf_read_cursor];
		usart_rx_buf_available -= 1;
		out[0] = usart_rx_buf[usart_rx_buf_read_cursor]; usart_rx_buf[usart_rx_buf_read_cursor] = 0;
		usart_rx_buf_read_cursor = (usart_rx_buf_read_cursor + 1) % USART_RX_BUF_SIZE;
		out[1] = usart_rx_buf[usart_rx_buf_read_cursor]; usart_rx_buf[usart_rx_buf_read_cursor] = 0;
		usart_rx_buf_read_cursor = (usart_rx_buf_read_cursor + 1) % USART_RX_BUF_SIZE;
		while (!midi_available()) {}; usart_rx_buf_available -= 2;
		out[2] = usart_rx_buf[usart_rx_buf_read_cursor]; usart_rx_buf[usart_rx_buf_read_cursor] = 0;
		usart_rx_buf_read_cursor = (usart_rx_buf_read_cursor + 1) % USART_RX_BUF_SIZE;
	} else {
		out[0] = midistatus;
		usart_rx_buf_available -= 2;
		out[1] = usart_rx_buf[usart_rx_buf_read_cursor]; usart_rx_buf[usart_rx_buf_read_cursor] = 0;
		usart_rx_buf_read_cursor = (usart_rx_buf_read_cursor + 1) % USART_RX_BUF_SIZE;
		out[2] = usart_rx_buf[usart_rx_buf_read_cursor]; usart_rx_buf[usart_rx_buf_read_cursor] = 0;
		usart_rx_buf_read_cursor = (usart_rx_buf_read_cursor + 1) % USART_RX_BUF_SIZE;
	}



}

int main(void)
{

	polyphony_init();
	for (float i = 0; i < MIDI_NOTES_COUNT; i += 1.0) {
		midi[(uint8_t)(i)] = pow(2.0, (i - 69.0) * 0.083333) * 440.0;
	}
	DDRC = 255;
	twi_init();
	USART_Init(MIDIUBRR);
	sei();
	uint16_t recv = 1; // address


	uint8_t n[8];
	n[0] = 60;
	n[1] = 62;
	n[2] = 63;
	n[3] = 67;
	n[4] = 63;
	n[5] = 62;
	n[6] = 60;
	n[7] = 55;

	samplerate_set(1, 10000);
	samplerate_set(2, 20000);
	samplerate_set(3, 20000);
	samplerate_set(4, 20000);
	samplerate_set(5, 20000);
	samplerate_set(6, 20000);
	samplerate_set(7, 20000);
	samplerate_set(8, 20000);

	waveform_set(1, 2);
	waveform_set(2, 2);
	waveform_set(3, 2);
	waveform_set(4, 2);
	waveform_set(5, 2);
	waveform_set(6, 2);
	waveform_set(7, 2);
	waveform_set(8, 2);



	for (uint16_t i = 0; i < 256; i++) {
		uint16_t bv = 0;

		/*
		bv += sin(2 * i / 256.0 * M_PI) * 127 + 127;
		bv += sin(4 * i / 256.0 * M_PI) * 63 + 127;
		bv += sin(8 * i / 256.0 * M_PI) * 127 + 127;
		bv += sin(16 * i / 256.0 * M_PI) * 63 + 127;
		bv += sin(32 * i / 256.0 * M_PI) * 127 + 127;
		bv += sin(64 * i / 256.0 * M_PI) * 127 + 127;
		bv += sin(128 * i / 256.0 * M_PI) * 63 + 127;
		bv += sin(256 * i / 256.0 * M_PI) * 127 + 127;
		bv += sin(512 * i / 256.0 * M_PI) * 63 + 127;


		bv += sin(2 * i / 256.0 * M_PI) * 127 + 127;

		buffervalue_set(1, i, bv / 7);
		buffervalue_set(2, i, bv / 7);
		buffervalue_set(3, i, bv / 7);
		buffervalue_set(4, i, bv / 7);
		buffervalue_set(5, i, bv / 7);
		buffervalue_set(6, i, bv / 7);
		buffervalue_set(7, i, bv / 7);
		buffervalue_set(8, i, bv / 7);
		_delay_ms(1);
		*/
	}




	uint8_t midi[3];

	while (1) {
		if (midi_available()) {

			midi_read_buffer(midi);

			if (midi[0] >= 0x90 || midi[0] <= 0x91) {

				if (midi[1] >> 7 != 1) {
					if (midi[2] != 0) {
						polyphony_play_note(midi[1], midi[2]);
					} else {
						polyphone_free_channel_by_note(midi[1]);
					}

				}

			}





		}
	}


	for (uint16_t i = 0; i < 256; i++) {
		uint16_t bv = 0;


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
		//_delay_ms(1);
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