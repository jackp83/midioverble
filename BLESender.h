/*
 * BluetoothLowEnergySender.h
 *
 *  Created on: 29 apr 2016
 *      Author: Andreas
 */

#ifndef BLESENDER_H_
#define BLESENDER_H_

#include <string>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <unistd.h>

/*
 * This class includes the public functions to connect to a BLE device
 * and to send to the BLE device.
 */
class BLESender {
private:
	int socket_number;
	struct sockaddr_l2 destinationaddress;
	int connect_to_socket();
	int l2cap_le_socket(std::string destination_bluetooth_address, uint8_t destination_type, uint8_t source_type, int security_level);
public:
	BLESender();
	virtual ~BLESender();
	int connect_to_remote_ble_device(std::string destination_bluetooth_address);
	int send_over_ble(char* array,int array_size);
	void set_socket_number(int socket_nr);
};

#endif /* BLESENDER_H_ */
