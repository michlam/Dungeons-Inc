///////////////
//  imports  //
///////////////
#include "map_system.hpp"
#include "phase/phase_system.hpp"
#include <level/level_system.hpp>


///////////////
// constants //
///////////////
const int ROOMS_PER_ROW = 3;
// width = 300 px, height = 200 px
const vec2 ROOM_DIMENSIONS = { 200, 100 };
const float offset_x = 195.0;
const float offset_y = 200.0;
const float dist_between_rooms_x = 405.0;
const float dist_between_rooms_y = 356.0;
const float center_x = 600;


// Declaration of map system. DON'T DECLARE IT ANYWHERE ELSE!
MapSystem map_system;


///////////////
// functions //
/////////////// 
/* MapSystem::MapSystem() { }


MapSystem::~MapSystem() { }
*/

void MapSystem::init(int levelDepth) {
	int width = window_width_px;
	int height = window_height_px;

	// printf("height: %d\n", height);
	// printf("width: %d\n", width); 

	Entity map = createMap();
	//generate_rooms(levelDepth, registry.motions.get(map).position);

	create_switch_map_view_button();


	// initialize 3 backgrounds which can hopefully house all the rooms, add more if need
	// the upper y limit for camera control will be handled by a calculation with level depth
	createRoomsViewMap(0);
	createRoomsViewMap(1);
	createRoomsViewMap(2);
	createRoomsViewMap(3);

	generate_full_map_view_rooms(levelDepth, registry.motions.get(map).position);

	render_starting_room_and_connecting_rooms(registry.motions.get(map).position);
}

void MapSystem::render_starting_room_and_connecting_rooms(vec2 mapDimensions) {
	Entity start_room = registry.room.entities[0];
	std::vector<Entity>& connections = registry.room.get(start_room).connections;
	int num_connections = connections.size();

	create_rooms_and_paths(TEXTURE_ASSET_ID::DUNGEON_MAP_LMR);

	Entity main_room = createRoom({ mapDimensions.x, mapDimensions.y + 550 });
	registry.renderRequests.remove(main_room);

	CorrespondingRoom& corresponding_room = registry.correspondingRoom.emplace(main_room);
	corresponding_room.room = start_room;
	float y_pos = (mapDimensions.y + 550) - dist_between_rooms_y;
	
	for (int i = 0; i < num_connections; i++) {
		float x_pos = registry.motions.get(connections[i]).position.x;
		Entity room = createRoom({x_pos, y_pos});
		registry.renderedRoom.get(main_room).connections.push_back(room);
		CorrespondingRoom& corresponding_room = registry.correspondingRoom.emplace(room);
		corresponding_room.room = connections[i];
	}
}

void MapSystem::render_new_current_room_and_connecting_rooms(Entity& current_room) {

	vec2 mapDimensions = vec2(1500 / 2 - (1500 - 1200) / 2, -1 * 1600 / 2 + 800);
	
	//delete current rooms that are rendered and lines
	std::vector<Entity> current_rendered_entities = registry.currentRender.entities;
	for (int j = 0; j < current_rendered_entities.size(); j++) {
		registry.remove_all_components_of(current_rendered_entities[j]);
	}
	
	//render new rooms and lines
	std::vector<Entity> connections = registry.room.get(current_room).connections;
	int num_connections = connections.size();

	render_map(connections);

	Entity main_room = createRoom({ mapDimensions.x, mapDimensions.y + 550 });
	registry.renderRequests.remove(main_room);

	CorrespondingRoom& corresponding_room = registry.correspondingRoom.emplace(main_room);
	corresponding_room.room = current_room;

	float y_pos = (mapDimensions.y + 550) - dist_between_rooms_y;

	for (int i = 0; i < num_connections; i++) {
		float x_pos = registry.motions.get(connections[i]).position.x;
		Entity room = createRoom({ x_pos, y_pos });
		registry.renderedRoom.get(main_room).connections.push_back(room);
		CorrespondingRoom& corresponding_room = registry.correspondingRoom.emplace(room);
		corresponding_room.room = connections[i];
	}
}

void MapSystem::render_map(std::vector<Entity> connections) {
	int num_connections = connections.size();

	if (num_connections == 3) {
		create_rooms_and_paths(TEXTURE_ASSET_ID::DUNGEON_MAP_LMR);
	}
	else if (num_connections == 0) {
		create_rooms_and_paths(TEXTURE_ASSET_ID::DUNGEON_MAP_NONE);
	}
	else if (num_connections == 1) {
		vec2 position = registry.motions.get(connections[0]).position;
		if (position.x == 195) {
			create_rooms_and_paths(TEXTURE_ASSET_ID::DUNGEON_MAP_L);
		}
		else if (position.x == 600) {
			create_rooms_and_paths(TEXTURE_ASSET_ID::DUNGEON_MAP_M);
		}
		else {
			create_rooms_and_paths(TEXTURE_ASSET_ID::DUNGEON_MAP_R);
		}
	}
	else {
		vec2 position_1 = registry.motions.get(connections[0]).position;
		vec2 position_2 = registry.motions.get(connections[1]).position;
		if ((position_1.x == 195 && position_2.x == 600) || (position_1.x == 600 && position_2.x == 195)) {
			create_rooms_and_paths(TEXTURE_ASSET_ID::DUNGEON_MAP_LM);
		}
		else if ((position_1.x == 600 && position_2.x == 1005) || (position_1.x == 1005 && position_2.x == 600)) {
			create_rooms_and_paths(TEXTURE_ASSET_ID::DUNGEON_MAP_MR);
		}
		else {
			create_rooms_and_paths(TEXTURE_ASSET_ID::DUNGEON_MAP_LR);
		}
	}
}

void MapSystem::handle_full_map_view_button(vec3& cameraPosition) {
	if (registry.fullMapViewBackground.entities.size() > 0) {
		Entity& full_map_view_button= registry.fullMapViewButton.entities[0];
		int active = registry.fullMapViewButton.get(full_map_view_button).is_active;
		
		for (int i = 0; i < registry.fullMapViewBackground.entities.size(); i++) {
			Entity& full_map_view_background = registry.fullMapViewBackground.entities[i];
			if (!registry.fullMapViewButton.get(full_map_view_button).is_active) {
				registry.renderRequests.get(full_map_view_background).z_val = "SHOW ROOM VIEW MAP";
			}
			else {
				registry.renderRequests.get(full_map_view_background).z_val = "HIDE ROOM VIEW MAP";
			}
		}

		for (int i = 0; i < registry.fullMapViewRoom.entities.size(); i++) {
			Entity& full_map_view_room= registry.fullMapViewRoom.entities[i];
			if (!registry.fullMapViewButton.get(full_map_view_button).is_active) {
				registry.renderRequests.get(full_map_view_room).z_val = "SHOW ROOM VIEW ROOM";
			}
			else {
				registry.renderRequests.get(full_map_view_room).z_val = "HIDE ROOM VIEW ROOM";
			}
		}

		for (int i = 0; i < registry.fullMapViewLine.entities.size(); i++) {
			Entity& full_map_view_line= registry.fullMapViewLine.entities[i];
			if (!registry.fullMapViewButton.get(full_map_view_button).is_active) {
				registry.renderRequests.get(full_map_view_line).z_val = "SHOW ROOM VIEW LINE";
			}
			else {
				registry.renderRequests.get(full_map_view_line).z_val = "HIDE ROOM VIEW LINE";
			}
		}
		if (registry.fullMapViewButton.get(full_map_view_button).is_active) {
			cameraPosition.x = 0.f;
			cameraPosition.y = 0.f;
			current_scene = IN_GAME;
		}
		else {
			cameraPosition.x = 0.0;
			cameraPosition.y = -1*(registry.motions.get(phase_system.current_room).position.y) + 650;
			current_scene = IN_FULL_MAP_VIEW;
		}
		registry.fullMapViewButton.get(full_map_view_button).is_active = !registry.fullMapViewButton.get(full_map_view_button).is_active;
	}
}

// Apply all the card effects on room with index roomIndex in the rooms array
void MapSystem::apply_cards(int roomIndex) {

}

// Generate 3 x numRooms rooms with random connections between every pair of 3 rooms
// numRows does not include the beginning or end row
void MapSystem::generate_rooms(int numRows, vec2 mapDimensions) {

	std::vector<Entity> empty_rooms;
	// Generate the initial rooms and store them into a vector array
	std::vector<Entity> rooms;

	Entity starting_room = createRoom({ mapDimensions.x, mapDimensions.y + 800 / 1.25 });

	rooms.push_back(starting_room);

	float prev_y = mapDimensions.y + 800 / 1.25;

	for (int i = 0; i < numRows; i++) {
		float room_y = prev_y - dist_between_rooms_y;
		for (int j = 0; j < ROOMS_PER_ROW; j++) {
			float room_x = offset_x + (j * dist_between_rooms_x);
			Entity room = createRoom({ room_x, room_y });
			rooms.push_back(room);
		}
		prev_y = room_y;
	}


	Entity ending_room = createRoom({ mapDimensions.x, prev_y - dist_between_rooms_y });

	rooms.push_back(ending_room);

	// Connect the first 3 rooms to the starting room
	std::vector<Entity> first_three_rooms = { rooms[1], rooms[2], rooms[3] };
	registry.room.get(starting_room).connections = first_three_rooms;

	int num_rooms = rooms.size();
	for (int k = 1; k + (ROOMS_PER_ROW - 1) < num_rooms - ROOMS_PER_ROW; k += ROOMS_PER_ROW) {


		// Get the 3 rooms in the next layer and add them to a vector array
		std::vector<std::vector<Entity>> to_connect_to;
		std::vector<Entity> left_room{ rooms[k + 3] };
		std::vector<Entity> middle_room{ rooms[k + 4] };
		std::vector<Entity> right_room{ rooms[k + 5] };

		to_connect_to.push_back(left_room);
		to_connect_to.push_back(middle_room);
		to_connect_to.push_back(right_room);

		//Generate random room connections
		for (int a = 0; a < ROOMS_PER_ROW; a++) {
			for (int b = 3; b < 5; b++) {
				if (b != a + 3) {
					if (rand() > (RAND_MAX / 2)) {
						to_connect_to[a].push_back(rooms[k + b]);
					}
				}
			}
		}

		// Shuffle the rooms rnadomly
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(to_connect_to.begin(), to_connect_to.end(), gen);

		// Randomly connect the 3 roooms in the next layer to the 3 rooms in the current layer
		registry.room.get(rooms[k]).connections = to_connect_to[0];
		registry.room.get(rooms[k + 1]).connections = to_connect_to[1];
		registry.room.get(rooms[k + 2]).connections = to_connect_to[2];

	}

	// Connect last 3 rooms to ending room
	std::vector<Entity> last_room = { rooms[num_rooms - 1] };


	// Subtracting one because of ending room in rooms array
	for (int l = num_rooms - ROOMS_PER_ROW - 1; l < num_rooms - 1; l++) {
		registry.room.get(rooms[l]).connections = last_room;
	}

	// Render pathways
	create_lines(rooms);

}


void MapSystem::generate_full_map_view_rooms(int numRows, vec2 mapDimensions) {

	std::vector<Entity> empty_rooms;
	// Generate the initial rooms and store them into a vector array
	std::vector<Entity> rooms;

	Entity starting_room = createFullMapViewRoom({ mapDimensions.x, mapDimensions.y + 800 / 1.25 });

	registry.renderRequests.get(starting_room).used_effect = EFFECT_ASSET_ID::GLOWING;

	rooms.push_back(starting_room);

	float prev_y = mapDimensions.y + 800 / 1.25;

	for (int i = 0; i < numRows; i++) {
		float room_y = prev_y - dist_between_rooms_y;
		for (int j = 0; j < ROOMS_PER_ROW; j++) {
			float room_x = offset_x + (j * dist_between_rooms_x);
			Entity room = createFullMapViewRoom({ room_x, room_y });
			rooms.push_back(room);
		}
		prev_y = room_y;
	}


	Entity ending_room = createFullMapViewRoom({ mapDimensions.x, prev_y - dist_between_rooms_y });

	rooms.push_back(ending_room);

	// Connect the first 3 rooms to the starting room
	std::vector<Entity> first_three_rooms = { rooms[1], rooms[2], rooms[3] };
	registry.room.get(starting_room).connections = first_three_rooms;

	int num_rooms = rooms.size();
	for (int k = 1; k + (ROOMS_PER_ROW - 1) < num_rooms - ROOMS_PER_ROW; k += ROOMS_PER_ROW) {


		// Get the 3 rooms in the next layer and add them to a vector array
		std::vector<std::vector<Entity>> to_connect_to;
		std::vector<Entity> left_room{ rooms[k + 3] };
		std::vector<Entity> middle_room{ rooms[k + 4] };
		std::vector<Entity> right_room{ rooms[k + 5] };

		to_connect_to.push_back(left_room);
		to_connect_to.push_back(middle_room);
		to_connect_to.push_back(right_room);

		//Generate random room connections
		for (int a = 0; a < ROOMS_PER_ROW; a++) {
			for (int b = 3; b < 5; b++) {
				if (b != a + 3) {
					if (rand() > (RAND_MAX / 2)) {
						to_connect_to[a].push_back(rooms[k + b]);
					}
				}
			}
		}

		// Shuffle the rooms rnadomly
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(to_connect_to.begin(), to_connect_to.end(), gen);

		// Randomly connect the 3 roooms in the next layer to the 3 rooms in the current layer
		registry.room.get(rooms[k]).connections = to_connect_to[0];
		registry.room.get(rooms[k + 1]).connections = to_connect_to[1];
		registry.room.get(rooms[k + 2]).connections = to_connect_to[2];

	}

	// Connect last 3 rooms to ending room
	std::vector<Entity> last_room = { rooms[num_rooms - 1] };


	// Subtracting one because of ending room in rooms array
	for (int l = num_rooms - ROOMS_PER_ROW - 1; l < num_rooms - 1; l++) {
		registry.room.get(rooms[l]).connections = last_room;
	}

	// Render pathways
	create_full_map_view_lines(rooms);

}

void MapSystem::create_lines(std::vector<Entity> rooms) {
	for (int i = 0; i < rooms.size(); i++) {
		float x_2 = registry.motions.get(rooms[i]).position.x;
		float y_2 = registry.motions.get(rooms[i]).position.y;
		std::vector<Entity> room_connections = registry.renderedRoom.get(rooms[i]).connections;
		for (int j = 0; j < room_connections.size(); j++) {

			float x_1 = registry.motions.get(room_connections[j]).position.x;
			float x = (x_1 + x_2) / 2;

			float y_1 = registry.motions.get(room_connections[j]).position.y;
			float y = (y_1 + y_2) / 2;

			float euclidean_dist = sqrt(pow((x_2 - x_1), 2) + pow((y_2 - y_1), 2));
			vec2 scale = { 5, euclidean_dist / 1.5 };

			float angle = atan2(y_1 - y_2, x_1 - x_2) - M_PI/2;

			Entity connection = createLine({ x, y }, scale, angle);
			//registry.currentRender.emplace(connection);
		}
	}
}

void MapSystem::create_full_map_view_lines(std::vector<Entity> rooms) {
	for (int i = 0; i < rooms.size(); i++) {
		float x_2 = registry.motions.get(rooms[i]).position.x;
		float y_2 = registry.motions.get(rooms[i]).position.y;
		std::vector<Entity> room_connections = registry.room.get(rooms[i]).connections;
		for (int j = 0; j < room_connections.size(); j++) {

			float x_1 = registry.motions.get(room_connections[j]).position.x;
			float x = (x_1 + x_2) / 2;

			float y_1 = registry.motions.get(room_connections[j]).position.y;
			float y = (y_1 + y_2) / 2;

			float euclidean_dist = sqrt(pow((x_2 - x_1), 2) + pow((y_2 - y_1), 2));
			vec2 scale = { 5, euclidean_dist };

			float angle = atan2(y_2 - y_1, x_2 - x_1) - 3.14159 / 2;

			Entity connection = createFullMapViewLine({ x, y }, scale, angle);
		}
	}
}

template <typename Prob = double>
bool binomial_trial(const Prob p = 0.5) {
	static auto dev = std::random_device();
	static auto gen = std::mt19937{ dev() };
	static auto dist = std::uniform_real_distribution<Prob>(0, 1);
	return (dist(gen) < p);
}