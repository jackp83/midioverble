/*
 * BluetoothLowEnergy.cpp
 *
 *  Created on: 29 apr 2016
 *      Author: Andreas
 */

#include "BLESender.h"

//Constructor
BLESender::BLESender() {

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
		if(connect_to_socket()==0){
			update_conn_param(0x0006,0x0006,0,200);
			return socket_number;
		}
	}
	return -1;
}

/*
 * Sends the array through the socket created in the connect_to_ble_device() function.
 * char* array - the array to send
 * int array_size - the size of array.
 * Returns 0 if successful and -1 if not successful.
 */
int BLESender::send_over_ble(char* array,int array_size) {
	if(write(socket_number, array, array_size)>0)
		return 0;
	close(socket_number);
	return -1;
}


void BLESender::set_socket_number(int socket_nr){
	socket_number = socket_nr;
}

/*
 *Updates the connection parameters for a BLE connection. Is used to determine when a central and a peripheral
 *in a link transmit data.
 *
 *The parameters are:
 *Connection Interval - with a minimum and maximum interval that determines how often a Central ask for data from the
 *peripheral. It must between 7.5 ms and 4 sec.
 *
 *Slave Latency - By setting a non-zero slave latency, the Peripheral can choose not to answer when the Central asks
 *for data up to the slave latency number of times. However, the Peripheral can send data if needed. This enables a
 *peripheral to stay sleeping for a longer time, if it doesn't have data to send.
 *
 *Connection Supervision Timeout - Determines the timeout from the last data exchange until a link is considered lost. A
 *Central will not start trying to reconnect before the timeout has passed
 *
 */
int BLESender::update_conn_param(int min_interval,int max_interval,int latency,int timeout) {
	int did = hci_get_route(NULL);
	int dd = hci_open_dev(did);
    return hci_le_conn_update(dd,32,min_interval,max_interval,latency,timeout,5000);//OBSERVERA!!!::HANDLE HÅRDKODAD
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
	sock = socket(AF_BLUETOOTH,SOCK_SEQPACKET,BTPROTO_L2CAP);//VAD ÄR DETTA???
	if (sock < 0) {
		perror("Failed to create L2CAP socket");
		return -1;
	}

	// Set up source address
	memset(&sourceaddress, 0, sizeof(sourceaddress));
	sourceaddress.l2_bdaddr_type = source_type;
	sourceaddress.l2_family = AF_BLUETOOTH;			//Bluetooth-Socket
	sourceaddress.l2_cid = htobs(4);
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
	destinationaddress.l2_family = AF_BLUETOOTH;	//Bluetooth-Socket
	destinationaddress.l2_cid = htobs(4);			//VAD ÄR DETTA ???
	bdaddr_t tmp;
	str2ba(destination_bluetooth_address.c_str(),&tmp);
	bacpy(&destinationaddress.l2_bdaddr, &tmp);

	return sock;
}
