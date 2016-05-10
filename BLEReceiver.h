#ifndef BLERECEIVER_H_
#define BLERECEIVER_H_

/*
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
*/
#include <alsa/asoundlib.h>

#include <string.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>



class BLEReceiver {
public:
	BLEReceiver();
	virtual ~BLEReceiver();
	int l2cap_le_listen_and_accept();
	int read_data(char *buf,int buf_size);
	void advertise_ble();
	void set_socket_number(int socket_nr);
private:
	int accept_socket;
};

#endif
