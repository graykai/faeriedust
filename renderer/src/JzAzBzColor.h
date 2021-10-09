#pragma once

#include "ofMain.h"

ofColor jab_to_of(float j, float a, float b);
glm::vec3 xyz_to_srgb(float x, float y, float z);

class JzAzBzColor
{
public:
	JzAzBzColor(ofColor color) : JzAzBzColor(color.r, color.g, color.b) {
	}

	JzAzBzColor(float j, float a, float b) : value({ j, a, b }) {
	}

	const float Jz() { return value[0]; }
	const float Az() { return value[1]; }
	const float Bz() { return value[2]; }

	ofColor color() {
		return jab_to_of(value.x, value.y, value.z);
	}

private:
	glm::vec3 value;

	glm::vec3 to_srgb();

};


