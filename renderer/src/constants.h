#pragma once

const float SCALE = 30;
const float NEIGHBORHOOD = 50;
const float BUFFER = 50;
const float INCREMENT = 0.05;
const float MAX_CHAOS = 3.;
const glm::vec2 HORIZONTAL = { 1., 0. };
const glm::vec2 VERTICAL = { 0., 1. };
const glm::vec2 ZERO = { 0., 0. };
const float MAX_FORCE = 3.;
const int STARTING_BOIDS = 10000;
const int PALETTE_SIZE = 5;
const int ITERATIONS = 5;

const int DEPTH_WIDTH = 640;
const int DEPTH_HEIGHT = 480;
const float DEPTH_ASPECT = (float)DEPTH_WIDTH / (float)DEPTH_HEIGHT;

