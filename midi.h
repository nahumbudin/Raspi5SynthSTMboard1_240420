/* 
 *	@ file		midiParser.h
*	@ author	Nahum Budin
*	@ date		18 - June -2024
*	@ version	2.0
*
*	@ brief		MidiParser
*
*				Assemnle incoming bytes into MIDI commands messages
*				Gets a new data byte from input queue, and look for byte value > 0x80, 
*				which means a MIDI status byte (command starts).
*				(ignore messages that starts with 0xf1 - 6, 0xf8 - e)
*				If there was a previos command and byte < 0x80, than rebuilt command as a running - status command(currently not supported).
*				According to the status byte get MIDI length for non Sysex nor a Reset command that
*				starts with an 0xfx(0xf0, 0xf7, 0xff).
*
*  Based on Music Open Lab Library.
*	Copyright AdjHeart Nahum Budin May 2017 and
*
*	History : version 1.0   13 - Oct - 2019
*/


#ifndef __MIDI
#define __MIDI

#include <stdint.h>

#define _MIDI_SYSEX_START 0xF0
#define _MIDI_SYSEX_END 0xF7

// TODO: temp
#define _MIDI_ADJ_SYSEX_VENDOR_ID_0			0
#define _MIDI_ADJ_SYSEX_VENDOR_ID_1			0x35 
#define _MIDI_ADJ_SYSEX_VENDOR_ID_2			0x56

#define _MIDI_PARSER_STATE_IDLE 0
#define _MIDI_PARSER_STATE_COMMAND_IN_PROGRESS 1
#define _MIDI_PARSER_STATE_SYSEX_IN_PROGRESS 2
#define _MIDI_PARSER_STATE_NEW_COMMAND_AVAILABLE 3

#define _MIDI_PARSER_COMMAND_BUFFER_LEN 128

/* How many bytes in MIDI commands. Last is unknown for Sysex */
static uint8_t midi_commands_length[8] = { 3, 3, 3, 3, 2, 2, 3, 0 };

int MIDI_PARSER_add_byte(uint8_t new_byte);

uint8_t* MIDI_PARSER_get_last_command();

uint8_t* MIDI_COMMAND_build_sysex_control_mssg(uint8_t control_id, uint16_t control_value);


#endif
