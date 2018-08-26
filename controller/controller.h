/*
 * avr-synth - Controller unit
 *
 * Authors:
 *  Ville-Pekka Lahti <vp@wdr.fi>
 */
#ifndef __AVR_SYNTH__CONTROLLER__
#define __AVR_SYNTH__CONTROLLER__
#include <avr-sound.h>
#include <util/delay.h>
#include <twi.h>

#include "controller-messages.h"

#ifndef MIDI_NOTES_COUNT
#define MIDI_NOTES_COUNT 140
#endif

void note_stop(uint8_t i2c_addr, uint8_t channel);
void note_start(uint8_t i2c_addr, uint8_t channel, uint8_t note);

void waveform_set(uint8_t i2c_addr, uint8_t wf);
void finetune_set(uint8_t i2c_addr, uint16_t ft);
void arpeggio_set(uint8_t i2c_addr, uint8_t notescount, uint8_t *notes, uint8_t notedelay);
void chord_set(uint8_t i2c_addr, uint16_t chord, uint8_t base);
void samplerate_set(uint8_t i2c_addr, uint16_t samplerate);

float midi[MIDI_NOTES_COUNT];

#endif /* __AVR_SYNTH__CONTROLLER__ */