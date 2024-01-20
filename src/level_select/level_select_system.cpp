#include <level_select/level_select_system.hpp>
#include <phase/intro_system.hpp>
#include <level/level_system.hpp>
#include "../audio/audio_system.hpp"
#include <cutscene/cutscene_system.hpp>

// Declaration of level_select system. DON'T DECLARE IT ANYWHERE ELSE!
LevelSelectSystem level_select_system;

bool selector_clicked = false;
float selector_mouse_y = 0;


const vec2 ACTIVE_WINDOW_POSITION = vec2({ WINDOW_WIDTH - 150, WINDOW_HEIGHT / 2 + 60 });
const vec2 ACTIVE_WINDOW_VELOCITY = vec2({ -6000, 0 });

const vec2 INACTIVE_WINDOW_POSITION = ACTIVE_WINDOW_POSITION + vec2({700, 0});
const vec2 INACTIVE_WINDOW_VELOCITY = vec2({ 6000, 0 });

const vec2 BUTTON_OFFSET = vec2(-120, 250);

const int NUM_BG = 4;
const int BG_OFFSET = 60;
const int BG_SPEED_Y = 60;
const int BG_HEIGHT = 1.5*WINDOW_HEIGHT;

const int BG_Y_CHANGE = 150;
const float BG_Y_SPEED = 10;
const float PARALLAX_RENDER_UPDATE_TIME = 10.0;

// handle parallax and background movement
float target_camera_y = 0.0;
float camera_y_change = 0.0;
float parallax_time_remaining = PARALLAX_RENDER_UPDATE_TIME;

// Add to this list when adding new levels!
const TEXTURE_ASSET_ID popup_window_list[8] = {
	TEXTURE_ASSET_ID::LSM_POPUP_TUTORIAL,
	TEXTURE_ASSET_ID::LSM_POPUP_ONE,
	TEXTURE_ASSET_ID::LSM_POPUP_TWO,
	TEXTURE_ASSET_ID::LSM_POPUP_THREE,
	TEXTURE_ASSET_ID::LSM_POPUP_FOUR,
	TEXTURE_ASSET_ID::LSM_POPUP_FIVE,
	TEXTURE_ASSET_ID::LSM_POPUP_SIX,
	TEXTURE_ASSET_ID::LSM_POPUP_SEVEN,
};

// Add to this list when adding new levels!
const TEXTURE_ASSET_ID lsm_button_list[8] = {
	TEXTURE_ASSET_ID::LSM_BUTTON_TUTORIAL,
	TEXTURE_ASSET_ID::LSM_BUTTON_ONE,
	TEXTURE_ASSET_ID::LSM_BUTTON_TWO,
	TEXTURE_ASSET_ID::LSM_BUTTON_THREE,
	TEXTURE_ASSET_ID::LSM_BUTTON_FOUR,
	TEXTURE_ASSET_ID::LSM_BUTTON_FIVE,
	TEXTURE_ASSET_ID::LSM_BUTTON_SIX,
	TEXTURE_ASSET_ID::LSM_BUTTON_SEVEN,
};


void LevelSelectSystem::init(RenderSystem* renderer_arg) {
	// reset the camera position for parallax
	reset_camera_position();
	target_camera_y = 0.0;
	camera_y_change = 0.0;
	parallax_time_remaining = PARALLAX_RENDER_UPDATE_TIME;

	// Set the current scene
	current_scene = LEVEL_SELECT;

	create_background(renderer_arg);

	create_level_selector(renderer_arg);

	popup_window_one = create_popup_window(renderer_arg);
	popup_window_two = create_popup_window(renderer_arg);

	start_button_one = create_start_button(renderer_arg, popup_window_one);
	start_button_two = create_start_button(renderer_arg, popup_window_two);

	level_select_text = create_level_select_text(renderer_arg);

	create_little_guy(renderer_arg);

	audio_player = Audio_system::get_audio();
	// Set one of the popup windows to tutorial
	LSMPopup& popup = registry.lsmPopups.get(popup_window_one);
	popup.is_active = true;
	RenderRequest& rr = registry.renderRequests.get(popup_window_one);
	rr.used_texture = TEXTURE_ASSET_ID::LSM_POPUP_TUTORIAL;

	current_level = &tutorial_level;
	level_focus = LEVEL_TUTORIAL;
}

void LevelSelectSystem::handle_level_selection(RenderSystem* renderer_arg, int level) {

	// Now we just set the window popups!
	if (level_system.is_level_unlocked(level)) {
		handle_parallax(renderer_arg, level);

		for (Entity entity : registry.lsmPopups.entities) {
			LSMPopup& popup = registry.lsmPopups.get(entity);

			// Change active if level is different
			if (level_focus != level) {

				// Flip the is_active!
				popup.is_active = !popup.is_active;
			}

			// Set the current popup window's texture
			if (popup.is_active) {
				RenderRequest& rr = registry.renderRequests.get(entity);
				rr.used_texture = popup_window_list[level];
			}
		}

		level_focus = level;

		// update lilguy's position
		Motion& level_select_button_motion = registry.motions.get(registry.lsmButtons.entities[level_focus]);
		Motion& lil_motion = registry.motions.get(little_guy);
		lil_motion.position.y = level_select_button_motion.position.y;
	}

}

void LevelSelectSystem::step(float elapsed_ms_since_last_update) {
	for (Entity entity : registry.lsmPopups.entities) {
		LSMPopup& popup = registry.lsmPopups.get(entity);
		Motion& motion = registry.motions.get(entity);
		if (popup.is_active) {
			// Check that it's in correct position (on the screen)
			if (motion.position.x <= ACTIVE_WINDOW_POSITION.x) {
				motion.position = ACTIVE_WINDOW_POSITION;
				motion.velocity.x = 0;
			}
			else {
				motion.velocity = ACTIVE_WINDOW_VELOCITY;
			}
		}
		else {
			// Check that it's in the correct position (off the screen)
			if (motion.position.x >= INACTIVE_WINDOW_POSITION.x) {
				motion.position = INACTIVE_WINDOW_POSITION;
				motion.velocity.x = 0;
			}
			else {
				motion.velocity = INACTIVE_WINDOW_VELOCITY;
			}
		}

		if (entity == popup_window_one) {
			Motion& button_motion = registry.motions.get(start_button_one);
			button_motion.position = motion.position + BUTTON_OFFSET;
		}

		if (entity == popup_window_two) {
			Motion& button_motion = registry.motions.get(start_button_two);
			button_motion.position = motion.position + BUTTON_OFFSET;
		}
	}

	// handles backgrounds:
	float camera_pos_y = get_camera_matrix().y;

	if ((camera_y_change > 0.0 && camera_pos_y > target_camera_y) || 
		(camera_y_change < 0.0 && camera_pos_y < target_camera_y)) {
		// check that it's not bigger than target y value
		if (camera_pos_y > target_camera_y) {
			camera_y_change = 0.0;
			update_camera_position(get_camera_matrix().x, target_camera_y);
		}
	}
	else if (camera_y_change != 0.0) {
		// move if timer changes some:
		parallax_time_remaining -= elapsed_ms_since_last_update;
		if (parallax_time_remaining <= 0.0) {

			if (camera_y_change > 0) {
				update_camera_position(get_camera_matrix().x, camera_pos_y + BG_Y_SPEED);
			}
			else {
				update_camera_position(get_camera_matrix().x, camera_pos_y - BG_Y_SPEED);
			}

			parallax_time_remaining = PARALLAX_RENDER_UPDATE_TIME;
		}
	}


	// handles backgrounds
	//for (float i = 0; i < registry.lsmBackgrounds.entities.size(); i++) {
	//	Entity entity = registry.lsmBackgrounds.entities[i];
	//	Motion& motion = registry.motions.get(entity);

	//	if (motion.velocity.y > 0) {
	//		// check that it's not bigger than the designated y value
	//		float new_pos_y = BG_HEIGHT - (level_focus * BG_OFFSET) * (i + 1);
	//		if (motion.position.y > new_pos_y) {
	//			motion.velocity.y = 0;
	//			motion.position.y = new_pos_y;
	//		}
	//	}

	//	if (motion.velocity.y < 0) {
	//		// check that it's not smaller than the designated y value
	//		float new_pos_y = BG_HEIGHT - (level_focus * BG_OFFSET) * (i + 1);
	//		if (motion.position.y < new_pos_y) {
	//			motion.velocity.y = 0;
	//			motion.position.y = new_pos_y;
	//		}
	//	}
	//}
}

void LevelSelectSystem::confirm_selection(RenderSystem* renderer_arg) {

	if (level_system.is_level_unlocked(level_focus)) {
		teardown_lsm();
		

		// BEGIN THE CHOSEN LEVEL! REMOVE IT FROM WORLD SYSTEM
		// level_system.begin_level(renderer_arg, level_focus);
		cutscene_system.play(level_focus);

	} else {
		std::cout << "Level is locked!" << std::endl;
	}
	audio_player->play_intro();
}

void LevelSelectSystem::teardown_lsm()
{
	// remove the renderables
	registry.remove_all_components_of(bg_1);
	registry.remove_all_components_of(bg_2);
	registry.remove_all_components_of(bg_3);
	registry.remove_all_components_of(bg_4);

	registry.remove_all_components_of(popup_window_one);
	registry.remove_all_components_of(popup_window_two);
	registry.remove_all_components_of(start_button_one);
	registry.remove_all_components_of(start_button_two);

	registry.remove_all_components_of(level_select_text);

	registry.remove_all_components_of(little_guy);

	registry.remove_all_components_of(selector_bounding_box);

	// remove the buttons
	for (Entity entity : registry.lsmButtons.entities) {
		registry.remove_all_components_of(entity);
	}

	reset_camera_position();
}



void LevelSelectSystem::create_background(RenderSystem* renderer) {
	Entity bg_list[NUM_BG] = { bg_1, bg_2, bg_3, bg_4 };
	for (int i = 0; i < NUM_BG; i++) {
		Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
		registry.meshPtrs.emplace(bg_list[i], &mesh);


		// Initialize the motion
		auto& motion = registry.motions.emplace(bg_list[i]);
		motion.angle = 0.f;
		motion.velocity = { 0.f, 0.f };
		motion.position = vec2({ WINDOW_WIDTH / 2, BG_HEIGHT - 20 });

		// Setting initial scale values
		motion.scale = vec2({ WINDOW_WIDTH + 5, WINDOW_HEIGHT * 3 });


		// Set render request values
		TEXTURE_ASSET_ID bg_texture;
		switch (i) {
			case 0:
				bg_texture = TEXTURE_ASSET_ID::LSM_BG_1;
				break;
			case 1:
				bg_texture = TEXTURE_ASSET_ID::LSM_BG_2;
				break;
			case 2:
				bg_texture = TEXTURE_ASSET_ID::LSM_BG_3;
				break;
			case 3:
				bg_texture = TEXTURE_ASSET_ID::LSM_BG_4;
				break;
		}

		EFFECT_ASSET_ID bg_shader;
		switch (i) {
			case 0:
				bg_shader = EFFECT_ASSET_ID::BG0;
				break;
			case 1:
				bg_shader = EFFECT_ASSET_ID::BG1;
				break;
			case 2:
				bg_shader = EFFECT_ASSET_ID::BG2;
				break;
			case 3:
				bg_shader = EFFECT_ASSET_ID::BG3;
				break;
		}

		registry.renderRequests.insert(
			bg_list[i], {
				bg_texture,
				bg_shader,
				GEOMETRY_BUFFER_ID::SPRITE,
				layers[6 + i]
			}
		);

		// registry.uiComponents.insert(bg_list[i], {});

		// flag for lsm background entity
		registry.lsmBackgrounds.insert(bg_list[i], {});
	}
}


void LevelSelectSystem::create_level_selector(RenderSystem* renderer) {
	// create the selector bounding box
	selector_bounding_box = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(selector_bounding_box, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(selector_bounding_box);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2(WINDOW_WIDTH/2 - 300, WINDOW_HEIGHT/2 + 62);

	// Setting initial scale values
	motion.scale = vec2({ 393, 595 });

	// Set render request values depending on level is unlocked?
	//registry.renderRequests.insert(
	//	selector_bounding_box, {
	//		TEXTURE_ASSET_ID::LSM_SELECTOR_TESTING,
	//		EFFECT_ASSET_ID::TEXTURED,
	//		GEOMETRY_BUFFER_ID::SPRITE,
	//		"TIMER"
	//	}
	//);

	// bounding box should not move
	registry.uiComponents.insert(selector_bounding_box, {});

	// flag for selector bounding box
	registry.lsmSelectors.insert(selector_bounding_box, {});



	// Iterate over all levels and create their buttons
	for (int i = 0; i < registry.levels.entities.size(); i++) {
		Entity level_ent = registry.levels.entities[i];
		Level& level_comp = registry.levels.get(level_ent);

		// The button itself is an entity
		Entity entity = Entity();

		Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
		registry.meshPtrs.emplace(entity, &mesh);

		// Initialize the motion
		auto& motion = registry.motions.emplace(entity);
		motion.angle = 0.f;
		motion.velocity = { 0.f, 0.f };
		motion.position = vec2({ WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 + 125*(i - 1.5) });

		// Setting initial scale values
		motion.scale = vec2({ 400, 100 });

		enum class EFFECT_ASSET_ID effect_used = level_comp.is_unlocked ? EFFECT_ASSET_ID::LSM_TEXTURED : EFFECT_ASSET_ID::LSM_GREYED;
		// Set render request values depending on level is unlocked?
		registry.renderRequests.insert(
			entity, {
				lsm_button_list[i],
				effect_used,
				GEOMETRY_BUFFER_ID::SPRITE,
				"PRIORITY"
			}
		);

		// cutscene elements should not move
		registry.uiComponents.insert(entity, {});

		registry.lsmButtons.insert(entity, {});
	}
}

Entity LevelSelectSystem::create_popup_window(RenderSystem* renderer) {
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = INACTIVE_WINDOW_POSITION;

	// Setting initial scale values
	motion.scale = vec2({ WINDOW_WIDTH / 1.3, WINDOW_HEIGHT / 1.3 });

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::LSM_POPUP,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"PRIORITY"
		}
	);

	// cutscene elements should not move
	registry.uiComponents.insert(entity, {});

	// set popup component (is_active should be false to begin with)
	registry.lsmPopups.insert(entity, { false });

	return entity;
}

Entity LevelSelectSystem::create_start_button(RenderSystem* renderer, Entity parent) {
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	auto& parent_motion = registry.motions.get(parent);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = parent_motion.position + BUTTON_OFFSET;

	// Setting initial scale values
	motion.scale = vec2({ WINDOW_WIDTH / 6, WINDOW_HEIGHT / 6 - 25});

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::LSM_START_BUTTON,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"PRIORITY_TEXT"
		}
	);

	// cutscene elements should not move
	registry.uiComponents.insert(entity, {});

	// start button flag
	registry.lsmStartButtons.insert(entity, {});

	return entity;
}


void LevelSelectSystem::handle_parallax(RenderSystem* renderer_arg, int level) {
	// check to see if target level and current level focus
	camera_y_change = (level_focus - level) * BG_Y_CHANGE;
	target_camera_y += camera_y_change;
}

Entity LevelSelectSystem::create_level_select_text(RenderSystem* renderer_arg) {
	Entity entity = Entity();

	Mesh& mesh = renderer_arg->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2(320, 90);

	// Setting initial scale values
	motion.scale = vec2({ 443, 94 });

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::LSM_LEVEL_SELECT,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"PRIORITY_TEXT"
		}
	);

	// cutscene elements should not move
	registry.uiComponents.insert(entity, {});

	return entity;
}

void LevelSelectSystem::create_little_guy(RenderSystem* renderer_arg) {
	little_guy = Entity();

	Mesh& mesh = renderer_arg->getMesh(GEOMETRY_BUFFER_ID::SPRITE_ANIMATION);
	registry.meshPtrs.emplace(little_guy, &mesh);

	// Initialize the animations
	Animation& animations = registry.animations.emplace(little_guy);
	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 4;
	animations.time_elapsed_ms = 0;
	animations.repeating = true;

	// Initialize the motion
	auto& motion = registry.motions.emplace(little_guy);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2({ WINDOW_WIDTH / 4 + 135, WINDOW_HEIGHT / 2 - 193 });

	// Setting initial scale values
	motion.scale = vec2(300/7.f, 60.f);

	//Set render request values
	registry.renderRequests.insert(
		little_guy, {
			TEXTURE_ASSET_ID::LILGUY,
			EFFECT_ASSET_ID::LSM_LITTLE_GUY,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"CUTSCENEBG"
		}
	);

	// cutscene elements should not move
	registry.uiComponents.insert(little_guy, {});

	registry.lsmLittleGuy.insert(little_guy, {});
}
