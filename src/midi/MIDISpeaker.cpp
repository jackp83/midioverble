/*
 * Midi.cpp
 *
 *  Created on: 29 apr 2016
 *      Author: jack
 */

#include "MIDISpeaker.h"


MIDISpeaker::MIDISpeaker() {
	midiout = NULL;
}

MIDISpeaker::~MIDISpeaker() {
	close_port();
}

/*
 * Opens a MIDI-out port on portname with the specified mode
 */
int MIDISpeaker::open_midi_out(std::string portname, int mode) {
	midiout = 0;
	if(snd_rawmidi_open(NULL,&midiout,portname.c_str(),mode) < 0) {
		perror("Could not open port out: ");
		return -1;
	}
	return 0;
}

int MIDISpeaker::close_port(){
	int ret = snd_rawmidi_close(midiout);
	midiout  = NULL;
	return ret;
}

int MIDISpeaker::write_to_out(char buf, int bufSize) {
	if(snd_rawmidi_write(midiout,&buf,bufSize) < 0) {
		perror("Could not write to MIDI out: ");
		return -1;
	}
	return 0;
}
