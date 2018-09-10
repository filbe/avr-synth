/*
 * avr-synth - Controller unit - Messages
 *
 * Authors:
 *  Ville-Pekka Lahti <vp@wdr.fi>
 */
#include <twi.h>

#include "controller-messages.h"

void note_stop(uint8_t i2c_addr, uint8_t channel)
{
	uint8_t data[2];
	data[0] = channel;
	data[1] = CMD_NOTE_STOP;
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

void note_stop_all(uint8_t i2c_addr)
{
	uint8_t data[2];
	data[0] = 0xff;
	data[1] = CMD_NOTE_STOP;
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

void note_start(uint8_t i2c_addr, uint8_t channel, uint8_t note)
{
	uint8_t data[3];
	data[0] = channel;
	data[1] = CMD_NOTE_START;
	data[2] = note;
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

void hz_start(uint8_t i2c_addr, uint8_t channel, float hz)
{
	uint8_t data[5];
	data[0] = channel;
	data[1] = CMD_HZ_START;
	data[2] = (uint16_t)(hz/256);
	data[3] = (uint16_t)(hz) && 0xff;
	data[4] = (uint16_t)(hz*256) && 0xff;
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

void waveform_set(uint8_t i2c_addr, uint8_t wf)
{
	uint8_t data[2];
	data[0] = 0;
	data[1] = CMD_WAVEFORM_SET + wf;
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

void buffervalue_set(uint8_t i2c_addr, uint16_t buffer_index, uint8_t buffer_data)
{
	uint8_t data[6];
	data[0] = 0;
	data[1] = CMD_BUFFERVALUE_SET;
	data[2] = buffer_index >> 8;
	data[3] = buffer_index & 0xff;
	data[4] = buffer_data;
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

void volume_set(uint8_t i2c_addr, uint8_t channel, uint8_t volume)
{
	uint8_t data[4];
	data[0] = 0;
	data[1] = CMD_VOLUME_SET;
	data[2] = channel;
	data[3] = volume;
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

void finetune_set(uint8_t i2c_addr, uint16_t ft)
{
	uint8_t data[4];
	data[0] = 0;
	data[1] = CMD_FINETUNE;
	data[2] = ft >> 8;
	data[3] = ft & 0xff;
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

void arpeggio_set(uint8_t i2c_addr, uint8_t notescount, uint8_t *notes, uint8_t notedelay)
{
	uint8_t data[4 + notescount];
	data[0] = 0;
	data[1] = CMD_ARPEGGIO_SET;
	data[2] = notescount;
	data[3] = notedelay;
	for (uint8_t i = 0; i < notescount; i++) {
		data[4 + i] = notes[i];
	}
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

void arpeggio_once_set(uint8_t i2c_addr, uint8_t notescount, uint8_t *notes, uint8_t notedelay)
{
	uint8_t data[4 + notescount];
	data[0] = 0;
	data[1] = CMD_ARPEGGIO_ONCE_SET;
	data[2] = notescount;
	data[3] = notedelay;
	for (uint8_t i = 0; i < notescount; i++) {
		data[4 + i] = notes[i];
	}
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

void chord_set(uint8_t i2c_addr, uint16_t chord, uint8_t base)
{
	uint8_t data[5];
	data[0] = 0;
	data[1] = CMD_CHORD_SET;
	data[2] = chord >> 8;
	data[3] = chord & 0xff;
	data[4] = base;
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

void samplerate_set(uint8_t i2c_addr, uint16_t samplerate)
{
	uint8_t data[4];
	data[0] = 0;
	data[1] = CMD_SAMPLERATE_SET;
	data[2] = samplerate >> 8;
	data[3] = samplerate & 0xff;
	twi_writeTo(i2c_addr, data, sizeof(data), 1);
}

