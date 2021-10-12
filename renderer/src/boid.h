#pragma once
#include "ofMain.h"

struct boid {
	glm::vec2 pos;
	glm::vec2 vel;
	glm::vec2 accel;
	glm::vec2 previous;

	// tuneables
	glm::vec2 max_speed;
	ofColor color;
};

void apply_force(boid& b, glm::vec2& force);
void tick(const float dt, boid& b, const glm::vec2& boundaries);

glm::vec2 separation(boid& b, std::vector<boid*>& n);
glm::vec2 seek(boid& b, glm::vec4& target);


