#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	mode = 0;
	midi.setVerbose(true);
	midi.openPort(0);
	midi.addListener(this);
	ofBackground(ofColor::whiteSmoke);
	colors.push_back(ofColor::aliceBlue);
	colors.push_back(ofColor::darkGoldenRod);
	colors.push_back(ofColor::darkSalmon);
	colors.push_back(ofColor::cadetBlue);
	colors.push_back(ofColor::black);
	colors.push_back(ofColor::orangeRed);
	colors.push_back(ofColor::darkSlateBlue);
	colors.push_back(ofColor::darkMagenta);
}

//--------------------------------------------`------------------
void ofApp::update(){
	if (buttons.size() > 0) {
		midiLock.lock();
		for (auto& newMode : buttons) {
			mode = newMode;
		}
		buttons.clear();
		midiLock.unlock();
	}
}

void ofApp::newMidiMessage(ofxMidiMessage& msg) {
	midiLock.lock();
	if (msg.status == MIDI_NOTE_ON)
		if (msg.channel == 10)
			buttons.push_back(msg.pitch);
	midiLock.unlock();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(colors.at(mode));
	ofDrawCircle(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 50.0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
