#pragma once

///////////////
//  imports  //
/////////////// 
// common
#include "common.hpp"
//#include "phase/phase_system.hpp"
#include <map>


///////////////
//  structs  //
/////////////// 
// stats for a character
struct Stats {
	int hp = 0;
	int atk = 0;
	int def = 0;
	int statSpread = 100;
};

struct StatChange {
	Stats stat_change;
};

// group of adventurers raiding the dungeon
struct Character {
	std::string name;
	Stats stats;
	Entity weapon;
	// add class, race info
	// hopefully we can implement dynamic character sprites based on their class and race, 
	// which is why they're fields of character instead

	Entity health_bar;

	Entity stat_box;

};

// an entity can target one other character entity
struct Target {
	Entity other_entity;
	Target(Entity& other_entity) { this->other_entity = other_entity; };
};

struct InCombat {
	// used to flag which entities are in combat so that it can run damage calculation (easier bc it can follow step template)
};

struct Mob {
};

// an adventurer entity has a class represented as an enumeration
struct Class {


	typedef enum { WARRIOR, MAGE, RANGER } CLASS;
	CLASS advClass;
};

// a mob entity has a race that affects their appearance
struct MobClass {
	typedef enum { BARBARIAN, BRUTE, SHAMAN } MOBCLASS;
	MOBCLASS mobClass;
};


// statBox data structure for adventurers
struct StatusBox {
};

/*
struct StatusText {
	Stats stats;
	Class adventurerClass;
};
*/

struct Weapon {
	Entity parent;
};

struct HealthBar {
	int health = 6;
	int max_health;
};

// helper to get the adventurer class based on a string
Class::CLASS string_to_class(std::string str);