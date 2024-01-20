#include <phase/intro_system.hpp>

bool is_intro_complete = false;


void IntroSystem::init(RenderSystem* renderer_arg) {
	current_scene = INTRO;
	this->renderer = renderer_arg;

	total_elapsed_time = 0;
	window = create_window(renderer_arg);
	ground = create_ground(renderer_arg);
	goblin = create_running_goblin(renderer_arg);
	logo = create_logo(renderer_arg);
	//Audio_system* audio_player = Audio_system::get_audio();
	//audio_player->play_intro();
	// ground = createLineGround({ 800, 500}, { 1500, 1 }, 0.f);
}

void IntroSystem::end_intro(RenderSystem* renderer_arg) {
	// Untoggle the pause set at the beginning. 
	phase_system.toggle_pause();

	// remove the renderables
	registry.remove_all_components_of(window);
	registry.remove_all_components_of(goblin);
	registry.remove_all_components_of(ground);
	registry.remove_all_components_of(logo);

	level_select_system.init(renderer_arg);
	menu_system.init(renderer_arg);
}

Entity IntroSystem::create_logo(RenderSystem* renderer) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::LOGO_TEXT);
	registry.meshPtrs.emplace(entity, &mesh);

	logo_mesh = &mesh;

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = { WINDOW_WIDTH / 2, -100.f * aspect_ratio_scale.y };
	motion.scale = mesh.original_size * 2000.f;
	motion.scale.x *= -1; // point front to the right


	//for testing proper mesh collision stuff (also change mesh path from logo.obj -> L.obj in render_system.hpp)
	//motion.angle = 2.1; 

	// Create and (empty) Salmon component to be able to refer to all turtles
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::LOGO_TEXT,
			GEOMETRY_BUFFER_ID::LOGO_TEXT,
			"CUTSCENEOBJECT1" });

	return entity;
}


Entity IntroSystem::create_window(RenderSystem* renderer) {
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 });

	// Setting initial scale values
	motion.scale = vec2({ WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2 });

	// Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::INTRO_WINDOW,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"CUTSCENEBG"
		}
	);

	// cutscene elements should not move
	registry.uiComponents.insert(entity, {});

	return entity;
}

Entity IntroSystem::create_ground(RenderSystem* renderer) {
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2({ WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 5) * 4 });

	// Setting initial scale values
	//motion.scale = vec2({ window_width_px * 2 , (window_height_px)  });
	motion.scale = vec2({ WINDOW_WIDTH , WINDOW_HEIGHT / 5 * 2 });

	// Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::GROUND,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"CUTSCENEBG"
		}
	);

	// cutscene elements should not move
	registry.uiComponents.emplace(entity);

	return entity;
}

Entity IntroSystem::create_running_goblin(RenderSystem* renderer) {
	Entity entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE_ANIMATION);
	registry.meshPtrs.emplace(entity, &mesh);

	logo_mesh = &mesh;

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = START_ANGLE;

	motion.position = vec2({ START_X, START_Y });

	// Setting initial scale values
	motion.scale = vec2({ GOBLIN_WIDTH, GOBLIN_HEIGHT });

	Animation& animations = registry.animations.emplace(entity);

	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 3;
	animations.ms_per_frame = 150;

	animations.animation_type = 0;

	animations.time_elapsed_ms = 0;
	animations.repeating = true;

	// Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::RUNNING_GOBLIN,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"CUTSCENEOBJECT2"
		}
	);

	// running goblin should animate during pause
	registry.animations_ignore_pause.emplace(entity);

	// cutscene elements should not move
	registry.uiComponents.emplace(entity);

	return entity;
}


void IntroSystem::text_step(Motion& motion) {

	float y_increment = y_velocity / 1000.f;
	motion.position.y += y_increment;

	if (y_velocity > 0) {
		y_velocity *= ACCELERATION;
	}
	if (y_velocity < 0) {
		y_velocity *= DECELERATION;
	}

	//if (PhysicsSystem::mesh_collides_y(*logo_mesh, logo, COLLISION_BOUND)) {
	if (PhysicsSystem::mesh_collides_ground(*logo_mesh, logo, ground)) {
		

		//y_velocity = 0;

		//snap back to ground level in case overshoot
		//motion.position.y = COLLISION_BOUND - (motion.scale.y / 2);

		// check if needs to bounce up
		if (y_velocity > 1000.f) {
			y_velocity = -y_velocity;
		}
		else {
			y_velocity = 0;
		}
		


	}
	else if (y_velocity < 0.f && y_velocity > -1000.f) { // check if reaches peak, must fall back down 
		y_velocity = -y_velocity;

	}
}

void IntroSystem::step(float elapsed_ms_since_last_update, RenderSystem* renderer_arg) {
	//eh

	if (!is_intro_complete) {


		// Calculate the time elapsed within the entire animation sequence
		total_elapsed_time += elapsed_ms_since_last_update;

		// text logo movement (mesh collision)
		Motion& logo_motion = registry.motions.get(logo);

		text_step(logo_motion);

		// goblin movement (keyframe interp)	
		// Get the goblin's motion and animations components
		Motion& goblin_motion = registry.motions.get(goblin);
		Animation& animations = registry.animations.get(goblin);

		if (goblin_motion.position.x < END_X) { // The goblin is running

			goblin_motion.position.x += (END_X - START_X) * (elapsed_ms_since_last_update / x_change);
			if (goblin_motion.position.x > JUMP_START_X) { // goblin trips

				// set up falling animation if not already set
				if (animations.animation_type == 0) {
					// goblin_motion.position.y -= 30;
					animations.animation_type = 1;
					animations.curr_frame = 0; // reset frames
					animations.repeating = false;
					animations.time_elapsed_ms = 0;
					animations.ms_per_frame = 200;
				}


				// interpolate y pos 
				if (goblin_motion.position.x < JUMP_MID_X) { //upwards
					goblin_motion.position.y += (MID_Y - START_Y) * (elapsed_ms_since_last_update / y_change);
				}
				else if (goblin_motion.position.y < END_Y) { //downwards
					goblin_motion.position.y += (END_Y - MID_Y) * (elapsed_ms_since_last_update / y_change);
				}
				else { //sliding
					x_change += 250;
				}

				//interpolate angle
				if (curr_angle < END_ANGLE) {
					curr_angle += ANGLE_INCREMENT;
					goblin_motion.angle += ANGLE_INCREMENT;
					//goblin_motion.angle += (END_ANGLE * (elapsed_ms_since_last_update / 10000.f));
				}

			}
		}
		// TEMPORARY CHECK FOR END OF CUTSCENE
		// REPLACE THIS WITH AN ACTUAL CHECK
		if (total_elapsed_time >= 0 && !is_intro_complete) { //6000
			is_intro_complete = true;
			end_intro(renderer_arg);
		}

	}

}


