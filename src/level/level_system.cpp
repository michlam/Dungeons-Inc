///////////////
//  imports  //
/////////////// 
#include <level/level_system.hpp>
#include <world_init.hpp>
#include <combat_system.hpp>
#include <map_system.hpp>
#include <card/card_system.hpp>
#include <dialogue/dialogue_system.hpp>
#include "utils.hpp"

// Declaration of level system. DON'T DECLARE IT ANYWHERE ELSE!
LevelSystem level_system;

// A variable to keep track the current level entity
Entity* current_level;

// entities for each level
Entity tutorial_level;
Entity level_one;
Entity level_two;
Entity level_three;
Entity level_four;
Entity level_five;
Entity level_six;
Entity level_seven;

Entity condition;

//Entity testing_level_four;
//Entity testing_level_five;
std::vector<Entity> adventurers;

// List of levels. Add new levels to this list
Entity* level_list[NUM_LEVELS] = { &tutorial_level, &level_one,
								&level_two, &level_three, &level_four, &level_five, &level_six, &level_seven
	//&testing_level_four, &testing_level_five
};


void LevelSystem::init() {

	// Load levels from json file
	initialize_levels();

	// Update the level's is_unlocked depending on the user_data's level field
	update_unlocked_levels();
}

void LevelSystem::begin_level(RenderSystem* renderer, int level) {
	// set the current scene
	current_scene = IN_GAME;
	current_level = level_list[level];

	// Recreate all characters
	Level& current_level_component = registry.levels.get(*current_level);
	// create adventurers
	for (int i = 0; i < NUM_ENEMIES; i++) {

		// only create adventurer if they are alive
		if (current_level_component.enemies[i].is_alive) {
			Class::CLASS enemy_class = string_to_class(current_level_component.enemies[i].enemy_class);
			Entity adventurer = createAdventurer(renderer, vec2(600, 550) + character_positions[i], enemy_class, current_level_component.enemies[i]);
			adventurers.push_back(adventurer);
			registry.movements.emplace(adventurer);


			combat_system.init_combat(adventurer);
		}
	}

	// Recreate the map
	map_system.init(current_level_component.depth);

	// Recreate the cards
	card_system.init(renderer);

	// Initialize dialogue system
	dialogue_system.init(renderer);

	phase_system.init(renderer);
}

void LevelSystem::isAlive() {
	Level& current_level_component = registry.levels.get(*current_level);
	for (int i = 0; i < adventurers.size(); i++) {
		if (registry.characters.get(adventurers[i]).stats.hp <= 0) { // fial here
			current_level_component.enemies[i].is_alive = false;
		}
	}
}

void LevelSystem::showVictoryScreen(RenderSystem* renderer) {
	condition = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(condition, &mesh);
	
	Motion& motion = registry.motions.emplace(condition);
	motion.position = vec2(600, 400);
	motion.scale = vec2(1200, 800);

	registry.renderRequests.insert(
		condition,
		{ TEXTURE_ASSET_ID::VICTORY,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"PRIORITY"});

	registry.uiComponents.emplace(condition);
}

void LevelSystem::removeVictoryScreen() {
	registry.remove_all_components_of(condition);
}

void LevelSystem::showDefeatScreen(RenderSystem* renderer) {
	condition = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(condition, &mesh);

	Motion& motion = registry.motions.emplace(condition);
	motion.position = vec2(600, 400);
	motion.scale = vec2(1200, 800);

	registry.renderRequests.insert(
		condition,
		{ TEXTURE_ASSET_ID::DEFEAT,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"PRIORITY" });

	registry.uiComponents.emplace(condition);
}

void LevelSystem::removeDefeatScreen() {
	registry.remove_all_components_of(condition);
}

bool LevelSystem::is_level_unlocked(int level) {
	Level& level_comp = registry.levels.get(*level_list[level]);
	return level_comp.is_unlocked;
}

void LevelSystem::initialize_levels() {
	// load the levels from json
	for (int i = 1; i <= NUM_LEVELS; i++) {
		utils::load_level_data(i);
	}
}

void LevelSystem::update_unlocked_levels() {

	int current_level = utils::get_curr_level();
	
	for (int i = 0; i < registry.levels.entities.size(); i++) {
		Entity entity = registry.levels.entities[i];
		Level& level = registry.levels.get(entity);

		if (i < current_level) {
			level.is_unlocked = true;
		}
		else {
			level.is_unlocked = false;
		}
	}


}

void LevelSystem::save_level_json() {
	// Check the level we just completed. 
	int index = std::distance(level_list, std::find(level_list, level_list + NUM_LEVELS, current_level));

	// Check if it is the last level. If so, do nothing.
	// If not, check if the next level is already unlocked. If so, do nothing.
	// If not, update level by incrementing current level.

	if (index + 1 != NUM_LEVELS) {
		Level& level_comp = registry.levels.get(*level_list[index + 1]);
		if (!level_comp.is_unlocked) {
			utils::update_level(utils::get_curr_level() + 1);
		}
	}

	// Call update unlocked levels.
	update_unlocked_levels();
}


