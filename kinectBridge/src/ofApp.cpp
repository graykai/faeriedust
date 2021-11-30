#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_SCREEN);
	setupOsc();
	setupKinect();
	offset = { 0., 0. };
	calibrationMode = false;

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
#define depthScale(x, y) ofMap(x, 0.0, 1.0, 0.0, DEPTH_WIDTH*2), ofMap(y, 0.0, 1.0, 0.0, DEPTH_HEIGHT*2)

void ofApp::sendBoundingUpdate() {
	if (!bodyDetected && !calibrationMode) {
		return;
	}
	uint64_t now = ofGetElapsedTimeMillis();
	if (now - lastSendTime < 100) {
		//return;
	}

	targetBounding.x = worldScale(currentBounding.x, DEPTH_WIDTH);
	targetBounding.y = worldScale(currentBounding.y, DEPTH_HEIGHT);
	targetBounding.width = worldScale(currentBounding.getWidth(), DEPTH_WIDTH);
	targetBounding.height = worldScale(currentBounding.getHeight(), DEPTH_HEIGHT);

	lastSendTime = now;
	ofxOscMessage message;
	message.setAddress("/body/target/1");
	message.addFloatArg(targetBounding.x);
	message.addFloatArg(targetBounding.y);
	message.addFloatArg(targetBounding.width);
	message.addFloatArg(targetBounding.height);
	tx.sendMessage(message);
}

void ofApp::processMessage() {
	ofxOscMessage msg;
	bool send = false;
	if (this->rx.getNextMessage(this->lastMessage)) {
		// We have a new message from OSC
		const auto& addr = this->lastMessage.getAddress();
		if (addr == "/3/xy") {
			float x = this->lastMessage.getArgAsFloat(0) * 0.5 - 0.25;
			float y = this->lastMessage.getArgAsFloat(1) * 0.5 - 0.25;

			offset.x = x;
			offset.y = y;
			msg.setAddress("/offset");
			msg.addFloatArg(x);
			msg.addFloatArg(y);
			send = true;
		} else if (addr == "/3/toggle1") {
			bool value = this->lastMessage.getArgAsFloat(0) > 0.5;
			if (value) {
				msg.setAddress("/calibration/on");
				calibrationMode = true;
			}
			else {
				msg.setAddress("/calibration/off");
				calibrationMode = false;
			}
			send = true;
		}
		else {
			ofLog() << this->lastMessage.getAddress() << ": " << this->lastMessage.getTypeString();
		}

		if (send) this->tx.sendMessage(msg);
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
		ofDisableBlendMode();
		ofSetColor(0);
		ofDrawRectangle(0, 0, DEPTH_WIDTH * 2, DEPTH_HEIGHT * 4);
		ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_SCREEN);
		ofSetColor(255, 128);
		kinect.getDepthSource()->draw(0, 0, DEPTH_WIDTH * 2, DEPTH_HEIGHT * 2);
		kinect.getDepthSource()->draw(0, 0, DEPTH_WIDTH*2, DEPTH_HEIGHT*2);
		ofDisableBlendMode();
		ofSetColor(255);
		kinect.getBodyIndexSource()->draw(0, DEPTH_HEIGHT*2, DEPTH_WIDTH*2, DEPTH_HEIGHT*2);
		kinect.getBodySource()->drawProjected(0, DEPTH_HEIGHT*2, DEPTH_WIDTH*2, DEPTH_HEIGHT*2, ofxKinectForWindows2::DepthCamera);

		if (bodyDetected)
		{
			ofPushStyle();
			ofPushMatrix();
			ofTranslate(0, DEPTH_HEIGHT*2);
			ofNoFill();
			ofSetLineWidth(3);
			ofSetColor(ofColor::darkMagenta);
			ofDrawRectangle(currentBounding);

			ofSetColor(ofColor::orangeRed);
			ofDrawRectangle(depthScale(targetBounding.x + offset.x, targetBounding.y + offset.y), depthScale(targetBounding.width, targetBounding.height));

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
