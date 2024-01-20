///////////////
//  imports  //
/////////////// 
// opengl
#define GL3W_IMPLEMENTATION
#include <gl3w.h>

// stlib
#include <chrono>
#include <iostream>
#include <thread>

// systems
#include "audio/audio_system.hpp"
#include "dialogue/dialogue_system.hpp"
#include "cutscene/cutscene_system.hpp"

#include "phase/intro_system.hpp"
#include "level/level_system.hpp"
#include "phase/phase_system.hpp"

#include "card/card_system.hpp"
#include "combat_system.hpp"
#include "map_system.hpp"

#include "physics_system.hpp"
#include "render_system.hpp"
#include "world_system.hpp"
#include "animation_system.hpp"
#include "ai_system.hpp"

///////////////
// constants //
/////////////// 
using Clock = std::chrono::high_resolution_clock;
// const int LEVEL_DEPTH = 2;


///////////////
// functions //
/////////////// 
// Entry point
int main() {
	// Global systems
	WorldSystem world_system;
	RenderSystem render_system;
	PhysicsSystem physics_system = PhysicsSystem(&render_system);
	IntroSystem intro_system;
	
	
	//MapSystem map_system;
	CardSystem card_system;

	// Initializing window
	GLFWwindow* window = world_system.create_window();
	if (!window) {
		// Time to read the error message
		printf("Press any key to exit");
		getchar();
		return EXIT_FAILURE;
	}

	// initialize the main systems
	render_system.init(window);
	level_system.init();
	
	world_system.init(&render_system);

	intro_system.init(&render_system);

	cutscene_system.init(&render_system);

	//std::this_thread::sleep_for(std::chrono::seconds(6));

	AISystem* ai_system = AISystem::get_ai_system();
	AISystem::Difficulty d = utils::load_AI_config();
	ai_system->set_difficulty(d);

	Audio_system* audio_player = Audio_system::get_audio();
	utils::load_audio_config();
	audio_player->loop_game_bgm();
	// map_system.init(LEVEL_DEPTH); moved to level_system.begin_level
	// world_system.path = map_system.adventure_path;
	// phase_system.init(&render_system);

	// variable timestep loop
	auto t = Clock::now();
	while (!world_system.is_over()) {
		// Processes system messages, if this wasn't present the window would become unresponsive
		glfwPollEvents();

		// Calculating elapsed times in milliseconds from the previous iteration
		auto now = Clock::now();
		float elapsed_ms =
			(float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count() / 1000;
		t = now;

		world_system.step(elapsed_ms);
		physics_system.step(elapsed_ms);
		phase_system.step(elapsed_ms);
		intro_system.step(elapsed_ms, &render_system);
		animation_system.step(elapsed_ms);

		
		if (current_scene == LEVEL_SELECT) level_select_system.step(elapsed_ms);
		else if (current_scene == IN_GAME) dialogue_system.step(elapsed_ms);
		else if (current_scene == CUTSCENE) cutscene_system.step(elapsed_ms);

		world_system.handle_collisions();

		render_system.draw();
	}

	return EXIT_SUCCESS;
}