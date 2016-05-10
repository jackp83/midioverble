/*
 * MIDIHandler.cpp
 *
 *  Created on: 9 maj 2016
 *      Author: jack
 */

#include "MIDIHandler.h"

MIDIHandler::MIDIHandler() {

}

MIDIHandler::~MIDIHandler() {
	// TODO Auto-generated destructor stub
}

int MIDIHandler::open_ports(std::string type, int mode) {
	if(midilistener.open_port(mode,type)==0 && midispeaker.open_midi_out(type,mode)==0)
		return 0;
	return -1;
}

int MIDIHandler::listen(char *buffer){
	if(midilistener.listen_for_MIDI_messages(buffer)==0)
		return 0;
	return -1;
}

int MIDIHandler::write(char c,int size){
	midispeaker.write_to_out(c,size);
	return 0;
}

int MIDIHandler::get_message_dataload_size_depending_on_first_byte(char c){
	return miditools.get_message_dataload_size_depending_on_first_byte(c);
}
