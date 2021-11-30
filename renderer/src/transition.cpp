#include "transition.h"

Transition::Transition() : currentValue(0), targetValue(0), step(0), duration(0), lastUpdateTime(0), running(false), setter(nullptr), getter(nullptr) { }

void Transition::update() {
	if (!running) return;
	if (setter == nullptr) return;
	if (getter == nullptr) return;

	const float now = ofGetElapsedTimeMillis();

	if (fabsf(currentValue - targetValue) <= 10.0f) {
		ofLog() << "End Transition";
		running = false;
		currentValue = targetValue;
	}
	else {
		currentValue = getter() + (step * (now - lastUpdateTime));
	}
	ofLog() << "Transition: " << currentValue << "->" << targetValue << "==" << step;
	setter(currentValue);
	lastUpdateTime = now;
}

void Transition::target(float t, float d, TransitionSetter s, TransitionGetter g) {
	setter = s;
	getter = g;
	currentValue = getter();
	targetValue = t;
	step = (targetValue - currentValue) / (d * 1000.0);
	running = true;
	ofLog() << "Start Transition";
	lastUpdateTime = ofGetElapsedTimeMillis();
}
