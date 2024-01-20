#pragma once
///////////////
//  imports  //
/////////////// 
#include <common.hpp>

///////////////
// constants //
///////////////

// Enum of levels
const enum level_type {
	LEVEL_TUTORIAL, 
	LEVEL_ONE,
	LEVEL_TWO,
	LEVEL_THREE,
	LEVEL_FOUR,
	LEVEL_FIVE,
	LEVEL_SIX,
	LEVEL_SEVEN
	//TESTING_LEVEL_FOUR,
	//TESTING_LEVEL_FIVE
};

// UPDATE THIS WHEN ADDING NEW LEVELS
const int NUM_LEVELS = 8;

const int NUM_ENEMIES = 4;

const vec2 character_positions[4] = {
	vec2(-50, -50),
	vec2(-50, +50),
	vec2(+50, -50),
	vec2(+50, +50)
};

///////////////
//  structs  //
/////////////// 

// Enemy struct
struct Enemy {
	std::string enemy_class;
	int difficulty_scale;
	int exp_dropped = 0;
	bool is_alive = 0;
};

// Level component
struct Level {
	std::string name;
	// std::string description;
	int depth;
	int difficulty = 1;
	bool is_unlocked = false;
	bool is_tutorial = false;
	Enemy enemies[NUM_ENEMIES];
};