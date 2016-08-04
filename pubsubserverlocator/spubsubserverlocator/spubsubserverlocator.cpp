#include "Particle.h"

SYSTEM_THREAD(ENABLED);

const int SERVER_PORT = 7000;


void locatorSubscriptionHandler(const char *event, const char *data); // forward declaration


void setup() {
	Serial.begin(9600);
	Particle.subscribe("serverLocator", locatorSubscriptionHandler, MY_DEVICES);
}

void loop() {
}


void locatorSubscriptionHandler(const char *event, const char *data) {
	char buf[64];

	snprintf(buf, sizeof(buf), "%s:%d", WiFi.localIP().toString().c_str(), SERVER_PORT);
	Particle.publish("serverLocation", buf, PRIVATE);

	Serial.printlnf("locatorSubscriptionHandler returned %s", buf);
}
