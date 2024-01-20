#pragma once
///////////////
//  imports  //
/////////////// 
#include <tiny_ecs_registry.hpp>
#include <render_system.hpp>

///////////////
// constants //
///////////////


///////////////
//   class   //
/////////////// 
class LevelSystem {
public:
	// Constructor
	LevelSystem() { }

	// Initialize the level system
	void LevelSystem::init();

	// Loads a level and begins it.
	void LevelSystem::begin_level(RenderSystem* renderer, int level);

	void LevelSystem::isAlive();

	void showVictoryScreen(RenderSystem* renderer);

	void removeVictoryScreen();

	void showDefeatScreen(RenderSystem* renderer);

	void removeDefeatScreen();

	// Checks if the given level is unlocked
	bool LevelSystem::is_level_unlocked(int level);

	// Update the level's is_unlocked depending on the user_data's level field
	void LevelSystem::update_unlocked_levels();

	// Updates the level in user_data if necessary
	void LevelSystem::save_level_json();

private:
	// Initialize all the levels
	void LevelSystem::initialize_levels();
};


// Declaration of level system. DON'T DECLARE IT ANYWHERE ELSE!
extern LevelSystem level_system;


// Entities for each level
extern Entity tutorial_level;
extern Entity level_one;

extern Entity level_two;
extern Entity level_three;
extern Entity level_four;

extern Entity level_five;
extern Entity level_six;
extern Entity level_seven;

extern std::vector<Entity> adventurers;



// Temporary levels for testing
// extern Entity testing_level_four;
// extern Entity testing_level_five;

// List of levels. Add new levels to this list
extern Entity* level_list[NUM_LEVELS];

// A variable to keep track the current level entity
extern Entity* current_level;

