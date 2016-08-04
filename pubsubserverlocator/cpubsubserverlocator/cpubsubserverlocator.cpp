#include "Particle.h"

SYSTEM_THREAD(ENABLED);

void locationSubscriptionHandler(const char *event, const char *data); // forward declaration

const unsigned long SERVER_ADDR_CHECK_RETRY_MS = 20000;

IPAddress serverAddr;
int serverPort;
bool haveServerAddr = false;
unsigned long serverAddrLastCheck = 0 - SERVER_ADDR_CHECK_RETRY_MS;


void setup() {
	Serial.begin(9600);
	Particle.subscribe("serverLocation", locationSubscriptionHandler, MY_DEVICES);
}

void loop() {
	if (!haveServerAddr && Particle.connected()) {
		if (millis() - serverAddrLastCheck >= SERVER_ADDR_CHECK_RETRY_MS) {
			Serial.println("requesting server location");

			serverAddrLastCheck = millis();
			Particle.publish("serverLocator", "", PRIVATE);
		}
	}
}

void locationSubscriptionHandler(const char *event, const char *data) {
	int addr[4], port;

	if (sscanf(data, "%u.%u.%u.%u:%u", &addr[0], &addr[1], &addr[2], &addr[3], &port) == 5) {
		serverAddr = IPAddress(addr[0], addr[1], addr[2], addr[3]);
		serverPort = port;
		haveServerAddr = true;

		Serial.printlnf("serverAddr=%s port=%d", serverAddr.toString().c_str(), serverPort);
	}
}
