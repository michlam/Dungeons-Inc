#pragma once
///////////////
//  imports  //
/////////////// 
#include "common.hpp"

// stlib
#include <vector>
#include <random>

// audio mixing
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

// systems
#include "render_system.hpp"
#include "phase/phase_system.hpp"


///////////////
//   class   //
/////////////// 
class WorldSystem {
// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods

public:
	WorldSystem();

	// Creates a window
	GLFWwindow* create_window();

	// starts the game
	void init(RenderSystem* renderer);

	// Releases all associated resources
	~WorldSystem();

	// Steps the game ahead by ms milliseconds
	bool step(float elapsed_ms);

	// Check for collisions
	void handle_collisions();

	// Should the game be over ?
	bool is_over()const;

private:
	void updateAnimations(float elapsed_ms);

	// restart level
	void restart_game();

	// OpenGL window handle
	GLFWwindow* window;

	// Game state
	RenderSystem* renderer;

	Entity level;
	Entity phase_ui;

	// music references
	Mix_Music* background_music;
	Audio_system* audio_player;

	// C++ random number generator
	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniform_dist; // number between 0..1

};
