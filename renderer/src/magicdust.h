#pragma once

#include "ofMain.h"
#include "ofxQuadtree.h"
#include "constants.h"
#include "boid.h"

class MagicDust {
public:
	MagicDust(int width, int height);
	~MagicDust();

	void update(std::function<glm::vec2(float, float, float)> field_force_at, std::function<glm::ivec2(float, float)> coord);
	void draw();

	void set_colors(std::vector<ofColor>& palette);
	void set_target(float x, float y, float w, float h);
	void set_target(float x, float y);

	const glm::vec4& get_target() const;

	// Temporarily public items
	std::vector<boid> boids; 

	void draw_boids();
	void build_neighbors();
	std::vector<boid*> neighbors_of(boid& b);
	std::vector<boid*> neighbors_of(glm::vec2& pt, float r);

private:
	ofRectangle boundary;
	glm::vec2 bounds;
	ofxQuadtree tree;
	glm::vec4 target;

};