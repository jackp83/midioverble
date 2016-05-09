/*
 * Midi.h
 *
 *  Created on: 29 apr 2016
 *      Author: jack
 */

#ifndef MIDISPEAKER_H_
#define MIDISPEAKER_H_

#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <alsa/asoundlib.h>

#include <string>

#include <string.h>
#include <sys/socket.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>

using namespace std;

class MIDISpeaker {
public:
	MIDISpeaker();
	virtual ~MIDISpeaker();
	void open_midi_out(std::string type, int mode);
	void write_to_out(char buf, int bufSize);
private:
	snd_rawmidi_t *midiout;
};

#endif /* MIDI_H_ */
