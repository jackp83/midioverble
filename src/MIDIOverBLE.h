/*
 * MIDIOverBLE.h
 *
 *  Created on: 9 maj 2016
 *      Author: andreas
 */

#ifndef MIDIOVERBLE_H_
#define MIDIOVERBLE_H_

#include "midi/MIDIListener.h"
#include "midi/MIDISpeaker.h"
#include "midi/MIDITools.h"
#include "ble/BLESender.h"
#include "ble/BLEReceiver.h"

class MIDIOverBLE {
private:
	BLESender blesender;
	BLEReceiver blereceiver;
	MIDIListener midilistener;
	MIDISpeaker midispeaker;
	MIDITools miditools;
	bool listen;
	bool central;
public:
	MIDIOverBLE();
	int connect(std::string bluetooth_address);
	int wait_for_connection();
	int open_ports(std::string type_in, int mode_in,std::string type_out, int mode_out);
	void listen_for_midi_messages();
	void listen_for_ble_messages();
	bool is_listening();
};

#endif /* MIDIOVERBLE_H_ */
