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

float midi[MIDI_NOTES_COUNT];

void polyphone_put_free_channel(uint8_t channel);

#endif /* __AVR_SYNTH__CONTROLLER__ */