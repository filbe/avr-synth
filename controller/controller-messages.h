/*
 * avr-synth - Controller unit - Messages
 *
 * Authors:
 *  Ville-Pekka Lahti <vp@wdr.fi>
 */

#include <stdint.h>
#include "controller-commands.h"
#ifndef __CONTROLLER_MESSAGES_H__
#define __CONTROLLER_MESSAGES_H__

void note_stop(uint8_t i2c_addr, uint8_t channel);
void note_stop_all(uint8_t i2c_addr);
void note_start(uint8_t i2c_addr, uint8_t channel, uint8_t note);
void hz_start(uint8_t i2c_addr, uint8_t channel, float hz);
void waveform_set(uint8_t i2c_addr, uint8_t wf);
void volume_set(uint8_t i2c_addr, uint8_t channel, uint8_t volume);
void finetune_set(uint8_t i2c_addr, uint16_t ft);
void arpeggio_set(uint8_t i2c_addr, uint8_t notescount, uint8_t *notes, uint8_t notedelay);
void arpeggio_once_set(uint8_t i2c_addr, uint8_t notescount, uint8_t *notes, uint8_t notedelay);
void chord_set(uint8_t i2c_addr, uint16_t chord, uint8_t base);
void samplerate_set(uint8_t i2c_addr, uint16_t samplerate);

#endif /* __CONTROLLER_MESSAGES_H__ */