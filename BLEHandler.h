/*
 * BLEHandler.h
 *
 *  Created on: 9 maj 2016
 *      Author: jack
 */

#ifndef BLEHANDLER_H_
#define BLEHANDLER_H_

#include "../midi/MIDIHandler.h"
#include "BLEReceiver.h"
#include "BLESender.h"

class BLEHandler {
private:
	int socket_nr;
	BLEReceiver blereceiver;
	BLESender blesender;
public:
	int connect(std::string bluetooth_address);
	int listen_for_connection();
	int send(char* array,int array_size);
	int listen(char *buf,int size);
};

#endif /* BLEHANDLER_H_ */
