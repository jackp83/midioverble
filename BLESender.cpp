/*
 * BluetoothLowEnergy.cpp
 *
 *  Created on: 29 apr 2016
 *      Author: Andreas
 */

#include <iostream>
#include "BLESender.h"

//Constructor
BLESender::BLESender() {
	socket_number = 1;
}

//Destructor
BLESender::~BLESender() {

}

/*
 * Connects to the device with destination_bluetooth_address using Bluetooth Low Energy.
 * BT_SECURITY_LOW is used as the security level.
 * Returns 0 if successful and -1 if not successful.
 */
int BLESender::connect_to_remote_ble_device(std::string destination_bluetooth_address) {

	socket_number = l2cap_le_socket(destination_bluetooth_address, BDADDR_LE_PUBLIC,BDADDR_LE_PUBLIC, BT_SECURITY_LOW);
	if(socket_number>0){
		if(connect_to_socket()==0)
			return socket_number;
	}
	return -1;

//	hci_le_create_conn()
//		uint16_t hci_handle;
//		bdaddr_t tmp;
//		str2ba(destination_bluetooth_address.c_str(),&tmp);
//		int x = hci_le_create_conn(hci_open_dev(hci_get_route(NULL)),htobs(0x0004), htobs(0x0004), 0,BDADDR_LE_PUBLIC, tmp, LE_PUBLIC_ADDRESS,htobs(0x0006) /*min_interval*/, htobs(0x0006) /*max_interval*/,htobs(0) /*latency*/,htobs(200) /*supervision_timeout*/,htobs(0x0001), htobs(0x0001), &hci_handle, 25000);
//		printf("%d\n",x);
//		return -1;
}

/*
 * Sends the array through the socket created in the connect_to_ble_device() function.
 * char* array - the array to send
 * int array_size - the size of array.
 * Returns 0 if successful and -1 if not successful.
 */
int BLESender::send_over_ble(char* array,int array_size) {
	printf("trying to send over ble\n");
	if(write(socket_number, array, array_size)>0) {
		printf("sent over ble\n");
		return 0;
	}
	return -1;
}


void BLESender::set_socket_number(int socket_nr){
	socket_number = socket_nr;
}

//============================================================================
// Private functions
//============================================================================

/*
 * Connects to the destinationaddress through socket.
 */
int BLESender::connect_to_socket() {
	if (connect(socket_number, (struct sockaddr *) &destinationaddress, sizeof(destinationaddress)) < 0) {
		perror(" Failed to connect");
		close(socket_number);
		return -1;
	}
	return 0;
}

/*
 * Sets up the socket to destination_bluetooth_address using L2CAP and returns the socket number.
 */
int BLESender::l2cap_le_socket(std::string destination_bluetooth_address, uint8_t destination_type, uint8_t source_type, int security_level) {

	struct sockaddr_l2 sourceaddress;
	struct bt_security bluetoothsecurity;

	int sock;
	sock = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);//VAD ÄR DETTA???
	if (sock < 0) {
		perror("Failed to create L2CAP socket");
		return -1;
	}

	// Set up source address
	memset(&sourceaddress, 0, sizeof(sourceaddress));
	sourceaddress.l2_bdaddr_type = source_type;
	sourceaddress.l2_family = AF_BLUETOOTH;			//VAD ÄR DETTA????
	sourceaddress.l2_cid = htobs(4);				//VAD ÄR DETTA????
	bdaddr_t tmp_bdaddr_any = {};//BDADDR_ANY
	bacpy(&sourceaddress.l2_bdaddr, &tmp_bdaddr_any/*NULL*/);

	if (bind(sock, (struct sockaddr *)&sourceaddress, sizeof(sourceaddress)) < 0) {
		perror("Failed to bind L2CAP socket");
		close(sock);
		return -1;
	}

	// Set the security level
	memset(&bluetoothsecurity, 0, sizeof(bluetoothsecurity));
	bluetoothsecurity.level = security_level;
	if (setsockopt(sock, SOL_BLUETOOTH, BT_SECURITY, &bluetoothsecurity,sizeof(bluetoothsecurity)) != 0) {
		fprintf(stderr, "Failed to set L2CAP security level\n");
		close(sock);
		return -1;
	}

	// Set up destination address
	memset(&destinationaddress, 0, sizeof(destinationaddress));
	destinationaddress.l2_bdaddr_type = destination_type;
	destinationaddress.l2_family = AF_BLUETOOTH;	//VAD ÄR DETTA ???
	destinationaddress.l2_cid = htobs(4);			//VAD ÄR DETTA ???
	bdaddr_t tmp;
	str2ba(destination_bluetooth_address.c_str(),&tmp);
	bacpy(&destinationaddress.l2_bdaddr, &tmp);

	fflush(stdout);

	return sock;
}
