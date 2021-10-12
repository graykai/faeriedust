#include "magicdust.h"

MagicDust::MagicDust(int width, int height) : boundary({ width / 2.f, height / 2.f, width / 2.f, height / 2.f }), bounds(width, height), tree(boundary, 10.), target({ width / 2., height / 2., 100.0, 250.0 }) {

}

MagicDust::~MagicDust() {

}

void MagicDust::update(std::function<glm::vec2(float,float,float)> field_force_at, std::function<glm::ivec2(float, float)> coord) {
	ControlValues& controls = ControlValues::instance();
	for (auto& b : boids) {
		auto& n = neighbors_of(b);

		glm::vec2 seeking = seek(b, target);
		glm::vec2 flow = field_force_at(b.pos.x, b.pos.y, glm::length(b.max_speed)) - b.vel;
		glm::vec2 sep = separation(b, n);
		glm::vec2 chaos = { ofRandomf(), ofRandomf() };
		chaos = glm::normalize(chaos) * controls.maxChaos - b.vel;
		glm::ivec2 c = coord(b.pos.x, b.pos.y);
		float seek_scale = 1;
		//apply_force(b, seeking + flow + sep + chaos * 0.25);
		apply_force(b, seeking * seek_scale + sep + flow * 0.25);
	}

	for (auto& b : boids) {
		tick(0, b, bounds);
	}

	build_neighbors();
}

void MagicDust::set_colors(std::vector<ofColor>& palette) {
	for (auto& b : boids) {
		b.color = palette.at((int)floor(ofRandomuf() * palette.size()));
	}
}

void MagicDust::draw_boids() {
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetLineWidth(3.);
	for (auto& b : boids) {
		ofSetColor(b.color);
		ofDrawLine(b.pos, b.previous);
	}
}

void MagicDust::build_neighbors() {
	tree.clear();
	for (auto& b : boids) {
		tree.insert(b.pos, &b);
	}
}

std::vector<boid*> MagicDust::neighbors_of(glm::vec2& pos, float radius) {
	static std::vector<boid*> empty;

	size_t count = 0;

	boid** results = (boid**)tree.query(pos, radius, count);
	if (results == nullptr || count == 0) return empty;
	std::vector<boid*> retval;
	retval.reserve(count);

	for (int i = 0; i < count; i++) {
		retval.emplace_back(results[i]);
	}

	return retval;
}

std::vector<boid*> MagicDust::neighbors_of(boid& b) {
	return neighbors_of(b.pos, NEIGHBORHOOD);
}

void MagicDust::set_target(float x, float y, float w, float h) {
	target.x = x;
	target.y = y;
	target.z = w;
	target.w = h;
}

void MagicDust::set_target(float x, float y) {
	target.x = x;
	target.y = y;
}

const glm::vec4& MagicDust::get_target() const {
	return target;
}