#pragma once

///////////////
//  imports  //
///////////////

// stlib
#include <vector>

// common
#include "tiny_ecs_registry.hpp"
#include "common.hpp"


///////////////
//   class   //
/////////////// 
class AISystem {
public:
	//static int room_tracker = 0;
	enum Difficulty {
		EASY,
		MEDIUM,
		HARD
	};

	int random_integer(int lower_bound, int upper_bound);

	void step(float elapsed_ms);

	void init(Difficulty new_difficulty);

    void set_difficulty(Difficulty new_difficulty);

	static AISystem* get_ai_system();



	Difficulty get_difficulty();

	//static ivec4 move_to_next_room(ivec2 current_room, std::vector<std::vector<Entity>>* room_connections);

	Entity move_to_next_room(Entity current_room);
private:
	Difficulty difficulty_level;
	static AISystem* ai_system;

	AISystem() {
	}
};