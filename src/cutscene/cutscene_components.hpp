#pragma once

#include <components.hpp>

///////////////
//   class   //
///////////////

// an action that a character performs
// stores the motion and / or goal_pos data to update the character to
struct Action {
	Motion motion;
	Movement movement;
	std::string emotion = "NONE";
};


// an event that occurs in a cutscene
// flags a character and stores their dialogue + their possible actions/movement
// choices represent possible dialogue choices that the user can select in response to whatever
// is happening in the cutscene event
struct CutsceneEvent {
	std::vector<Action> actions;
	std::vector<std::string> dialogue;
	std::vector<std::string> choices;
};


// flag for cutscene choice buttons
struct CutsceneChoice {
	std::string choice;
	bool is_selected = false;
};


// flag for elements in a cutscene
struct CutsceneElement {
	std::string name;
};


// flag for cutscene effects
struct CutsceneEffect {
	std::string effect;
	Entity entity;
	int ticks = 0;
};