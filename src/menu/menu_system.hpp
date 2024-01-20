#pragma once
///////////////
//  imports  //
/////////////// 
#include <render_system.hpp>
#include <tiny_ecs_registry.hpp>
#include <phase/phase_system.hpp>
#include <level/level_system.hpp>
#include <level_select/level_select_system.hpp>


///////////////
// constants //
///////////////

///////////////
//   class   //
/////////////// 


class MenuSystem {
public:
	// Constructor
	MenuSystem() {

	}

	// Initialize the menu system. Should only be called at the end of the intro cutscene.
	void MenuSystem::init(RenderSystem* renderer_arg);

	// Toggle open/close the menu
	void MenuSystem::toggle_menu();

	// Handles the button clicks on the menu
	void MenuSystem::button_click_handler(Entity entity);


private:
	// Toggle open the settings ui
	void MenuSystem::open_settings_ui();

	// Toggle close the settings ui
	void MenuSystem::close_settings_ui();


	// Create the menu button on the top left
	Entity MenuSystem::create_open_menu_button(RenderSystem* renderer_arg);

	// Create the menu ui
	Entity MenuSystem::create_menu_ui(RenderSystem* renderer_arg);

	// Create the reset data button
	Entity MenuSystem::create_reset_data_button(RenderSystem* renderer_arg);
	// Create the exit level button
	Entity MenuSystem::create_exit_level_button(RenderSystem* renderer_arg);
	// Create the settings button
	Entity MenuSystem::create_settings_button(RenderSystem* renderer_arg);
	// Create the close menu button
	Entity MenuSystem::create_close_menu_button(RenderSystem* renderer_arg);

	// Create the settings ui
	Entity MenuSystem::create_settings_ui(RenderSystem* renderer_arg);

	// Create the settings back button
	Entity MenuSystem::create_settings_back_button(RenderSystem* renderer_arg);

	// Create the icon buttons, may need the y-value as well.
	Entity MenuSystem::create_icon_button(TEXTURE_ASSET_ID texture_id, RenderSystem* renderer_arg, int ypos);


	Entity open_menu_button;
	Entity menu_ui;
	Entity reset_data_button;
	Entity exit_level_button;
	Entity settings_button;
	Entity close_menu_button;

	Entity settings_ui;
	Entity settings_back_button;
	Entity music_enabled_icon;
	Entity music_disabled_icon;
	Entity sfx_enabled_icon;
	Entity sfx_disabled_icon;
	Entity diff_1_icon;
	Entity diff_2_icon;
	Entity diff_3_icon;

	// Game state
	RenderSystem* renderer;
	AISystem* ai_system;
	Audio_system* audio_player;
};

// Declaration of menu system. DON'T DECLARE IT ANYWHERE ELSE!
extern MenuSystem menu_system;

// A variable to keep track if the menu is open 
// Intitially set to false. If true, cannot click anything other than menu
extern bool menu_opened;

extern bool is_icon_pressed;