/*
 * MIDIOverBLE.cpp
 *
 *  Created on: 9 maj 2016
 *      Author: andreas
 */

#include <stdio.h>
#include "MIDIOverBLE.h"

MIDIOverBLE::MIDIOverBLE() {
	listen = false;
}

int MIDIOverBLE::connect(std::string bluetooth_address){
	int socket_nr = blesender.connect_to_remote_ble_device(bluetooth_address);
	if(socket_nr<0)
		return -1;
	listen = true;
	blereceiver.set_socket_number(socket_nr);
	return 0;
}

int MIDIOverBLE::wait_for_connection(){
	int socket_nr = blereceiver.l2cap_le_listen_and_accept();
	if(socket_nr<0)
		return -1;
	listen = true;
	blesender.set_socket_number(socket_nr);
	return 0;
}

int MIDIOverBLE::open_ports(std::string type_in, int mode_in,std::string type_out, int mode_out){
	if(midilistener.open_port(mode_in,type_in)<0)
		return -1;
	if(midispeaker.open_midi_out(type_out,mode_out)<0)
		return -1;
	return 0;
}

void MIDIOverBLE::listen_for_midi_messages(){
	char buffer[3];

	while(listen){
		if(midilistener.listen_for_MIDI_messages(buffer,sizeof(buffer))==0){
			//starta klocka
			blesender.send_over_ble(buffer,sizeof(buffer));//printfa
			//blereceiver.read_data(buf,sizeof(buf));//printfa
			//stoppa klocka
		}else{
			listen = false;
		}
	}
}

void MIDIOverBLE::listen_for_ble_messages(){
	char buf[3];
	int size;
	while(listen){
		if(blereceiver.read_data(buf,sizeof(buf))==0){
			size = miditools.get_message_dataload_size_depending_on_first_byte(buf[0]);
			if(size>=0)
				midispeaker.write_to_out(buf[0],1);
			if(size>=1)
				midispeaker.write_to_out(buf[1],1);
			if(size>=2)
				midispeaker.write_to_out(buf[2],1);
		}else{
			listen = false;
		}
	}
}

bool MIDIOverBLE::is_listening() {
	return listen;
}

/*
void MIDIOverBLE::start_listen(){
	listen = true;
}

void MIDIOverBLE::stop_listen(){
	listen = false;
}
*/
