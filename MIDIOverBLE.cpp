/*
 * MIDIOverBLE.cpp
 *
 *  Created on: 9 maj 2016
 *      Author: andreas
 */

#include <stdio.h>
#include "MIDIOverBLE.h"

MIDIOverBLE::MIDIOverBLE() {

}

MIDIOverBLE::~MIDIOverBLE() {
	// TODO Auto-generated destructor stub
}

int MIDIOverBLE::connect(std::string bluetooth_address){
	return blehandler.connect(bluetooth_address);
}

int MIDIOverBLE::wait_for_connection(){
	return blehandler.listen_for_connection();
}

int MIDIOverBLE::open_ports(std::string type, int mode){
	return midihandler.open_ports(type,mode);
}

void MIDIOverBLE::listen_for_midi_messages(){
	char buffer[3];
	while(1){
		if(midihandler.listen(buffer)==0){
			blehandler.send(buffer,sizeof(buffer));
			printf("%x  ", buffer[0]);
			printf("%x  ", buffer[1]);
			printf("%x\n", buffer[2]);
		}
	}
}

void MIDIOverBLE::listen_for_ble_messages(){
	char buf[3];
	int size;
	while(1){
		if(blehandler.listen(buf,sizeof(buf))==0){
			size = midihandler.get_message_dataload_size_depending_on_first_byte(buf[0]);
			if(size>=0) {
				midihandler.write(buf[0], 1);
				printf("%x  ", buf[0]);
			}
			if(size>=1) {
				midihandler.write(buf[1],1);
				printf("%x  ", buf[1]);
			}
			if(size>=2) {
				midihandler.write(buf[2],1);
				printf("%x  ", buf[2]);
			}
		}
		printf("\n");
	}
}
