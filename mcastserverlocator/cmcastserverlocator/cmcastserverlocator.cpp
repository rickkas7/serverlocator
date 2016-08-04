#include "Particle.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

const int MCAST_PORT = 7000;
const int UDP_BUFFER_SIZE = 128;
const unsigned long SERVER_ADDR_CHECK_RETRY_MS = 20000;

bool firstTimeReady = true;
UDP mcastUdp;
IPAddress mcastAddr(239, 0, 0, 70);
char udpBuffer[UDP_BUFFER_SIZE];

IPAddress serverAddr;
int serverPort;
bool haveServerAddr = false;
unsigned long serverAddrLastCheck = 0 - SERVER_ADDR_CHECK_RETRY_MS;


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

			// First time we get WiFi.ready(), initialize the UDP listener. This happens
			// every time we reconnect to WiFi.
			mcastUdp.begin(MCAST_PORT);
		}

		if (!haveServerAddr && millis() - serverAddrLastCheck >= SERVER_ADDR_CHECK_RETRY_MS) {
			serverAddrLastCheck = millis();

			// If we don't have a server IP address, request it periodically (every 20 seconds)
			Serial.printlnf("requesting server address at %s:%d", mcastAddr.toString().c_str(), MCAST_PORT);

			strcpy(udpBuffer, "\n");
			mcastUdp.sendPacket(udpBuffer, strlen(udpBuffer), mcastAddr, MCAST_PORT);
		}

		int count = mcastUdp.receivePacket(udpBuffer, UDP_BUFFER_SIZE - 1);
		if (count > 0) {
			udpBuffer[count] = 0;

			// Handle a received UDP packet from the server with the IP address and port of the server
			int addr[4], port;

			if (sscanf(udpBuffer, "%u.%u.%u.%u:%u", &addr[0], &addr[1], &addr[2], &addr[3], &port) == 5) {
				serverAddr = IPAddress(addr[0], addr[1], addr[2], addr[3]);
				serverPort = port;
				haveServerAddr = true;

				Serial.printlnf("serverAddr=%s port=%d", serverAddr.toString().c_str(), serverPort);
			}
		}

	}
	else {
		// If WiFi disconnects, we need to reestablish listeners next time
		firstTimeReady = true;
	}
}
