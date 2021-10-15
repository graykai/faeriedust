#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	setupOsc();
	setupKinect();

	ofSetBackgroundAuto(false);
	lastSendTime = ofGetElapsedTimeMillis();
}

void ofApp::setupOsc() {
	ofxOscReceiverSettings rxSettings{ 9000, true, true };
	ofxOscSenderSettings txSettings{ "localhost", 8000, true };

	this->tx.setup(txSettings);
	this->rx.setup(rxSettings);
}

void ofApp::setupKinect() {
	kinect.open();
	kinect.initDepthSource();
	kinect.initBodyIndexSource();
	kinect.initBodySource();

	bodyDetected = false;
}
//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
	while (this->rx.hasWaitingMessages()) this->processMessage();

	if (kinect.isFrameNew()) {
		bodyDetected = false;
		auto& bodies = kinect.getBodySource()->getBodies();
		for (auto& body : bodies) {
			if (body.tracked) {
				bodyDetected = true;
				currentBounding = processBody(body);
			}
		}
	}

	sendBoundingUpdate();
}

void ofApp::applySeeking() {

}

#define worldScale(v, m) ofMap(v, 0.0, m, 0.0, 1.0)

void ofApp::sendBoundingUpdate() {
	if (!bodyDetected) {
		return;
	}
	uint64_t now = ofGetElapsedTimeMillis();
	if (now - lastSendTime < 100) {
		//return;
	}

	lastSendTime = now;
	ofLog() << "Sending Location";
	ofxOscMessage message;
	message.setAddress("/body/target/1");
	message.addFloatArg(worldScale(currentBounding.x, DEPTH_WIDTH));
	message.addFloatArg(worldScale(currentBounding.y, DEPTH_HEIGHT));
	message.addFloatArg(worldScale(currentBounding.getWidth(), DEPTH_WIDTH));
	message.addFloatArg(worldScale(currentBounding.getHeight(), DEPTH_HEIGHT));
	tx.sendMessage(message);
}

void ofApp::processMessage() {
	if (this->rx.getNextMessage(this->lastMessage)) {
		// We have a new message from OSC
		ofLog() << this->lastMessage.getAddress() << ": " << this->lastMessage.getTypeString();
	}	
}

ofRectangle ofApp::processBody(const ofxKinectForWindows2::Data::Body& body) {
	glm::vec4 bounds = { ofGetWidth(), ofGetHeight(), 0, 0 };
	for (auto& joint : body.joints) {
		auto& pos = joint.second.getPositionInDepthMap();
		if (pos.x < bounds.x) {
			bounds.x = pos.x;
		}
		else if (pos.x > bounds.z) {
			bounds.z = pos.x;
		}

		if (pos.y < bounds.y) {
			bounds.y = pos.y;
		}
		else if (pos.y > bounds.w) {
			bounds.w = pos.y;
		}
	}

	return ofRectangle(bounds.x, bounds.y, bounds.z - bounds.x, bounds.w - bounds.y);
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (kinect.isFrameNew()) {
		kinect.getDepthSource()->draw(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT);
		kinect.getBodyIndexSource()->draw(0, DEPTH_HEIGHT, DEPTH_WIDTH, DEPTH_HEIGHT);
		kinect.getBodySource()->drawProjected(0, DEPTH_HEIGHT, DEPTH_WIDTH, DEPTH_HEIGHT, ofxKinectForWindows2::DepthCamera);

		if (bodyDetected)
		{
			ofPushStyle();
			ofPushMatrix();
			ofTranslate(0, DEPTH_HEIGHT);
			ofNoFill();
			ofSetLineWidth(3);
			ofSetColor(ofColor::red);
			ofDrawRectangle(targetBounding);
			ofSetColor(ofColor::darkMagenta);
			ofDrawRectangle(currentBounding);

			ofPopMatrix();
			ofPopStyle();
		}

	}
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
