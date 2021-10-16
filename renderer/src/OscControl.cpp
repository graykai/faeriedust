#include "OscControl.h"
#include "ofApp.h"

void OscControl::setup(uint16_t port, ofApp* app) {
	this->values = &ControlValues::instance();
	this->app = app;
	ofxOscReceiverSettings settings = {
		port, true, true
	};
	this->rx.setup(settings);
}

void OscControl::update() {
	ofxOscMessage message;
	while (this->rx.hasWaitingMessages()) {
		this->rx.getNextMessage(message);

		if (app) this->processMessage(message);
	}
}

void OscControl::processMessage(const ofxOscMessage& message) {
	const auto& addr = message.getAddress();

	if (addr == "/3/xy") {
		const float x = message.getArgAsFloat(0);
		const float y = message.getArgAsFloat(1);
		this->app->onTargetUpdate(x, y);
	}
	else if (addr == "/3/toggle1") {
		const float value = message.getArgAsFloat(0);
		this->app->onPause(value > 0.5);
	}
	else if (addr == "/body/target/1") {
		const float x = message.getArgAsFloat(0);
		const float y = message.getArgAsFloat(1);
		const float w = message.getArgAsFloat(2);
		const float h = message.getArgAsFloat(3);
		//ofLog() << "target 1: (" + ofToString(x) + "x" + ofToString(y) + "+" << ofToString(w) << "+" << ofToString(h) << ")";
		if ((x < 0 || y < 0 || w < 0.1 || h < 0.1)&&!this->app->calibrationMode) {
			// skip updating the target because shit got big (or small)
		}
		else {
			this->app->onTargetUpdate(x, y, w, h);
		}
	}
	else if (addr == "/offset") {
		const float x = message.getArgAsFloat(0);
		const float y = message.getArgAsFloat(1);
		this->app->onOffsetUpdate(x, y);
	}
	else if (addr == "/calibration/on") {
		this->app->calibrationMode = true;
	}
	else if (addr == "/calibration/off") {
		this->app->calibrationMode = false;
	}
	else {
		ofLog() << addr << ": " << message.getTypeString();
	}
}

#define screenScale(x, y, ox, oy) ofMap(x+ox, 0.0, 1.0, 0, ofGetWidth()),ofMap(y+oy, 0.0, 1.0, 0, ofGetHeight())

void ofApp::onTargetUpdate(float x, float y) {
	debugOffset.x = 1.0 - x;
	debugOffset.y = 1.0 - y;
	dust->set_target(screenScale(1.0-x, 1.0-y, offsetX, offsetY));
}

void ofApp::onTargetUpdate(float x, float y, float w, float h) {
	debugOffset.x = 1.0-x;
	debugOffset.y = 1.0 - y;
	dust->set_target(screenScale(1.0-x-w/2.0, 1.0-y-h/2.0, offsetX, offsetY), screenScale(w, h, 0, 0));
}

void ofApp::onPause(bool state) {
	paused = state;
}

void ofApp::onOffsetUpdate(float x, float y) {
	offsetX = x;
	offsetY = y;
}