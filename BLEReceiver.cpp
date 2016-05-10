/*
 * BLEReceiver.cpp
 *
 *  Created on: 29 apr 2016
 *      Author: jack
 */

#include "BLEReceiver.h"

BLEReceiver::BLEReceiver() {
	accept_socket = NULL;
}

BLEReceiver::~BLEReceiver() {

}

/*
 * Sets up a Bluetooth Low Energy socket and security level
 * -returns the listening socket
 */
int BLEReceiver::l2cap_le_listen_and_accept() {
	int listen_socket;
	char remote_address[18];
	socklen_t optlen;
	struct sockaddr_l2 srcaddr, addr;
	struct bt_security btsec;
	bdaddr_t tmp_bdaddr_any = {};
	listen_socket = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	if (listen_socket < 0) {
		perror("Failed to create L2CAP socket");
		return -1;
	}

	/* Set up source address */
	memset(&srcaddr, 0, sizeof(srcaddr));
	srcaddr.l2_family = AF_BLUETOOTH;
	srcaddr.l2_cid = htobs(4);
	srcaddr.l2_bdaddr_type = BDADDR_LE_PUBLIC;
	bacpy(&srcaddr.l2_bdaddr, &tmp_bdaddr_any);

	if (bind(listen_socket, (struct sockaddr *) &srcaddr, sizeof(srcaddr)) < 0) {
		perror("Failed to bind L2CAP socket");
		close(listen_socket);
		return -1;
	}

	/* Set the security level */
	memset(&btsec, 0, sizeof(btsec));
	btsec.level = BT_SECURITY_LOW;
	if (setsockopt(listen_socket, SOL_BLUETOOTH, BT_SECURITY, &btsec,
							sizeof(btsec)) != 0) {
		fprintf(stderr, "Failed to set L2CAP security level\n");
		close(listen_socket);
		return -1;
	}

	if (listen(listen_socket, 10) < 0) {
		perror("Listening on socket failed");
		close(listen_socket);
		return -1;
	}

	printf("Waiting for connections\n");

	memset(&addr, 0, sizeof(addr));
	optlen = sizeof(addr);
	accept_socket = accept(listen_socket, (struct sockaddr *) &addr, &optlen);

	if (accept_socket < 0) {
		perror("Accept failed");
		close(listen_socket);
		return -1;
	}

	ba2str(&addr.l2_bdaddr, remote_address);
	printf("Connect from %s\n", remote_address);
	close(listen_socket);
	return accept_socket;
}

/*
 * Reads data from a remote socket connection and writes it to midi out
 * Takes a Midi and MIDITools object as arguments
 */
int BLEReceiver::read_data(char *buf,int buf_size) {
	int bytes_read;
	int size;
	printf("Read data\n");
	bytes_read = read(accept_socket, buf, buf_size);

	if( bytes_read < 0 ) {
		perror("Could not read data");
		return -1;
	}else{
		printf("MESSAGE RECEIVED OVER BLE\n");
		return 0;
	}
}

void BLEReceiver::advertise_ble() {
	int dev_id = 0;
	int hci_socket;

	dev_id = hci_get_route(NULL);
	hci_socket = hci_open_dev(dev_id);

	if(dev_id < 0) {
		perror("Could not open bluetooth device: ");
	}

	hci_le_set_advertise_enable(hci_socket, 0, 1000);
}

void BLEReceiver::set_socket_number(int socket_nr){
	accept_socket= socket_nr;
}

/*
 * Returns true if Bluetooth Low Energy connection exists
 */
