#include <menu/menu_system.hpp>
#include "../ai_system.hpp"
#include "../audio/audio_system.hpp"
#include "cutscene/cutscene_system.hpp"

// Constants
const int MENU_UI_ONSCREEN_X = 600;
const int MENU_UI_OFFSCREEN_X = 4000;

const int TOP_BUTTON_ONSCREEN_Y = 275;
const int MID_BUTTON_ONSCREEN_Y = 400;
const int BOT_BUTTON_ONSCREEN_Y = 525;


const int SETTINGS_UI_ONSCREEN_X = 600;
const int SETTINGS_UI_OFFSCREEN_X = 5000;

const int ACTIVE_SETTINGS_BUTTON_OFFSCREEN_X = 5000;
const int INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X = 6000;
const int SETTINGS_BUTTON_ONSCREEN_X = 715;

const int TOP_SETTINGS_BUTTON_ONSCREEN_Y = 310;
const int MID_SETTINGS_BUTTON_ONSCREEN_Y = 385;
const int BOT_SETTINGS_BUTTON_ONSCREEN_Y = 460;

// A variable to keep track if the menu is open 
// Intitially set to false. If true, cannot click anything other than menu
bool menu_opened = false;

// A variable to keep track if an icon has been pressed
bool is_icon_pressed = false;


// Declaration of menu system. DON'T DECLARE IT ANYWHERE ELSE!
MenuSystem menu_system;

void MenuSystem::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;
	this->ai_system = AISystem::get_ai_system();
	this->audio_player = Audio_system::get_audio();

	// Create the menu button on the top left. 
	open_menu_button = create_open_menu_button(renderer_arg);

	// Create the menu ui
	menu_ui = create_menu_ui(renderer_arg);

	// Create the reset data button
	reset_data_button = create_reset_data_button(renderer_arg);

	// Create the exit level button
	exit_level_button = create_exit_level_button(renderer_arg);

	// Create the settings button
	settings_button = create_settings_button(renderer_arg);

	// Create the close menu button
	close_menu_button = create_close_menu_button(renderer_arg);


	// Create the settings ui
	settings_ui = create_settings_ui(renderer_arg);

	// Create the settings back button
	settings_back_button = create_settings_back_button(renderer_arg);

	// Create the icon buttons
	music_enabled_icon = create_icon_button(TEXTURE_ASSET_ID::MUSIC_ENABLED_ICON, renderer_arg, TOP_SETTINGS_BUTTON_ONSCREEN_Y);
	music_disabled_icon = create_icon_button(TEXTURE_ASSET_ID::MUSIC_DISABLED_ICON, renderer_arg, TOP_SETTINGS_BUTTON_ONSCREEN_Y);

	sfx_enabled_icon = create_icon_button(TEXTURE_ASSET_ID::SFX_ENABLED_ICON, renderer_arg, MID_SETTINGS_BUTTON_ONSCREEN_Y);
	sfx_disabled_icon = create_icon_button(TEXTURE_ASSET_ID::SFX_DISABLED_ICON, renderer_arg, MID_SETTINGS_BUTTON_ONSCREEN_Y);

	diff_1_icon = create_icon_button(TEXTURE_ASSET_ID::DIFF_1_ICON, renderer_arg, BOT_SETTINGS_BUTTON_ONSCREEN_Y);
	diff_2_icon = create_icon_button(TEXTURE_ASSET_ID::DIFF_2_ICON, renderer_arg, BOT_SETTINGS_BUTTON_ONSCREEN_Y);
	diff_3_icon = create_icon_button(TEXTURE_ASSET_ID::DIFF_3_ICON, renderer_arg, BOT_SETTINGS_BUTTON_ONSCREEN_Y);

	// SET ACTIVE AND INACTIVE OFFSCREEN X!
	/*
	
	Motion& motion1 = registry.motions.get(music_disabled_icon);
	motion1.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;

	Motion& motion2 = registry.motions.get(sfx_disabled_icon);
	motion2.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;

	Motion& motion3 = registry.motions.get(diff_2_icon);
	motion3.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;

	Motion& motion4 = registry.motions.get(diff_3_icon);
	motion4.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;

	*/
	
	bool bgm_status = this->audio_player->get_bgm_status();
	bool sfx_status = this->audio_player->get_sound_effect_status();
	AISystem::Difficulty diff = this->ai_system->get_difficulty();


	if (bgm_status == true) {
		Motion& motion1 = registry.motions.get(music_disabled_icon);
		motion1.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
	}
	else {
		Motion& motion1 = registry.motions.get(music_enabled_icon);
		motion1.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
	}


	if (sfx_status == true) {
		Motion& motion2 = registry.motions.get(sfx_disabled_icon);
		motion2.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
	}
	else {
		Motion& motion2 = registry.motions.get(sfx_enabled_icon);
		motion2.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
	}

	if (diff == AISystem::EASY) {
		Motion& motion3 = registry.motions.get(diff_2_icon);
		motion3.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;

		Motion& motion4 = registry.motions.get(diff_3_icon);
		motion4.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
	}
	else if (diff == AISystem::MEDIUM) {
		Motion& motion3 = registry.motions.get(diff_1_icon);
		motion3.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;

		Motion& motion4 = registry.motions.get(diff_3_icon);
		motion4.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
	}
	else {
		Motion& motion3 = registry.motions.get(diff_1_icon);
		motion3.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;

		Motion& motion4 = registry.motions.get(diff_2_icon);
		motion4.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
	}
}

void MenuSystem::toggle_menu()
{
	printf("here \n");

	// Pause game if in game
	if (current_scene == IN_GAME && !is_paused) {
		phase_system.toggle_pause();
	}

	for (Entity entity : registry.menuUis.entities) {

		// Check for scene and correct button
		if ((current_scene == LEVEL_SELECT && entity != exit_level_button) || ((current_scene == IN_GAME || current_scene == IN_FULL_MAP_VIEW || current_scene == CUTSCENE) && entity != reset_data_button)) {
			// Toggle menu by manipulating its position
			Motion& motion = registry.motions.get(entity);

			if (motion.position.x == MENU_UI_OFFSCREEN_X) {
				motion.position.x = MENU_UI_ONSCREEN_X;
			}
			else {
				motion.position.x = MENU_UI_OFFSCREEN_X;
			}
		}
	}


	// If menu is open, you cannot click anything else
	menu_opened = !menu_opened;
	std::cout << "Menu is open: " << menu_opened << std::endl;
}

void MenuSystem::button_click_handler(Entity entity) {
	// Call specific things depending on the entity clicked
	if (entity == close_menu_button) {
		toggle_menu();
	} else if (entity == reset_data_button) {
		level_select_system.teardown_lsm();
		utils::update_level(1);
		level_system.update_unlocked_levels();
		level_select_system.init(renderer);
	}
	else if (entity == exit_level_button) {
		// exit the level.
		phase_system.delete_phase();
		phase_system.delete_cards();
		phase_system.delete_map();
		phase_system.delete_characters();
		adventurers.clear();
		level_system.removeVictoryScreen();
		dialogue_system.reset_dialogue_system();
		cutscene_system.reset_system();

		level_select_system.init(renderer);

		// move menu back off the screen
		toggle_menu();
		is_paused = false;

		Motion& motion1 = registry.motions.get(exit_level_button);
		motion1.position.x = MENU_UI_OFFSCREEN_X;

		Motion& motion2 = registry.motions.get(reset_data_button);
		motion2.position.x = MENU_UI_OFFSCREEN_X;
	} else if (entity == settings_button) {
		open_settings_ui();
	}
	else if (entity == settings_back_button) {
		utils::update_AI_config(this->ai_system->get_difficulty());
		utils::update_audio_config(this->audio_player->get_bgm_status(), this->audio_player->get_sound_effect_status());

		close_settings_ui();
	}
	else if (entity == music_enabled_icon && !is_icon_pressed) {
		is_icon_pressed = true;

		this->audio_player->set_bgm(false);

		Motion& motion1 = registry.motions.get(music_enabled_icon);
		Motion& motion2 = registry.motions.get(music_disabled_icon);

		motion1.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
		motion2.position.x = SETTINGS_BUTTON_ONSCREEN_X;
	}
	else if (entity == music_disabled_icon && !is_icon_pressed) {
		is_icon_pressed = true;

		this->audio_player->set_bgm(true);

		Motion& motion1 = registry.motions.get(music_disabled_icon);
		Motion& motion2 = registry.motions.get(music_enabled_icon);

		motion1.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
		motion2.position.x = SETTINGS_BUTTON_ONSCREEN_X;
	}
	else if (entity == sfx_enabled_icon && !is_icon_pressed) {
		is_icon_pressed = true;

		this->audio_player->set_sound_effect(false);

		Motion& motion1 = registry.motions.get(sfx_enabled_icon);
		Motion& motion2 = registry.motions.get(sfx_disabled_icon);

		motion1.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
		motion2.position.x = SETTINGS_BUTTON_ONSCREEN_X;
	} 
	else if (entity == sfx_disabled_icon && !is_icon_pressed) {
		is_icon_pressed = true;

		this->audio_player->set_sound_effect(true);

		Motion& motion1 = registry.motions.get(sfx_disabled_icon);
		Motion& motion2 = registry.motions.get(sfx_enabled_icon);

		motion1.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
		motion2.position.x = SETTINGS_BUTTON_ONSCREEN_X;
	}
	else if (entity == diff_1_icon && !is_icon_pressed) {
		is_icon_pressed = true;

		this->ai_system->set_difficulty(AISystem::MEDIUM);

		Motion& motion1 = registry.motions.get(diff_1_icon);
		Motion& motion2 = registry.motions.get(diff_2_icon);

		motion1.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
		motion2.position.x = SETTINGS_BUTTON_ONSCREEN_X;
	}
	else if (entity == diff_2_icon && !is_icon_pressed) {
		is_icon_pressed = true;

		this->ai_system->set_difficulty(AISystem::HARD);

		Motion& motion1 = registry.motions.get(diff_2_icon);
		Motion& motion2 = registry.motions.get(diff_3_icon);

		motion1.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
		motion2.position.x = SETTINGS_BUTTON_ONSCREEN_X;
	}
	else if (entity == diff_3_icon && !is_icon_pressed) {
		is_icon_pressed = true;

		this->ai_system->set_difficulty(AISystem::EASY);

		Motion& motion1 = registry.motions.get(diff_3_icon);
		Motion& motion2 = registry.motions.get(diff_1_icon);

		motion1.position.x = INACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
		motion2.position.x = SETTINGS_BUTTON_ONSCREEN_X;
	}
}

void MenuSystem::open_settings_ui()
{
	// open the settings if closed
	Motion& settings_ui_motion = registry.motions.get(settings_ui);
	if (settings_ui_motion.position.x == SETTINGS_UI_OFFSCREEN_X) {
		// move the menu stuff off the screen
		for (Entity entity : registry.menuUis.entities) {
			Motion& motion = registry.motions.get(entity);
			motion.position.x = MENU_UI_OFFSCREEN_X;
		}

		// move the settings stuff onto the screen
		for (Entity entity : registry.settingsUI.entities) {
			Motion& motion = registry.motions.get(entity);

			if (registry.settingsIcons.has(entity)) {
				// check if it's active
				if (motion.position.x == ACTIVE_SETTINGS_BUTTON_OFFSCREEN_X) {
					motion.position.x = SETTINGS_BUTTON_ONSCREEN_X;
				}
			}
			else {
				motion.position.x = SETTINGS_UI_ONSCREEN_X;
			}
		}
	}
}

void MenuSystem::close_settings_ui()
{
	// close the settings if opened
	Motion& settings_ui_motion = registry.motions.get(settings_ui);
	if (settings_ui_motion.position.x == SETTINGS_UI_ONSCREEN_X) {
		// move the settings stuff off the screen
		for (Entity entity : registry.settingsUI.entities) {
			Motion& motion = registry.motions.get(entity);

			if (registry.settingsIcons.has(entity)) {
				// check if it's active
				if (motion.position.x == SETTINGS_BUTTON_ONSCREEN_X) {
					motion.position.x = ACTIVE_SETTINGS_BUTTON_OFFSCREEN_X;
				}
			}
			else {
				motion.position.x = SETTINGS_UI_OFFSCREEN_X;
			}
		}

		// move the menu stuff back on the screen
		for (Entity entity : registry.menuUis.entities) {
			// need some checks for reset data and exit level buttons
			if (current_scene == LEVEL_SELECT && entity != exit_level_button || ((current_scene == IN_GAME || current_scene == IN_FULL_MAP_VIEW) && entity != reset_data_button)) {
				Motion& motion = registry.motions.get(entity);
				motion.position.x = MENU_UI_ONSCREEN_X;
			}
		}
	}
}

Entity MenuSystem::create_open_menu_button(RenderSystem* renderer)
{
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2(45, 45);

	// Setting initial scale values
	motion.scale = vec2(60, 60);

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::OPEN_MENU_BUTTON,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"MENU BUTTON"
		}
	);

	// menu elements should not move
	registry.uiComponents.insert(entity, {});

	// menu button flag
	registry.openMenuButton.insert(entity, {});

	return entity;
}

Entity MenuSystem::create_menu_ui(RenderSystem* renderer)
{
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2(MENU_UI_OFFSCREEN_X, 400);

	// Setting initial scale values
	motion.scale = vec2(1200, 800);

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::MENU_UI,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"MENU UI 1"
		}
	);

	// menu elements should not move
	registry.uiComponents.insert(entity, {});

	registry.menuUis.insert(entity, {});

	return entity;
}

Entity MenuSystem::create_reset_data_button(RenderSystem* renderer)
{
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2(MENU_UI_OFFSCREEN_X, TOP_BUTTON_ONSCREEN_Y);

	// Setting initial scale values
	motion.scale = vec2(275, 89);

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::RESET_DATA_BUTTON,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"MENU UI 2"
		}
	);

	// menu elements should not move
	registry.uiComponents.insert(entity, {});

	registry.menuUis.insert(entity, {});

	registry.menuButtons.insert(entity, {});

	return entity;
}

Entity MenuSystem::create_exit_level_button(RenderSystem* renderer)
{
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2(MENU_UI_OFFSCREEN_X, TOP_BUTTON_ONSCREEN_Y);

	// Setting initial scale values
	motion.scale = vec2(275, 89);

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::EXIT_LEVEL_BUTTON,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"MENU UI 2"
		}
	);

	// menu elements should not move
	registry.uiComponents.insert(entity, {});

	registry.menuUis.insert(entity, {});

	registry.menuButtons.insert(entity, {});

	return entity;
}

Entity MenuSystem::create_settings_button(RenderSystem* renderer)
{
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2(MENU_UI_OFFSCREEN_X, MID_BUTTON_ONSCREEN_Y);

	// Setting initial scale values
	motion.scale = vec2(275, 89);

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::MENU_SETTINGS_BUTTON,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"MENU UI 2"
		}
	);

	// menu elements should not move
	registry.uiComponents.insert(entity, {});

	registry.menuUis.insert(entity, {});

	registry.menuButtons.insert(entity, {});

	return entity;
}

Entity MenuSystem::create_close_menu_button(RenderSystem* renderer)
{
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2(MENU_UI_OFFSCREEN_X, BOT_BUTTON_ONSCREEN_Y);

	// Setting initial scale values
	motion.scale = vec2(275, 89);

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::CLOSE_MENU_BUTTON,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"MENU UI 2"
		}
	);

	// menu elements should not move
	registry.uiComponents.insert(entity, {});

	registry.menuUis.insert(entity, {});

	registry.menuButtons.insert(entity, {});

	return entity;
}

Entity MenuSystem::create_settings_ui(RenderSystem* renderer_arg) {
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2(SETTINGS_UI_OFFSCREEN_X, 400);

	// Setting initial scale values
	motion.scale = vec2(1200, 800);

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::SETTINGS_UI,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"MENU UI 1"
		}
	);

	// menu elements should not move
	registry.uiComponents.insert(entity, {});

	// Settings UI 
	registry.settingsUI.insert(entity, {});

	return entity;
}

Entity MenuSystem::create_settings_back_button(RenderSystem* renderer_arg)
{
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2(SETTINGS_UI_OFFSCREEN_X, BOT_BUTTON_ONSCREEN_Y + 30);

	// Setting initial scale values
	motion.scale = vec2(275, 89);

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::SETTINGS_BACK_BUTTON,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"MENU UI 2"
		}
	);

	// menu elements should not move
	registry.uiComponents.insert(entity, {});

	registry.settingsUI.insert(entity, {});

	registry.settingsButtons.insert(entity, {});

	return entity;
}

Entity MenuSystem::create_icon_button(TEXTURE_ASSET_ID texture_id, RenderSystem* renderer_arg, int ypos) {
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2(ACTIVE_SETTINGS_BUTTON_OFFSCREEN_X, ypos);

	// Setting initial scale values
	motion.scale = vec2(40, 40);

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			texture_id,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"MENU UI 2"
		}
	);

	// menu elements should not move
	registry.uiComponents.insert(entity, {});

	registry.settingsUI.insert(entity, {});
	registry.settingsButtons.insert(entity, {});
	registry.settingsIcons.insert(entity, {});

	return entity;
}
