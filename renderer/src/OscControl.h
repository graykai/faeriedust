#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "constants.h"

class ofApp;

class OscControl {
public:
	OscControl() : values(nullptr), app(nullptr) { };
	~OscControl() = default;

	void setup(uint16_t port, ofApp *app);
	void update();

	void processMessage(const ofxOscMessage& message);

private:
	ofxOscReceiver rx;
	ControlValues* values;
	ofApp* app;
};