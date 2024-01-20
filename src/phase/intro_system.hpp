#pragma once

#include <physics_system.hpp>
#include <phase/phase_system.hpp>
#include "audio/audio_system.hpp"
#include "level_select/level_select_system.hpp"
#include <menu/menu_system.hpp>

///////////////
// constants //
///////////////

const float WINDOW_WIDTH = 1200.f;
const float WINDOW_HEIGHT = 800.f;
const float COLLISION_BOUND = WINDOW_HEIGHT / 5 * 3;

const float GOBLIN_HEIGHT = 250;
const float GOBLIN_WIDTH = 200;

const float START_X = 50.f;
const float END_X = 1080.f;

const float JUMP_START_X = 600.f;
const float JUMP_MID_X = JUMP_START_X + 100.f;
const float JUMP_END_X = JUMP_MID_X + 100.f;

const float START_Y = 375.f;
const float MID_Y = START_Y - 25.f;
const float END_Y = MID_Y + 100.f;

const float START_ANGLE = 0.f;
const float END_ANGLE = 0.1f;

const float ANGLE_INCREMENT = 0.05f;

const float ACCELERATION = 1.05f;
const float DECELERATION = 0.85f;


///////////////
//   class   //
/////////////// 
class IntroSystem {
public:
	// Constructor
	IntroSystem() {

	}

	// Initialize the phase system, including the UIs
	void IntroSystem::init(RenderSystem* renderer_arg);

	// Change time_remaining based on elapsed_ms, call next_phase if necessary
	void IntroSystem::step(float elapsed_ms_since_last_update, RenderSystem* renderer_arg);

private:
	// Ends the intro scene and goes directly into tutorial level
	void IntroSystem::end_intro(RenderSystem* renderer_arg);

	// handles falling logo text movement
	void IntroSystem::text_step(Motion& motion);

	// Create cutscene entities entity
	Entity create_window(RenderSystem* renderer);
	Entity create_running_goblin(RenderSystem* renderer);
	Entity create_logo(RenderSystem* renderer);
	Entity create_ground(RenderSystem* renderer);


	Entity window;
	Entity ground;
	Entity goblin;
	Entity logo;

	Mesh* logo_mesh;

	int total_elapsed_time = 0;

	// larger value = slower movement here, idk what to name these sorry
	float x_change = 2000.f;
	float y_change = 500.f;

	float curr_angle = 0.f;

	// different from y_change (for the logo falling animation)
	float y_velocity = 1000.f;

	// Game state
	RenderSystem* renderer;
};

// Declaration of intro system. DON'T DECLARE IT ANYWHERE ELSE!
extern IntroSystem intro_system;

extern bool is_intro_complete;