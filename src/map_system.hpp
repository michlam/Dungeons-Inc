#pragma once

///////////////
//  imports  //
/////////////// 
// common
#include "common.hpp"
#include <tiny_ecs_registry.hpp>
#include <world_init.hpp>
#include "nlohmann/json.hpp"

// stdlib
#include <random>

class MapSystem {
public:
	//MapSystem();

	// Releases all associated resources
	// ~MapSystem();

	// initializes the map with n rows of rooms
	void init(int levelDepth);

	// apply current card effects on the room with the specified index in the Rooms component
	void apply_cards(int roomIndex);

	void MapSystem::handle_full_map_view_button(vec3& cameraPosition);

	void MapSystem::generate_full_map_view_rooms(int numRows, vec2 mapDimensions);

	void MapSystem::create_full_map_view_lines(std::vector<Entity> rooms);

	void MapSystem::render_starting_room_and_connecting_rooms(vec2 mapDimensions);

	void MapSystem::render_new_current_room_and_connecting_rooms(Entity& current_room);

private:
	// generate n rows of 3 rooms and connections between
	void generate_rooms(int numRows, vec2 mapDimensions);
	void create_lines(std::vector<Entity> rooms);
	void render_map(std::vector<Entity> connections);
};

// Declaration of map system. DON'T DECLARE IT ANYWHERE ELSE!
extern MapSystem map_system;