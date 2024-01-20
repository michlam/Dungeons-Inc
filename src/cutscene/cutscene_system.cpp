#include <cutscene/cutscene_system.hpp>
#include <cutscene/cutscene_loader.hpp>
#include <dialogue/dialogue_system.hpp>
#include <level/level_system.hpp>
#include <menu/menu_system.hpp>

CutsceneSystem cutscene_system;

// flags to see when to process the next action
bool play_next_cutscene_event = false;
bool is_cutscene_done = true;
bool accepting_mouse_input = false;
bool in_transition = false;

// hehe
int goblin_count = 0;
int goblin_tick = 5;

// current cutscene to process
int current_cutscene_event_index = -1;

// action to perform after cutscene is over
// generally a level should be loaded, but if it's -1 then load level select menu
int level_to_load = -1;


// init cutscene system with render system argument
// just in case tbh
void CutsceneSystem::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;
	cutscene_loader.init(renderer_arg);
}


// start playing the given cutscene
void CutsceneSystem::play(int cutscene_to_play) {
	// reset the cutscene system to default settings
	reset_system();

	// set level to load, if there is one
	level_to_load = cutscene_to_play;

	// load cutscene data from json file
	cutscene_loader.load_cutscene(cutscene_to_play);

	// set up state for cutscene
	current_scene = CUTSCENE;
	play_next_cutscene_event = true;
	is_cutscene_done = false;

	// set up dialogue system
	dialogue_system.init(this->renderer);
	dialogue_system.set_cutscene_settings();

	// at this point, all necessary cutscene events are in the registry
	// but just to double-check:
	// if (registry.cutscene_events.size() > 0) {
	// 	is_cutscene_done = true;
	// }

	// add screen brightening

	if (registry.cutscene_events.size() > 0) {
		in_transition = true;

		assert(registry.screenStates.components.size() <= 1);
		ScreenState& screen = registry.screenStates.components[0];
		screen.screen_darken_factor = 1.0f;
		screen.is_darken = false;
	}
}


// step through the cutscene system
// step is only ever called when the current scene is CUTSCENE so we don't need to worry about exit condition
// figure out if we need to start on the next cutscene event or mark the cutscene as done
void CutsceneSystem::step(float elapsed_ms_since_last_update) {
	if (in_transition) {
		// process screen darken factor
		assert(registry.screenStates.components.size() <= 1);
		ScreenState& screen = registry.screenStates.components[0];

		if (screen.ticks == 0) {
			if (screen.screen_darken_factor < 1.f && screen.is_darken == true) {
				screen.screen_darken_factor += 0.1;

				if (screen.screen_darken_factor >= 1.0f) {
					screen.screen_darken_factor = 1.0f;
					in_transition = false;
				}
			}
			else if (screen.screen_darken_factor > 0.f && screen.is_darken == false) {
				screen.screen_darken_factor -= 0.1;

				if (screen.screen_darken_factor <= 0.0f) {
					screen.screen_darken_factor = 0.0f;
					in_transition = false;
				}

			}
			screen.ticks = 25;
		}

		--screen.ticks;
	}
	else if (play_next_cutscene_event) {
		// play next cutscene but stop mouse input
		accepting_mouse_input = false;
		play_next_cutscene_event = false;
		++current_cutscene_event_index;

		// reset some stuff
		remove_emotions();

		// check if there are any more events to run
		// if not, end system
		if (current_cutscene_event_index >= registry.cutscene_events.size()) {
			is_cutscene_done = true;
			
			if (registry.cutscene_events.size() > 0) {
				// add screen darkening
				in_transition = true;

				assert(registry.screenStates.components.size() <= 1);
				ScreenState& screen = registry.screenStates.components[0];
				screen.screen_darken_factor = 0.f;
				screen.is_darken = true;
			}
		}

		// else start loading next event
		else {
			// determine which choice was selected, if there was a choice
			int selected_choice_index = 0;

			for (int i = 0; i < registry.cutscene_choices.components.size(); i++) {
				CutsceneChoice current_choice = registry.cutscene_choices.components[i];

				if (current_choice.is_selected) {
					selected_choice_index = i;
				}
			}

			// load next cutscene event
			CutsceneEvent current_event = registry.cutscene_events.components[current_cutscene_event_index];
			Entity current_character = registry.cutscene_events.entities[current_cutscene_event_index];
			
			if (current_event.actions.size() != 0 && current_event.dialogue.size() != 0) {
				// apply action data
				Action current_action = current_event.actions[selected_choice_index];

				Motion& current_motion = registry.motions.get(current_character);
				current_motion = current_action.motion;

				Movement& current_movement = registry.movements.has(current_character) ?
					registry.movements.get(current_character) :
					registry.movements.emplace(current_character);
				current_movement = current_action.movement;

				add_emotion(current_character, current_action.emotion);

				// apply dialogue data
				std::string current_dialogue = current_event.dialogue[selected_choice_index];
				dialogue_system.make_character_say(current_character, current_dialogue);
			}
		}

		// erase any dialogue choices if they were there
		for (Entity choice : registry.cutscene_choices.entities) {
			registry.remove_all_components_of(choice);
		}
	}

	// if no cutscene event to play, check if cutscene done
	// if done, then begin next step
	else if (is_cutscene_done) {
		reset_system();

		if (level_to_load >= 0 && level_to_load <= 7) {
			level_system.begin_level(this->renderer, level_to_load);
		}
		else {
			level_select_system.init(this->renderer);
		}
	}

	// else, cutscene is NOT done but no new event to play next, meaning waiting for current event to finish
	// or waiting for user input to proceed to next event
	else {
		if (dialogue_system.is_complete()) {
			accepting_mouse_input = true;

			// check if the cutscene has choices or not. if has choices, render them
			// else, render advancement sign

			CutsceneEvent current_event = registry.cutscene_events.components[current_cutscene_event_index];

			if (current_event.choices.size() > 0) {
				render_cutscene_choices();
			}
			else {
				dialogue_system.show_advancement_sign();
			}
		}
		else {
			accepting_mouse_input = false;
			dialogue_system.step(elapsed_ms_since_last_update);
		}

		spawn_goblins();
	}
}


// ummmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// ignore thx
void CutsceneSystem::spawn_goblins() {
	// minor ignore im so sorry
	Entity current_event_entity = registry.cutscene_events.entities[current_cutscene_event_index];
	if (registry.cutscene_elements.get(current_event_entity).name == "GOBLINS") {
		if (goblin_count < 75 && goblin_tick <= 0) {
			std::random_device rand_x;
			std::mt19937 gen_x(rand_x());
			std::uniform_int_distribution<int> distribution_x(915, default_window_size.x);
			int random_x = distribution_x(gen_x);

			std::random_device rand_y;
			std::mt19937 gen_y(rand_y());
			std::uniform_int_distribution<int> distribution_y(400, default_window_size.y - 150);
			int random_y = distribution_y(gen_y);

			Entity goblin_mob = createCutsceneMob(this->renderer, MobClass::BARBARIAN);

			Motion& goblin_motion = registry.motions.emplace(goblin_mob);
			goblin_motion.position = { default_window_size.x + 50, random_y };
			goblin_motion.scale = { 80, 100 };

			CutsceneElement& goblin_element = registry.cutscene_elements.emplace(goblin_mob);
			goblin_element.name = "GOBLINS";

			Movement& goblin_movement = registry.movements.emplace(goblin_mob);
			goblin_movement.goal_pos = { random_x, random_y };

			goblin_count++;

			std::random_device rand;
			std::mt19937 gen(rand());
			std::uniform_int_distribution<int> distribution(2, 25);
			int random = distribution(gen);

			goblin_tick = random;
		}
		else {
			goblin_tick--;
		}
	}
}


// handles advancing the cutscene event to the next one, if there is one
void CutsceneSystem::handle_cutscene_advancement(GLFWwindow* window, int key, int action, int mod) {
	// first, we need to check if we're accepting mouse input or not
	// as cutscene might not be done rendering
	if (accepting_mouse_input) {
		if (key == GLFW_MOUSE_BUTTON_LEFT && !menu_opened) {
			if (action == GLFW_PRESS) {
				// fetch mouse cursor position and sprite motion data
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);

				if (registry.cutscene_choices.size() > 0) {
					handle_choice_advance(xpos, ypos);
				}
				else {
					handle_dialogue_advance(xpos, ypos);
				}
			}
		}
	}
}


// handles mouse input when there are no cutscene choices
void CutsceneSystem::handle_dialogue_advance(double xpos, double ypos) {
	// get speech bubble entity and its motion info
	Entity speech_bubble = dialogue_system.speech_bubble;
	Motion& motion = registry.motions.get(speech_bubble);
	float speech_bubble_xpos = motion.position.x * aspect_ratio_scale.x;
	float speech_bubble_ypos = motion.position.y * aspect_ratio_scale.y;
	float speech_bubble_width = abs(motion.scale.x) * aspect_ratio_scale.x;
	float speech_bubble_height = abs(motion.scale.y) * aspect_ratio_scale.y;

	// find speech bubble's bounding box
	float x_left_bound = speech_bubble_xpos - (speech_bubble_width / 2);
	float x_right_bound = speech_bubble_xpos + (speech_bubble_width / 2);
	float y_up_bound = speech_bubble_ypos - (speech_bubble_height / 2);
	float y_down_bound = speech_bubble_ypos + (speech_bubble_height / 2);

	if ((x_left_bound <= xpos) && (xpos <= x_right_bound) && (y_up_bound <= ypos) && (ypos <= y_down_bound)) {
		play_next_cutscene_event = true;
		dialogue_system.hide_advancement_sign();
	}
}


// handles mouse input when there are cutscene choices
void CutsceneSystem::handle_choice_advance(double xpos, double ypos) {
	CutsceneEvent current_event = registry.cutscene_events.components[current_cutscene_event_index];

	for (Entity entity : registry.cutscene_choices.entities) {
		Motion& motion = registry.motions.get(entity);
		float choice_bubble_xpos = motion.position.x * aspect_ratio_scale.x;
		float choice_bubble_ypos = motion.position.y * aspect_ratio_scale.y;
		float choice_bubble_width = abs(motion.scale.x) * aspect_ratio_scale.x;
		float choice_bubble_height = abs(motion.scale.y) * aspect_ratio_scale.y;

		// find speech bubble's bounding box
		float x_left_bound = choice_bubble_xpos - (choice_bubble_width / 2);
		float x_right_bound = choice_bubble_xpos + (choice_bubble_width / 2);
		float y_up_bound = choice_bubble_ypos - (choice_bubble_height / 2);
		float y_down_bound = choice_bubble_ypos + (choice_bubble_height / 2);

		if ((x_left_bound <= xpos) && (xpos <= x_right_bound) && (y_up_bound <= ypos) && (ypos <= y_down_bound)) {
			play_next_cutscene_event = true;
			CutsceneChoice& choice = registry.cutscene_choices.get(entity);
			choice.is_selected = true;
			break;
		}
	}
}


// reset the cutscene system's values and current list of cutscene events
void CutsceneSystem::reset_system() {
	// reset flags
	play_next_cutscene_event = false;
	is_cutscene_done = true;
	current_cutscene_event_index = -1;
	goblin_tick = 5;
	goblin_count = 0;

	// reset dialogue system
	dialogue_system.reset_dialogue_system();

	// remove all cutscene events as we don't need them anymore
	registry.cutscene_events.clear();

	// remove all cutscene elements and their associated components
	for (Entity entity : registry.cutscene_elements.entities) {
		registry.remove_all_components_of(entity);
	}

	// reset screen brightness
	assert(registry.screenStates.components.size() <= 1);
	ScreenState& screen = registry.screenStates.components[0];
	screen.screen_darken_factor = 0.f;
	screen.is_darken = false;
}


// renders all cutscene choices
void CutsceneSystem::render_cutscene_choices() {
	// load current cutscene event
	CutsceneEvent current_event = registry.cutscene_events.components[current_cutscene_event_index];

	// check if there are dialogue options to load
	// if there is, then load them
	// also make sure to not re-load choices that already exist
	if (current_event.choices.size() > 0 && registry.cutscene_choices.size() == 0) {

		vec2 bottom_position = { 825 + 130, default_window_size.y - 175 - 56 };

		float y_pos = bottom_position.y - (60 * current_event.choices.size());
		vec2 current_position = { bottom_position.x, y_pos };

		// for each dialogue choice, load a text box for it
		for (std::string choice : current_event.choices) {
			current_position.y += 60;
			create_cutscene_choice(choice, current_position);
		}
	}
}


// renders a bubble for a given choice
void CutsceneSystem::create_cutscene_choice(std::string choice, vec2 position) {
	Entity choice_entity = Entity();

	// add entity to cutscene elements for easy removal
	// and then also cutscene options for quick retrieval
	CutsceneElement& choice_element = registry.cutscene_elements.emplace(choice_entity);
	choice_element.name = choice;
	registry.cutscene_choices.emplace(choice_entity);

	// render it
	registry.renderRequests.insert(
		choice_entity, {
			TEXTURE_ASSET_ID::CUTSCENE_CHOICE_BUBBLE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"CHARACTER_PORTRAIT",
		}
	);

	// add motion data
	Motion& choice_motion = registry.motions.emplace(choice_entity);
	choice_motion.scale = { 350, 50 };
	choice_motion.position = position;

	// add text component
	Text text = Text{};
	text.text_lines[0] = choice;
	text.colour = { 0.0, 0.0, 0.0 };
	text.scale = 1.20f;
	text.position = { position.x / 2 + 22, position.y / 1.6 + 4 };
	registry.texts.insert(choice_entity, text);
}


// adds an emotive particle above a given entity
void CutsceneSystem::add_emotion(Entity entity, std::string emotion) {
	// Motion motion = registry.motions.get(entity);
	// vec2 entity_position = motion.position;

	if (emotion == "LOVE") {
		create_love_effect(entity);
	}
	else if (emotion == "ANGER") {
		create_anger_effect(entity);
	}
	else if (emotion == "CONFUSION") {
		create_confusion_effect(entity);
	}
	else if (emotion == "SURPRISE") {
		create_surprise_effect(entity);
	} 
	else if (emotion == "NONE") {
		// don't do anything
	}
	else {
		assert("BAD CUTSCENE EFFECT");
	}
}

void CutsceneSystem::remove_emotions() {
	for (Entity entity : registry.cutscene_effects.entities) {
		registry.remove_all_components_of(entity);
	}
}