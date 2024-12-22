/* 
 *	@ file		midi.h
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
*				The function returns the parser state.
*				When a full valid command is collected, the function returns a status of _MIDI_PARSER_STATE_NEW_COMMAND_AVAILABLE.
*				At this point the command buffer shall be read and a dummy call to the function should be perfotmed to initialize the parser.
*
*  Based on Music Open Lab Library.
*	Copyright AdjHeart Nahum Budin May 2017 and
*
*	History : version 1.0   13 - Oct - 2019
*/

#include "midi.h"

/* This buffer holds the parssed MIDI message. The 1st byte indicates the message length */
uint8_t command_buffer[_MIDI_PARSER_COMMAND_BUFFER_LEN];
uint8_t command_buffer_index = 1;
uint8_t state = _MIDI_PARSER_STATE_IDLE;
uint8_t command_in_progress;
uint8_t command_length;

uint8_t midi_sysex_control_mssg[8] = 
{ 
	_MIDI_SYSEX_START, 
	_MIDI_ADJ_SYSEX_VENDOR_ID_0, 
	_MIDI_ADJ_SYSEX_VENDOR_ID_1,
	_MIDI_ADJ_SYSEX_VENDOR_ID_2,
	0,
	0,
	0,
	_MIDI_SYSEX_END
};

int MIDI_PARSER_add_byte(uint8_t new_byte)
{
	// debug_buf[debug_buf_idx++] = new_byte;
	
	if (state == _MIDI_PARSER_STATE_IDLE)
	{
		if (new_byte < 0x80)
		{
			// Not a command byte - wait for next byte
		}
		else
		{
			// A command byte found
			command_in_progress = new_byte;
			command_buffer_index = 2; // next byte
			command_buffer[1] = new_byte;
			// Get MIDI command length (0 if command starts with Fx)
			command_length = midi_commands_length[((command_in_progress - 0x80) >> 4)];
			
			if (command_length != 0) 
			{
				// Handle MIDI commands that have 2 or 3 bytes (0xf_ command len is marked as 0)
				state = _MIDI_PARSER_STATE_COMMAND_IN_PROGRESS;
			}
			else if (command_in_progress == 0xf0) 
			{
				// Sysex command start
				//	fprintf(stderr, "Sysex start\n");
				state = _MIDI_PARSER_STATE_SYSEX_IN_PROGRESS;
			}
			else if (command_in_progress == 0xff) 
			{
				// Handle Reset commands - it is a single byte command
				command_buffer[0] = 1;
				command_buffer[1] = command_buffer_index;
				
				state = _MIDI_PARSER_STATE_NEW_COMMAND_AVAILABLE;
				
			}
			else
			{
				// Handle Fx commands
				
				
			}			
		}
	}
	else if (state == _MIDI_PARSER_STATE_COMMAND_IN_PROGRESS)	
	{
		// MIDI command parssing in progress - get more command bytes
		if (new_byte < 0x80)
		{
			// command data bytes
			command_buffer[command_buffer_index++] = new_byte;
			command_length--;
			
			if (command_length <= 1)
			{
				// Last command byte
				command_buffer[0] = command_buffer_index - 1;				
				state = _MIDI_PARSER_STATE_NEW_COMMAND_AVAILABLE;
			}
		}
	}
	else if (state == _MIDI_PARSER_STATE_SYSEX_IN_PROGRESS)
	{
		command_buffer[command_buffer_index++] = new_byte;
		if (new_byte == 0xf7) 
		{
			// Last Sysex command byte
			command_buffer[0] = command_buffer_index - 1;				
			state = _MIDI_PARSER_STATE_NEW_COMMAND_AVAILABLE;
		}
	}
	else if (state == _MIDI_PARSER_STATE_NEW_COMMAND_AVAILABLE)
	{
		// To get here, make a dummy call to this function after a full command was parssed
		command_buffer_index = 1;
		state = _MIDI_PARSER_STATE_IDLE;
	}
	
	return state;
}

uint8_t* MIDI_PARSER_get_last_command()
{
	return &command_buffer[0];
}

/* Builds and returns a MIDI SYSEX message.
 *  Control id 0-127 
 *  Control value 0-0x3fff*/
uint8_t* MIDI_COMMAND_build_sysex_control_mssg(uint8_t control_id, uint16_t control_value)
{
	midi_sysex_control_mssg[4] = control_id & 0x7f;
	midi_sysex_control_mssg[5] = (control_value & 0x3f80) >> 7; // 7msbs
	midi_sysex_control_mssg[6] = control_value & 0x007f; // 7 lsbs
	
	return &midi_sysex_control_mssg[0];
}
