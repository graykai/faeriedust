#pragma once

#include "ofMain.h"

const float SCALE = 30;
const float NEIGHBORHOOD = 80;
const float BUFFER = 50;
const float INCREMENT = 0.05f;
const float INITIAL_MAX_CHAOS = 1.5;
const float INITIAL_MAX_SPEED = 5.;
const glm::vec2 HORIZONTAL = { 1., 0. };
const glm::vec2 VERTICAL = { 0., 1. };
const glm::vec2 ZERO = { 0., 0. };
const glm::vec2 INITIAL_BLUR = { 1.0, 1.0 };
const float INITIAL_MAX_FORCE = 1.5;
const int STARTING_BOIDS =5000;
const int PALETTE_SIZE = 5;
const int ITERATIONS = 5;
const uint16_t RX_PORT = 8000;

class ControlValues {
public:
	float maxForce;
	float maxSpeed;
	float maxChaos;
	glm::vec2 blurControl;
	glm::vec2 offset;

	static ControlValues& instance() {
		if (singleton == nullptr) {
			singleton = new ControlValues();
		}

		return *singleton;
	}

	void forceChanged(float & value) { maxForce = value; }
	void speedChanged(float & value) {
		maxSpeed = value;
	}
	void chaosChanged(float& value) { maxChaos = value; }
	void blurXChanged(float& value) { blurControl.x = value; }
	void blurYChanged(float & value) { blurControl.y = value; }

	void offsetXChanged(float& value) { offset.x = value; }
	void offsetYChanged(float& value) { offset.y = value; }

private:
	ControlValues() {
		maxForce = INITIAL_MAX_FORCE;
		maxChaos = INITIAL_MAX_CHAOS;
		maxSpeed = INITIAL_MAX_SPEED;
		blurControl = INITIAL_BLUR;
		offset = { 0, 0 };
	}
	static ControlValues* singleton;
};