/*
 * BLEHandler.cpp
 *
 *  Created on: 9 maj 2016
 *      Author: jack
 */

#include "BLEHandler.h"

int BLEHandler::connect(std::string bluetooth_address){
	socket_nr = blesender.connect_to_remote_ble_device(bluetooth_address);
	if(socket_nr<0){
		return -1;
	}
	blereceiver.set_socket_number(socket_nr);
	return 0;
}

int BLEHandler::listen_for_connection(){
	socket_nr = blereceiver.l2cap_le_listen_and_accept();
	if(socket_nr<0){
		return -1;
	}
	blesender.set_socket_number(socket_nr);
	return 0;
}

int BLEHandler::send(char* array,int array_size){
	return blesender.send_over_ble(array,array_size);
}

int BLEHandler::listen(char *buf,int size){
	return blereceiver.read_data(buf,size);
}
