/*
 * Midi.h
 *
 *  Created on: 29 apr 2016
 *      Author: jack
 */

#ifndef MIDISPEAKER_H_
#define MIDISPEAKER_H_

#include <alsa/asoundlib.h>
#include <sys/socket.h>
#include <string>

using namespace std;

class MIDISpeaker {
public:
	MIDISpeaker();
	virtual ~MIDISpeaker();
	int open_midi_out(std::string portname, int mode);
	int write_to_out(char buf, int bufSize);
	int close_port();
private:
	snd_rawmidi_t *midiout;
};

#endif /* MIDI_H_ */
