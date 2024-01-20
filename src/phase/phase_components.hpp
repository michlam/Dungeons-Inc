#pragma once

///////////////
//  imports  //
/////////////// 
#include "common.hpp"

///////////////
// constants //
/////////////// 
// Constants for the time limit of each phase in seconds
const float START_TIME = 5;
const float DRAW_TIME = 5;
const float PLAN_TIME = 45;
const float PRE_COMBAT_TIME = 7.5;
const float COMBAT_TIME = 15;
const float DAMAGE_TIME = 1;
const float END_TIME = 5;

// These are hard coded to the dimensions of the entity texture
const float PHASE_UI_WIDTH = 4.f * 64.f;
const float PHASE_UI_HEIGHT = 4.f * 64.f;

const float TIME_LIMITS[7] = { START_TIME, DRAW_TIME, PLAN_TIME, PRE_COMBAT_TIME,
	COMBAT_TIME, DAMAGE_TIME, END_TIME };

const int NUM_TEXT_LINES = 10;

// Enum of phase types
const enum phase_type {
	START, DRAW, PLAN, PRE_COMBAT, COMBAT, DAMAGE, END
};


///////////////
//  structs  //
/////////////// 

// A phase component
struct Phase {
	phase_type type;
	int round;
	float time_limit;
	float time_remaining;
};

// A text rendering component
struct Text {
	std::string text_lines[NUM_TEXT_LINES] = { "", "", "", "", "", "", "", "", "", "" };
	vec3 colour = { 1.f, 0.f, 0.f };
	vec2 position = { 0.f, 0.f };
	float scale = 1.0f;
};

// A flag component for text that should reset on new level
struct ResettingText {
};


