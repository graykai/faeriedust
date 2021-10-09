#include "boid.h"

void apply_force(boid& b, glm::vec2& force) {
	auto l = glm::length(force);
	if (l > MAX_FORCE) {
		force = glm::normalize(force) * MAX_FORCE;
	}
	b.accel += force;

}

void tick(const float dt, boid &b, const glm::vec2& boundaries) {
	b.previous = b.pos;
	b.vel += b.accel;
	b.vel = glm::normalize(b.vel) * b.max_speed;
	b.pos += b.vel;

	b.accel *= 0.;
	b.vel *= .75;

	if (b.pos.x < 0) {
		b.pos.x = b.previous.x = boundaries.x;
	}
	if (b.pos.x > boundaries.x) {
		b.pos.x = b.previous.x = 0;
	}

	if (b.pos.y < 0) { b.pos.y = b.previous.y = boundaries.y; }
	if (b.pos.y > boundaries.y) { b.pos.y = b.previous.y = 0; }
}

glm::vec2 separation(boid& b, std::vector<boid*>& n) {
	int count = 0;
	glm::vec2 sum = { 0, 0 };

	for (auto o : n) {
		float d = glm::distance(o->pos, b.pos);
		if (d > 0 && d < BUFFER) {
			glm::vec2 diff = b.pos - o->pos;
			sum += diff;
			count++;
		}
	}

	if (count > 0) {
		sum /= count;
		sum *= b.max_speed;
		if (glm::length(sum) > glm::length(b.max_speed)) {
			sum = glm::normalize(sum) * b.max_speed;
		}
		sum -= b.vel;
		if (glm::length(sum) > glm::length(b.max_speed)) {
			sum = glm::normalize(sum) * b.max_speed;
		}
	}

	return sum;
}

glm::vec2 seek(boid& b, glm::vec2& target) {
	return glm::normalize(target - b.pos) * b.max_speed - b.vel;
}