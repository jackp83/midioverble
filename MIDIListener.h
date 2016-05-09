/*
 * MIDIListener.h
 *
 *  Created on: 29 apr 2016
 *      Author: andreas
 */

#ifndef MIDILISTENER_H_
#define MIDILISTENER_H_

#include <alsa/asoundlib.h>
#include <sys/socket.h>
#include <string>

#include "../ble/BLESender.h"

/*
 * This class includes the public functions to open/close a
 * MIDI port and listen to it for incoming messages.
 */

const int MIDI_BUFFER_SIZE = 3;//Size of the buffer in witch raw MIDI will be written.

class MIDIListener {
private:
	BLESender blesender;
	snd_rawmidi_t* midiin;
	bool listen;
public:
	MIDIListener(BLESender _blesender);
	virtual ~MIDIListener();
	int open_port(int mode,std::string portname);
	int close_port();
	void listen_for_MIDI_messages();
};

#endif /* MIDILISTENER_H_ */
