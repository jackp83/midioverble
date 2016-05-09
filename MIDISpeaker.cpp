/*
 * Midi.cpp
 *
 *  Created on: 29 apr 2016
 *      Author: jack
 */

#include "MIDISpeaker.h"


MIDISpeaker::MIDISpeaker() {

}

MIDISpeaker::~MIDISpeaker() {

}

/*
 * opens a midi out port
 */
void MIDISpeaker::open_midi_out(std::string type, int mode) {
	midiout = 0;
	if(snd_rawmidi_open(NULL,&midiout,type.c_str(),mode) < 0) {
		perror("Could not open port");
	}
}


void MIDISpeaker::write_to_out(char buf, int bufSize) {
	if(snd_rawmidi_write(midiout,&buf,bufSize) < 0) {
		perror("Could not write");
	}
}
