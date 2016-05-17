/*
 * MIDITools.cpp
 *
 *  Created on: 4 maj 2016
 *      Author: andreas
 */

#include "MIDITools.h"

MIDITools::MIDITools() {}

MIDITools::~MIDITools() {}

int MIDITools::get_message_dataload_size_depending_on_first_byte(char message){

	std::bitset<8> message_in_bits(message);

	//Invalid Message
	if(message_in_bits.to_ulong()<128)
		return -1;

	//Channel Message
	if(message_in_bits.to_ulong()<240){
		std::bitset<8> message_first_4_bits(message_in_bits.to_string().substr(0,4));//change to <4>
		return get_channel_message_dataload(message_first_4_bits.to_ulong());
	}//System Message
	else{
		std::bitset<8> message_last_4_bits(message_in_bits.to_string().substr(4,4));//change to <4>
		return get_system_message_dataload(message_last_4_bits.to_ulong());
	}
	return -1;
}


//============================================================================
// Private functions
//============================================================================

int MIDITools::get_channel_message_dataload(int message_first_4_bits){
	switch(message_first_4_bits){
		case NOTE_OFF://1000	Note Off event.
			return 2; break;
		case NOTE_ON://1001	Note On event.
			return 2; break;
		case POLYPHONIC_KEY_PRESSURE://1010	Polyphonic Key Pressure (Aftertouch).
			return 2; break;
		case CONTROL_CHANGE://1011	Control Change.
			return 2; break;
		case PROGRAM_CHANGE://1100	Program Change.
			return 1; break;
		case CHANNEL_PRESSURE://1101	Channel Pressure (After-touch).
			return 1; break;
		case PITCH_BEND_CHANGE://1110	Pitch Bend Change.
			return 2; break;
	}
	return -1;
}

int MIDITools::get_system_message_dataload(int message_last_4_bits){
	switch(message_last_4_bits){
		case SYSTEM_EXCLUSIVE://0000 System Exclusive
			return -3; break;
		case MIDI_TIME_CODE://0001 MIDI Time Code Qtr. Frame
			return 1; break;
		case SONG_POSITION_POINTER://0010 Song Position Pointer
			return 2; break;
		case SONG_SELECT://0011 Song Select.
			return 1; break;
		case UNDEFINED_1://0100 Undefined. (Reserved)
			return -2; break;
		case UNDEFINED_2://0101 Undefined. (Reserved)
			return -2; break;
		case TUNE_REQUEST://0110 Tune Request.
			return 0; break;
		case END_OF_EXCLUSIVE://0111 End of Exclusive.
			return -3; break;
		case TIMING_CLOCK://1000 Timing Clock.
			return 0; break;
		case UNDEFINED_3://1001 Undefined. (Reserved)
			return -2; break;
		case START://1010 Start.
			return 0; break;
		case CONTINUE://1011 Continue.
			return 0; break;
		case STOP://1100 Stop.
			return 0; break;
		case UNDEFINED_4://1101 Undefined. (Reserved)
			return -2; break;
		case ACTIVE_SENSING://1110 Active Sensing.
			return 0; break;
		case RESET://1111 Reset.
			return 0; break;
	}
	return -1;
}
