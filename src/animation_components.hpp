#pragma once

///////////////
//  imports  //
///////////////
// common
#include "common.hpp"

///////////////
//  structs  //
///////////////
struct Fading {
	float time_elapsed = 0.f;
	float max_time = 100.f;

	float fade_factor = 1.f;
};

struct FadingWeapon {

	float drop = 1.f;

	float angle;

};


struct DamageNumber {

	float decrement = 2.f;

};

struct Swinging {
	float time_elapsed = 0.f;
	float max_time = 2 * M_PI;
	float target_angle;


};

struct Casting {
	float time_elapsed = 0.f;
	float max_time = 2 * M_PI;
	float target_angle;


};

struct Shooting {
	float time_elapsed = 0.f;
	float max_time = 2 * M_PI;
	float target_angle;
	vec2 original_pos;

};

struct Stabbing {
	float time_elapsed = 0.f;
	float max_time = 2 * M_PI;
	float target_angle;

};

struct Blood {
	float time_elapsed = 0.f;
	float alpha = 1.0f;
};
