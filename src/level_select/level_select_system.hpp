#pragma once
///////////////
//  imports  //
/////////////// 
#include <render_system.hpp>
#include <tiny_ecs_registry.hpp>
#include "../audio/audio_system.hpp"


///////////////
// constants //
///////////////

///////////////
//   class   //
/////////////// 

class LevelSelectSystem {
public:
	// Constructor
	LevelSelectSystem() {

	}

	// Initialize the level select system, including the UIs
	void LevelSelectSystem::init(RenderSystem* renderer_arg);

	// Handles selecting level for popup window
	void handle_level_selection(RenderSystem* renderer_arg, int level);

	// Handles the motion for popup windows
	void LevelSelectSystem::step(float elapsed_ms_since_last_update);

	// Handles loading a level once it's selected to start
	void confirm_selection(RenderSystem* renderer_arg);

	// Initiates the teardown of lsm
	void LevelSelectSystem::teardown_lsm();


private:

	// Create the background
	void create_background(RenderSystem* renderer);

	// Create all the level selector buttons
	void LevelSelectSystem::create_level_selector(RenderSystem* renderer_arg);

	// Creates a popup window that appears when a level selector button is pressed
	Entity LevelSelectSystem::create_popup_window(RenderSystem* renderer_arg);

	// Create a start button that corresponds to a popup window
	Entity LevelSelectSystem::create_start_button(RenderSystem* renderer_arg, Entity parent);

	// Handle the background parallax movement
	void handle_parallax(RenderSystem* renderer_arg, int level);


	// Create the top left level select text
	Entity LevelSelectSystem::create_level_select_text(RenderSystem * renderer_arg);

	// Create a little guy that moves around on level select
	void LevelSelectSystem::create_little_guy(RenderSystem* renderer_arg);


	Entity bg_1;
	Entity bg_2;
	Entity bg_3;
	Entity bg_4;

	Entity popup_window_one;
	Entity popup_window_two;
	Entity start_button_one;
	Entity start_button_two;

	Entity level_select_text;

	Entity little_guy;

	Entity selector_bounding_box;

	int level_focus;

	Audio_system* audio_player;
};

// Declaration of level_select system. DON'T DECLARE IT ANYWHERE ELSE!
extern LevelSelectSystem level_select_system;

extern 	bool selector_clicked;
extern float selector_mouse_y;