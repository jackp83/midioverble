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

#include "MIDIOverBLE.h"

int main(int argc, char **argv) {
	MIDIOverBLE mob;
	mob.open_ports("virtual",0);
	printf("aaaaa\n");
	//if(mob.connect("24:FD:52:25:9B:56")==0){	//CLIENT
	if(mob.wait_for_connection()==0){//SERVER
		std::thread t1(&MIDIOverBLE::listen_for_midi_messages, &mob);
		std::thread t2(&MIDIOverBLE::listen_for_ble_messages, &mob);
		if(t1.joinable())
			t1.join();
		if(t2.joinable())
			t2.join();
	}
	return 0;
}
