#pragma once

///////////////
//  imports  //
/////////////// 
#include <common.hpp>
#include <tiny_ecs_registry.hpp>
#include <iostream>
#include <sstream>

// Systems
#include <render_system.hpp>
#include <world_init.hpp>
#include <combat_system.hpp>
#include <dialogue/dialogue_system.hpp>
#include <card/card_functions.hpp>
#include "../audio/audio_system.hpp"
#include "../ai_system.hpp"

///////////////
// constants //
///////////////

///////////////
//   class   //
/////////////// 

// pathways: 1 = left, 2 = right, 3 = middle
// bottom left, top left, top right, bottom right

const std::vector<std::vector<std::vector<vec2>>> character_paths = {
	{
		{vec2(525.0, 550.0), vec2(480.0, 550.0), vec2(480.0, 550.0), vec2(195.0, 550.0), vec2(195.0, 200.0), vec2(195.0, 200.0)},
		{vec2(575.0, 550.0), vec2(480.0, 550.0), vec2(480.0, 550.0), vec2(195.0, 550.0), vec2(195.0, 200.0), vec2(195.0, 200.0)},
		{vec2(625.0, 550.0), vec2(480.0, 550.0), vec2(480.0, 550.0), vec2(195.0, 550.0), vec2(195.0, 200.0), vec2(195.0, 200.0)},
		{vec2(675.0, 550.0), vec2(480.0, 550.0), vec2(480.0, 550.0), vec2(195.0, 550.0), vec2(195.0, 200.0), vec2(195.0, 200.0)}
	},
	{
		{vec2(575.0, 550.0), vec2(740.0, 550.0), vec2(740.0, 550.0), vec2(1005.0, 550.0), vec2(1005.0, 200.0), vec2(1005.0, 200.0)},
		{vec2(525.0, 550.0), vec2(740.0, 550.0), vec2(740.0, 550.0), vec2(1005.0, 550.0), vec2(1005.0, 200.0), vec2(1005.0, 200.0)},
		{vec2(675.0, 550.0), vec2(740.0, 550.0), vec2(740.0, 550.0), vec2(1005.0, 550.0), vec2(1005.0, 200.0), vec2(1005.0, 200.0)},
		{vec2(625.0, 550.0), vec2(740.0, 550.0), vec2(740.0, 550.0), vec2(1005.0, 550.0), vec2(1005.0, 200.0), vec2(1005.0, 200.0)}
	},
	{
		{vec2(600.0, 475.0),  vec2(600.0, 200.0), vec2(600.0, 200.0)},
		{vec2(600.0, 575.0),  vec2(600.0, 200.0), vec2(600.0, 200.0)},
		{vec2(600.0, 525.0),  vec2(600.0, 200.0), vec2(600.0, 200.0)},
		{vec2(600.0, 625.0),  vec2(600.0, 200.0), vec2(600.0, 200.0)}
	}
};

class PhaseSystem {
public:
	// Constructor
	PhaseSystem() {

	}

	// Initialize the phase system, including the UIs
	void PhaseSystem::init(RenderSystem* renderer_arg);

	// Iterate to next phase and call corresponding functions
	void PhaseSystem::next_phase();

	// Change time_remaining based on elapsed_ms, call next_phase if necessary
	void PhaseSystem::step(float elapsed_ms_since_last_update);

	// Pause if unpaused. Unpause if paused.
	void PhaseSystem::toggle_pause();

	// Keyboard handler for key inputs
	void PhaseSystem::keyboard_handler(int key, int action);

	// Resets the phase back to initial values
	void PhaseSystem::reset_phase();

	// Convert a string to enum of phase_type
	phase_type PhaseSystem::string_to_phase_type(std::string str);

	Entity current_room;

	// Deletes the phase
	void PhaseSystem::delete_phase();

	// Deletes the cards
	void PhaseSystem::delete_cards();

	// Deletes the map
	void PhaseSystem::delete_map();

	// Deletes the characters
	void PhaseSystem::delete_characters();

private:
	// Creates a phase ui entity
	Entity create_phase_ui(RenderSystem* renderer);

	// Creates a phase textbox entity
	Entity create_phase_textbox(RenderSystem* renderer);	
	
	// Creates a lore ui entity
	Entity create_lore_ui(RenderSystem* renderer);

	// Creates a lore textbox entity
	Entity create_lore_textbox(RenderSystem* renderer);

	// Creates a tutorial ui entity
	Entity create_tutorial_ui(RenderSystem* renderer);

	// Creates a tutorial textbox entity
	Entity create_tutorial_textbox(RenderSystem* renderer);

	// Creates the pause overlay entity
	Entity create_pause_overlay(RenderSystem* renderer);

	// Creates the play overlay entity
	Entity PhaseSystem::create_play_overlay(RenderSystem* renderer);


	// Updates the text in the lore textbox
	void update_lore_textbox();

	// Updates the text in the phase textbox
	void update_phase_textbox();

	// Updates the text in the tutorial textbox
	void update_tutorial_textbox();

	int position_to_index(vec2 position);
	// Returns a string of the given phase type
	std::string get_phase_type(phase_type type);

	Entity phase_ui;
	Entity phase_textbox;
	Entity lore_ui;
	Entity lore_textbox;
	Entity tutorial_ui;
	Entity tutorial_textbox;
	Entity pause_overlay;
	Entity play_overlay;


	// Game state
	RenderSystem* renderer;
	Audio_system* audio_player;
	AISystem* ai_system;
};

// Declaration of phase system. DON'T DECLARE IT ANYWHERE ELSE!
extern PhaseSystem phase_system;

// A variable to keep track if the phase is paused
extern bool is_paused;
