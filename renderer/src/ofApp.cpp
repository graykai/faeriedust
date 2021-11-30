#include "ofApp.h"
#include "JzAzBzColor.h"

//#define RENDER_FLOWS
#define RENDER_MAGIC
//#define RENDER_DEPTH

//--------------------------------------------------------------
void ofApp::setup() {

#ifdef PRODUCTION
	ofSetFullscreen(true);
#endif
	ofSetFrameRate(60.);
	ofEnableAlphaBlending();
	ofEnableAntiAliasing();
	ofDisableArbTex();

	rx.setup(RX_PORT, this);

	gui.setup();
	auto& controls = ControlValues::instance();
	gui.add(maxChaos.set("Max Chaos", controls.maxChaos, 0, 2));
	gui.add(maxSpeed.set("Max Speed", controls.maxSpeed, 0.1, 20));
	gui.add(maxForce.set("Max Force", controls.maxForce, 0.1, 5));
	gui.add(blurX.set("Blur 1", controls.blurControl.x, 1, 10));
	gui.add(blurY.set("Blur 2", controls.blurControl.y, 0, 2));
	gui.add(offsetX.set("Offset X", controls.offset.x, -1, 1));
	gui.add(offsetY.set("Offset Y", controls.offset.y, -1, 1));

	maxChaos.addListener(&controls, &ControlValues::chaosChanged);
	maxSpeed.addListener(&controls, &ControlValues::speedChanged);
	maxForce.addListener(&controls, &ControlValues::forceChanged);
	blurX.addListener(&controls, &ControlValues::blurXChanged);
	blurY.addListener(&controls, &ControlValues::blurYChanged);
	offsetX.addListener(&controls, &ControlValues::offsetXChanged);
	offsetY.addListener(&controls, &ControlValues::offsetYChanged);

	constants.setName("constants");
	constants.add(maxChaos);
	constants.add(maxSpeed);
	constants.add(maxForce);
	constants.add(blurX);
	constants.add(blurY);
	gui.setPosition(0, 480);

	dust = make_unique<MagicDust>(ofGetWidth(), ofGetHeight());

	channel0.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	channel1.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	channel2.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	chan0 = chan1 = true;

	output.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);

	char *version = (char*)glGetString(GL_VERSION);
	ofLog() << "GL " << version;

	if (!blur_shade.load("shader")) {
		ofLog() << "Shader failed to load";
		ofExit();
	}

	if (!add_shade.load("shader.vert", "add.frag")) {
		ofLog() << "Shader failed to load";
		ofExit();
	}

	setupModes();

	paused = debug = false;

	rows = ofGetHeight() / SCALE;
	cols = ofGetWidth() / SCALE;
	nRows = ofGetHeight() / NEIGHBORHOOD;
	nCols = ofGetWidth() / NEIGHBORHOOD;
	increment = INCREMENT;

	palette.reserve(PALETTE_SIZE);
	for (int i = 0; i < PALETTE_SIZE; i++) {
		palette.push_back(ofColor::chartreuse);
	}

	field = new float*[cols];
	for (int i = 0; i < cols; i++) {
		field[i] = new float[rows];
	}
	dust->boids.reserve(STARTING_BOIDS);

	for (int i = 0; i < STARTING_BOIDS; i++) {
		ofColor c = palette.at((int)floor(ofRandomuf() * palette.size()));
		dust->boids.push_back(new_boid_at(ofRandomWidth(), ofRandomHeight(), c));
	}

	ofSetBackgroundAuto(false);

	initialize();
}

void ofApp::setupModes() {
	selector.setup();
	selector.addMode(0, [&]() {
		initializeColors(0.8, 0.703787, 0.641102, 20923); // Icey Mode
	});
	selector.addMode(1, [&]() {
		initializeColors(0.8, 0.19834, 0.522721, 35879); // Angry Mode
	});

}

void ofApp::initialize() {
	//channel0.begin();
	//ofBackground(32);
	//channel0.end();

	float a = ofRandomuf();
	float b = ofRandomuf();
	fieldColor = jab_to_of(0.2, a, b);
	boidColor = jab_to_of(0.8, 1 - a, 1 - b);
	neighborColor = jab_to_of(1.0, b, a);

	initializeColors(0.8, a, b);
	float startY = ofRandomf();

	xoff = ofRandomf();

	for (int i = 0; i < cols; i++) {
		yoff = startY;
		for (int j = 0; j < rows; j++) {
			field[i][j] = ofNoise(xoff, yoff, zoff) * (2 * PI);
			yoff += increment;
		}
		xoff += increment;
	}

}

void ofApp::initializeColors(float j, float a, float b, int newSeed) {
	if (newSeed) {
		seed = newSeed;
	}
	else {
		seed = floor(ofRandom(50000));
	}

	ofSeedRandom(seed);
	ofLog() << a << ", " << b << "#" << seed;
	for (int i = 0; i < PALETTE_SIZE; i++) {
		palette[i] = jab_random(j, 1. - a, 1. - b);
	}
	dust->set_colors(palette);
}

ofColor ofApp::jab_random(float j, float a, float b) {
	j = 1;
	a += ofRandomuf();
	b += ofRandomuf();

	return jab_to_of(j, a, b);
}


//--------------------------------------------------------------
void ofApp::update(){
	rx.update();
	selector.update();
	if (paused) return;
	if (calibrationMode) return;

	dust->update([&](float x, float y, float speed) {
		glm::ivec2 coords = field_coord(x, y);
		glm::vec2 force = field_force_at(coords.x, coords.y, speed);
		return force;
		}, [&](float x, float y) {
			return field_coord(x, y);
		});

}

void ofApp::update_target_location() {
	//ofPixels& depth = depth_scaled.getPixels();
	//glm::vec3 acc = { 0., 0., 0. };
	//for (int x = 0; x < depth.getWidth(); x++) {
	//	for (int y = 0; y < depth.getHeight(); y++) {
	//		if (depth.getColor(x, y).getBrightness() > 128) {
	//			acc += { (float)x, (float)y, 1. };
	//		}
	//	}
	//}

	//if (acc.z <= 1) return;

	//dust->set_target(acc.x / acc.z * SCALE, acc.y / acc.z * SCALE);
}

glm::ivec2 ofApp::field_coord(float x, float y) {
	return { ofClamp((int)floor(x / SCALE), 0, cols - 1),
			 ofClamp((int)floor(y / SCALE), 0, rows - 1) };

}

glm::vec2 ofApp::field_force_at(int col, int row, float speed) {

	float angle = field[col][row];

	glm::vec2 force = { sin(angle), cos(angle) };

	return glm::normalize(force) * speed;
}

void ofApp::renderTarget(bool filled) {
	const auto& target = dust->get_target();
	ofPushStyle();
	ofSetColor(neighborColor);
	if (filled) {
		ofFill();
	}
	else {
		ofSetLineWidth(10);
		ofNoFill();
	}
	ofDrawRectangle(target.x, target.y, target.z, target.w);
	string msg = "Window Size: " + ofToString(ofGetWidth()) + ", " + ofToString(ofGetHeight());
	ofDrawBitmapString(msg, 10, 10);
	msg = "Incoming: " + ofToString(debugOffset);
	ofDrawBitmapString(msg, 10, 20);
	msg = "Target: " + ofToString(target.x);
	ofDrawBitmapString(msg, 10, 30);
	msg = ", " + ofToString(target.y);
	ofDrawBitmapString(msg, 150, 30);
	msg = "Offset: " + ofToString(offsetX) + ", " + ofToString(offsetY);
	ofDrawBitmapString(msg, 10, 40);
	ofPopStyle();

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (calibrationMode) {
		ofClear(ofColor::black);
		renderTarget(true);
		return;
	}


#ifdef RENDER_MAGIC

		channel0.begin();
		ofClear(0, 0, 0, 255);
		dust->draw_boids();
		channel0.end();

		channel1.begin();
		channel0.draw(0, 0);
		channel1.end();

		channel2.begin();
		ofClear(0, 0, 0, 255);
		channel2.end();

	if (chan0) {
		for (int i = 0; i < ITERATIONS; i++) {
			draw_with(channel1, channel2, blur_shade, HORIZONTAL, i);
			draw_with(channel2, channel1, blur_shade, VERTICAL, i);
		}
		draw_with(channel1, channel2, blur_shade, HORIZONTAL, 1.);
		draw_with(channel2, channel1, blur_shade, VERTICAL, 1.);
	}

	if (!chan1) {
		channel0.begin();
		ofClear(0, 0, 0, 255);
		channel0.end();
	}

	const auto& controls = ControlValues::instance();

	add_shade.begin();
	add_shade.setUniform3f("resolution", { ofGetWidth(), ofGetHeight(), 0 }); //TODO: optimize
	add_shade.setUniformTexture("channel0", channel1.getTexture(), 0);
	add_shade.setUniformTexture("channel1", channel2.getTexture(), 1);
	add_shade.setUniformTexture("base", channel0.getTexture(), 2);
	add_shade.setUniform2f("blurScale", controls.blurControl);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	add_shade.end();

#endif // RENDER_MAGIC


#ifdef RENDER_FLOWS

	ofSetLineWidth(3.);

	ofPixels& depth = depth_scaled.getPixels();

	for (int col = 0; col < cols; col++) {
		for (int row = 0; row < rows; row++) {
			int x = (col + 0.5) * SCALE;
			int y = (row + 0.5) * SCALE;


			glm::vec2 center = { x, y };
			ofColor c = depth.getColor(col, row);
			ofSetColor(fieldColor, c.getBrightness());
			glm::vec2 bias = c.getBrightness() > 128. ? VERTICAL : HORIZONTAL;
			glm::vec2 start = field_force_at(col, row, 7.) + bias * 7.;
			glm::vec2 end = start * -1.;
			ofDrawLine(start + center, end + center);
			ofDrawCircle(start + center, 3.);
		}
	}
#endif // RENDER_FLOWS
}

void ofApp::draw_with(ofFbo& source, ofFbo& target, ofShader& s, const glm::vec2& direction, float scale) {
	target.begin();

	s.begin();
	s.setUniform3f("resolution", { ofGetWidth(), ofGetHeight(), 0 }); //TODO: optimize
	s.setUniformTexture("channel0", source.getTexture(), 0);
	s.setUniform2f("direction", direction * scale);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	s.end();
	target.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	ofImage screen;
	switch (key) {
	case 's':

		screen.grabScreen(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
		screen.saveImage("capture.png");
		break;
	case ' ':
		initialize();
		break;
	case OF_KEY_RETURN:
		paused = !paused;
		break;
	case OF_KEY_TAB:
		debug = !debug;
		break;
	case '1':
		chan0 = !chan0;
		break;
	case '2':
		chan1 = !chan1;
		break;
	case '+':
		ofLog() << "Output Parameters";
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	dust->set_target(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (button == 0) {
		dust->boids.push_back(new_boid_at(x, y, boidColor));
	}
}

boid ofApp::new_boid_at(float x, float y, ofColor c) {
	return {
		{x, y},
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 15, 15. },
		c
	};
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
