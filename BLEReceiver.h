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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>

#include "../midi/MIDISpeaker.h"
#include "../midi/MIDITools.h"


class BLEReceiver {
public:
	BLEReceiver();
	virtual ~BLEReceiver();
	int l2cap_le_listen_and_accept();
	int read_data(MIDISpeaker obj,MIDITools midit);
	void advertise_ble();
private:
	int accept_socket;
};

#endif
