//============================================================================
// Name        : BluetoothOverMIDI.cpp
// Author      : Andreas Ljungström & Jack Panikian
// Version     :
// Copyright   :
// DescripXtion : Run the program as root!!!
//============================================================================

//Jacks Bluetooth address 24:FD:52:1A:AF:0D
//Andreas Bluetooth address 24:FD:52:25:9B:56

#include <iostream>
#include <string.h>
#include <thread>

#include "MIDIOverBLE.h"
#include "Test.h"

void run_central() {
	MIDIOverBLE mob;
	mob.open_ports("virtual",0,"virtual",0);

	if(mob.wait_for_connection()==0){
		std::thread t1(&MIDIOverBLE::listen_for_midi_messages, &mob);
		std::thread t2(&MIDIOverBLE::listen_for_ble_messages, &mob);
		if(t1.joinable())
			t1.join();
		if(t2.joinable())
			t2.join();

	}
}

void run_peripheral(std::string bluetooth_address) {
	MIDIOverBLE mob;
	mob.open_ports("virtual",0,"virtual",0);

	if(mob.connect(bluetooth_address)==0){
		std::thread t1(&MIDIOverBLE::listen_for_midi_messages, &mob);
		std::thread t2(&MIDIOverBLE::listen_for_ble_messages, &mob);
		if(t1.joinable())
			t1.join();
		if(t2.joinable())
			t2.join();
	}
}

int run_tests_as_central(){
	Test test;
	return test.run_tests();
}

int run_tests_as_peripheral(std::string bluetooth_address){
	Test test;
	return test.run_tests(bluetooth_address,1000,0,6,6,6,6,0,0,0x000A,0x000A);
}



int main(int argc, char **argv) {
	if (argc > 1){
		run_peripheral(argv[1]);//RUN AS ROOT!!!!
	}else{
		run_central();
	}

	//run_tests_as_peripheral("24:FD:52:1A:AF:0D");
	//or
	//run_tests_as_central();

	return 0;
}
