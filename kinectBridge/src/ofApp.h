#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxKinectForWindows2.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:
		ofxOscReceiver rx;
		ofxOscSender tx;
		ofxOscMessage lastMessage;

		void processMessage();
		void setupKinect();
		void setupOsc();
		ofRectangle processBody(const ofxKinectForWindows2::Data::Body& body);
		void applySeeking();
		void sendBoundingUpdate();

		ofxKFW2::Device kinect;

		uint64_t lastSendTime;
		ofRectangle currentBounding;
		ofRectangle targetBounding;
		bool bodyDetected;
		
};

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424