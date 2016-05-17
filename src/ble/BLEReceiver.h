#ifndef BLERECEIVER_H_
#define BLERECEIVER_H_


#include <iostream>
#include <string>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <unistd.h>
//#include <bluetooth/uuid.h>

/*
 * Includes functions for setting up a BLE Central connection and for reading incoming data
 */

#define MIDI_UUID "03B80E5A-EDE8-4B33-A751-6CE34EC4C700"
class BLEReceiver {
public:
	BLEReceiver();
	virtual ~BLEReceiver();
	int l2cap_le_listen_and_accept();
	int read_data(char *buf,int buf_size);
	void set_socket_number(int socket_nr);
private:
	int accept_socket;
};

#endif
