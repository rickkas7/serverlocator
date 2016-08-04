#include "Particle.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

const int SERVER_PORT = 7000;
const int MCAST_PORT = 7000;
const int UDP_BUFFER_SIZE = 128;

bool firstTimeReady = true;
UDP mcastUdp;
IPAddress mcastAddr(239, 0, 0, 70);
char udpBuffer[UDP_BUFFER_SIZE];

void setup() {
	Serial.begin(9600);

	// Only enable WiFi, not cloud access for this test
	WiFi.on();
	WiFi.connect();

}

void loop() {
	if (WiFi.ready()) {
		if (firstTimeReady) {
			firstTimeReady = false;
			Serial.println("wifi ready");

			mcastUdp.begin(MCAST_PORT);
			mcastUdp.joinMulticast(mcastAddr);
		}
		else {
			int count = mcastUdp.receivePacket(udpBuffer, UDP_BUFFER_SIZE - 1);
			if (count > 0) {
				// Respond to the multicast request with a unicast response
				snprintf(udpBuffer, UDP_BUFFER_SIZE, "%s:%d", WiFi.localIP().toString().c_str(), SERVER_PORT);
				mcastUdp.sendPacket(udpBuffer, strlen(udpBuffer), mcastUdp.remoteIP(), mcastUdp.remotePort());

				Serial.printlnf("received server request, returning %s", udpBuffer);
			}

		}
	}
	else {
		// If WiFi disconnects, we need to reestablish listeners next time
		firstTimeReady = true;
	}
}
