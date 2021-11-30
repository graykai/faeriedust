#pragma once

#include "ofMain.h"

using TransitionSetter = std::function<void(float)>;
using TransitionGetter = std::function<float()>;

class Transition {
public:
	Transition();
	void update();
	void target(float t, float d, TransitionSetter s, TransitionGetter g);

protected:
	float currentValue;
	float targetValue;
	float step;
	float duration;
	float lastUpdateTime;
	bool running;
	TransitionSetter setter;
	TransitionGetter getter;
};
