/*
 * avr-synth - Controller unit - I2C commands header
 *
 * Authors:
 *  Ville-Pekka Lahti <vp@wdr.fi>
 */
#ifndef __CONTROLLER_COMMANDS_H__
#define __CONTROLLER_COMMANDS_H__

#define CMD_NOTE_STOP			0x00
#define CMD_NOTE_START			0x01
#define CMD_WAVEFORM_SET		0x10
#define CMD_FINETUNE			0x20
#define CMD_BUFFERVALUE_SET		0x31
#define CMD_ARPEGGIO_SET		0x51
#define CMD_ARPEGGIO_ONCE_SET	0x52
#define CMD_CHORD_SET			0x61
#define CMD_SAMPLERATE_SET		0xf0

#endif /* __CONTROLLER_COMMANDS_H__ */