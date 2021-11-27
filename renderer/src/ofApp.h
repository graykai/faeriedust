#pragma once

#include "ofMain.h" 
#include "constants.h"
#include "ofxQuadtree.h"
#include "ofxGui.h"
#include "OscControl.h"
#include "boid.h"
#include "magicdust.h"
#include "../ModeSelector.h"

#pragma comment(lib, "userenv.lib")

//#define PRODUCTION

class ofApp : public ofBaseApp{

	public:
		void setup();
		void setupModes();
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

		void onTargetUpdate(float x, float y);
		void onTargetUpdate(float x, float y, float w, float h);
		void onOffsetUpdate(float x, float y);

		void onPause(bool state);

		bool calibrationMode = false;

private:
	std::vector<ofColor> palette;
	ofColor fieldColor;
	ofColor boidColor;
	ofColor neighborColor;
	float** field;
	int cols, rows, nRows, nCols;
	float xoff, yoff, zoff;
	float increment;
	bool paused, debug;
	ofFbo channel0, channel1, channel2;
	ofPixels output;
	ofColor jab_random(float j, float a, float b);
	ModeSelector selector;

	void initialize();
	void initializeColors(float j, float a, float b, int newSeed=0);
	glm::vec2 field_force_at(int col, int row, float speed);
	glm::ivec2 field_coord(float x, float y);

	boid new_boid_at(float x, float y, ofColor c);
	void draw_with(ofFbo& source, ofFbo& target, ofShader& shader, const glm::vec2& direction, float scale);
	void update_target_location();
	void renderTarget(bool filled=false);


	ofShader blur_shade;
	ofShader add_shade;

	std::unique_ptr<MagicDust> dust;
	ofPixels pixels;
	int seed;
	bool chan0, chan1;

	ofxPanel gui;
	ofParameterGroup constants;
	ofParameter<float> maxSpeed, maxChaos, maxForce, blurX, blurY, offsetX, offsetY;
	float depth_output_width;
	float depth_output_height;

	glm::vec2 debugOffset;

	OscControl rx;
};
