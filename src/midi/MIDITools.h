/*
 * MIDITools.h
 *
 *  Created on: 4 maj 2016
 *      Author: andreas
 */

#ifndef MIDITOOLS_H_
#define MIDITOOLS_H_

#include <string>
#include <bitset>

const int NOTE_OFF = 8;
const int NOTE_ON = 9;
const int POLYPHONIC_KEY_PRESSURE = 10;
const int CONTROL_CHANGE = 11;
const int PROGRAM_CHANGE = 12;
const int CHANNEL_PRESSURE = 13;
const int PITCH_BEND_CHANGE = 14;

const int SYSTEM_EXCLUSIVE = 0;
const int MIDI_TIME_CODE = 1;
const int SONG_POSITION_POINTER = 2;
const int SONG_SELECT = 3;
const int UNDEFINED_1 = 4;
const int UNDEFINED_2 = 5;
const int UNDEFINED_3 = 9;
const int UNDEFINED_4 = 13;
const int TUNE_REQUEST = 6;
const int END_OF_EXCLUSIVE = 7;
const int TIMING_CLOCK = 8;
const int START = 10;
const int CONTINUE = 11;
const int STOP = 12;
const int ACTIVE_SENSING = 14;
const int RESET = 15;

class MIDITools {
private:
	int get_channel_message_dataload(int message_first_4_bits);
	int get_system_message_dataload(int message_last_4_bits);
public:
	MIDITools();
	virtual ~MIDITools();
	int get_message_dataload_size_depending_on_first_byte(char message);
};

#endif /* MIDITOOLS_H_ */
