/*
 * MIDIListener.cpp
 *
 *  Created on: 29 apr 2016
 *      Author: andreas
 */

#include <iostream>

#include "MIDIListener.h"

MIDIListener::MIDIListener() {
	midiin = NULL;
}

/*
 * Calls the close_port() function
 */
MIDIListener::~MIDIListener() {
	close_port();
}

/*
 * Open MIDI-In port on portname with the mode mode.
 */
int MIDIListener::open_port(int mode,std::string portname){
	if ((snd_rawmidi_open(&midiin, 0, portname.c_str(), mode)) < 0)
		return -1;
	return 0;
}

/*
 * Stops the listen_for_MIDI_messages function and closes the port.
 */
int MIDIListener::close_port(){
	int ret = snd_rawmidi_close(midiin);
	midiin  = NULL;
	return ret;
}

/*
 * Listens for MIDI messages on the port opened on open_port.
 */
int MIDIListener::listen_for_MIDI_messages(char *buffer,int arraysize){
	if(snd_rawmidi_read(midiin, buffer, arraysize)>=0){
		snd_rawmidi_drain(midiin);
		fflush(stdout);
		return 0;
	}else{
		snd_rawmidi_drain(midiin);
		fflush(stdout);
		return 1;
	}
}
