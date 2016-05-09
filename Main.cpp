//============================================================================
// Name        : BluetoothOverMIDI.cpp
// Author      : Andreas Ljungström & Jack Panikian
// Version     :
// Copyright   :
// DescripXtion :
//============================================================================

#include <iostream>
#include <string.h>
#include <thread>

#include "ble/BLESender.h"
#include "ble/BLEReceiver.h"
#include "midi/MIDIListener.h"
#include "midi/MIDISpeaker.h"


int run_client(std::string bluetooth_address);
int run_server();

int main(int argc, char **argv) {
	//run_client("24:FD:52:1A:AF:0D");
	run_server();
	return 0;
}

int run_client(std::string bluetooth_address){
	BLESender blesender;
	if(blesender.connect_to_remote_ble_device(bluetooth_address)<0){
		std::cout << "Could not connect to device";
		return -1;
	}
	MIDIListener midilistener (blesender);
	if(midilistener.open_port(0,"virtual")<0){
		std::cout << "Problem opening MIDI input";
		return -1;
	}
	midilistener.listen_for_MIDI_messages();
	return 0;
}

int run_server(){
	BLEReceiver ble;
	MIDISpeaker midi;
	MIDITools midit;

	ble.l2cap_le_listen_and_accept();
	midi.open_midi_out("virtual", 0);

	//ble.read_data(midi,midit);
	while(1) {
		std::thread t1(&BLEReceiver::read_data, &ble, midi, midit);
		t1.join();
	}

	return 0;
}
