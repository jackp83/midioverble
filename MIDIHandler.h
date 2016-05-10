/*
 * MIDIHandler.h
 *
 *  Created on: 9 maj 2016
 *      Author: jack
 */

#ifndef MIDIHANDLER_H_
#define MIDIHANDLER_H_

#include <string>

#include "../ble/BLEHandler.h"
#include "MIDIListener.h"
#include "MIDISpeaker.h"
#include "MIDITools.h"

class MIDIHandler {
private:
	MIDIListener midilistener;
	MIDISpeaker midispeaker;
	MIDITools miditools;
public:
	MIDIHandler();
	virtual ~MIDIHandler();
	int open_ports(std::string type, int mode);
	int listen(char *buffer);
	int write(char c,int size);
	int get_message_dataload_size_depending_on_first_byte(char c);
};

#endif /* MIDIHANDLER_H_ */
