/*
 * MIDIOverBLE.h
 *
 *  Created on: 9 maj 2016
 *      Author: andreas
 */

#ifndef MIDIOVERBLE_H_
#define MIDIOVERBLE_H_

#include "midi/MIDIHandler.h"
#include "ble/BLEHandler.h"

class MIDIOverBLE {
private:
	BLEHandler blehandler;
	MIDIHandler midihandler;
public:
	MIDIOverBLE();
	virtual ~MIDIOverBLE();
	int connect(std::string bluetooth_address);
	int wait_for_connection();
	int open_ports(std::string type, int mode);
	void listen_for_midi_messages();
	void listen_for_ble_messages();
};

#endif /* MIDIOVERBLE_H_ */
