#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

class ofApp;

using Setter = std::function<void()>;

class ModeSelector : public ofxMidiListener {
public:
	void setup();
	void update();
	void addMode(int mode, Setter fnc);
	void newMidiMessage(ofxMidiMessage& msg);

protected:
	ofxMidiIn midi;
	int mode = 0;
	bool changed = false;
	std::unordered_map<int, Setter> activators;
};