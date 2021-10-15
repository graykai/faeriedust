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
		if (x < 0 || y < 0 || w < 0.1 || h < 0.1 ) {
			// skip updating the target because shit got big (or small)
		}
		else {
			this->app->onTargetUpdate(x, y, w, h);
		}
	}
	else {
		ofLog() << addr << ": " << message.getTypeString();
	}
}

#define screenScale(x, y) ofMap(x, 0.0, 1.0, 0, ofGetWidth()),ofMap(y, 0.0, 1.0, 0, ofGetHeight())

void ofApp::onTargetUpdate(float x, float y) {
	dust->set_target(screenScale(x, y));
}

void ofApp::onTargetUpdate(float x, float y, float w, float h) {
	dust->set_target(screenScale(x, y), screenScale(w, h));
}

void ofApp::onPause(bool state) {
	paused = state;
}