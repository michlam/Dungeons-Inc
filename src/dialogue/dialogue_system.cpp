#include <dialogue/dialogue_system.hpp>
#include <random>


DialogueSystem dialogue_system;


// constant values for tutorial speech bubbles
TEXTURE_ASSET_ID TUTORIAL_SPEECH_BUBBLE = TEXTURE_ASSET_ID::TUTORIAL_SPEECH_BUBBLE;

const vec2 TUTORIAL_DIMENSIONS = { 200, 250 };
const vec2 TUTORIAL_POSITION = { 15 + 100, default_window_size.y - 175 - 15 };

const int TUTORIAL_PADDING = 6;
const vec2 TUTORIAL_TEXT_POSITION = { TUTORIAL_POSITION.x - 95, TUTORIAL_POSITION.y - 350 };
const float TUTORIAL_FONT_SIZE = 1.0;

const int TUTORIAL_CHARACTER_LIMIT = 19;


// constant values for cutscene speech bubbles
TEXTURE_ASSET_ID CUTSCENE_SPEECH_BUBBLE = TEXTURE_ASSET_ID::CUTSCENE_SPEECH_BUBBLE;

const vec2 CUTSCENE_DIMENSIONS = { 825, 175 };
const vec2 CUTSCENE_POSITION = { 15 + 100 + 30 + 20, default_window_size.y - (175 / 2) - 20 };

const int CUTSCENE_PADDING = -20;
const vec2 CUTSCENE_TEXT_POSITION = { CUTSCENE_POSITION.x + CUTSCENE_PADDING, CUTSCENE_POSITION.y - 275 };
const float CUTSCENE_FONT_SIZE = 1.15;

const int CUTSCENE_CHARACTER_LIMIT = 60;


// constant values for standard speech bubbles
TEXTURE_ASSET_ID REGULAR_SPEECH_BUBBLE = TEXTURE_ASSET_ID::SPEECH_BUBBLE;

const vec2 DIMENSIONS = { 200, 125 };
const vec2 POSITION = { 15 + 100, default_window_size.y - 175 - 15 };

const int PADDING = 6;
const vec2 TEXT_POSITION = { POSITION.x - 93, POSITION.y - 254 };
const float FONT_SIZE = 0.9;

const int CHARACTER_LIMIT = 23;


// current speech bubble dimensions
TEXTURE_ASSET_ID SPEECH_BUBBLE_TEXTURE = REGULAR_SPEECH_BUBBLE;
vec2 SPEECH_BUBBLE_DIMENSIONS = DIMENSIONS;
vec2 SPEECH_BUBBLE_POSITION = POSITION;
vec2 SPEECH_BUBBLE_TEXT_POSITION = TEXT_POSITION;
float SPEECH_BUBBLE_FONT_SIZE = FONT_SIZE;
int SPEECH_BUBBLE_PADDING = PADDING;
int MAX_CHARACTERS_PER_LINE = CHARACTER_LIMIT;


// reading speed = 4 words/sec, or 0.25 sec/word
float READING_SPEED = 250.f;


// stores a list of messages that need to be displayed on the current speech bubble
std::vector<std::string> messages_queue;


// stores a list of each line of the current speech bubble to display
std::vector<std::string> speech_lines;


// text rendering speed
int text_rendering_speed = 1;


// info about the current message to display
int current_line_index = 0;
int current_letter_index = 0;
int text_timer = 0;
float bubble_timer = 0.f;


// flag to check if there's any dialogue currently playing
bool is_dialogue_complete = true;


// all current player quotes
// to be loaded via json later
std::map<phase_type, std::vector<std::string>> player_quotes;


// initialize the dialogue system
void DialogueSystem::init(RenderSystem* renderer_arg) {
	// just in case...
	reset_dialogue_system();

	// add renderer field
	renderer = renderer_arg;

	// update quotes
	player_quotes = utils::load_dialogue_data();

	// create and render assets
	// just do player for now
	create_player_sprite();
	create_character_portrait();
}


// step through the dialogue system
// basically figure out if it needs to continue rendering a speech bubble, type the next letter
// start decrementing the bubble's existence timer, set the next message to be renderd
// OR delete the speech bubble
void DialogueSystem::step(float elapsed_ms_since_last_update) {
	// quick check to make sure dialogue system has been initialized
	// assert(registry.players.has(player) && "Dialogue System has not been initialized yet!");

	// only perform the following steps if there's a dialogue that needs to be updated
	if (registry.motions.has(speech_bubble)) {
		// update speech bubble. render one letter at a time
		// only expand bubble if timer is greater than 0.f,
		// ie. we still have time to render the bubble
		if ((bubble_timer > 0.f) && (registry.motions.get(speech_bubble).scale != SPEECH_BUBBLE_DIMENSIONS)) {
			render_speech_bubble(elapsed_ms_since_last_update);
		}
		// render character name IF it's a cutscene speech bubble
		else if (!registry.players.has(character_name_bubble) && SPEECH_BUBBLE_TEXTURE == CUTSCENE_SPEECH_BUBBLE) {
			render_character_name(character);
		}
		// start typing text after expanding speech bubble
		// depending on the text_timer, only updates the text based on current text speed
		else if (current_line_index < speech_lines.size()) {
			type_text();
		}
		// start decreasing timer to remove speech bubble
		else if (bubble_timer > 0.f) {
			bubble_timer -= elapsed_ms_since_last_update;
		}
		// if the timer has hit 0, the speech bubble needs to begin the removal process
		else if (bubble_timer <= 0.f) {
			if (messages_queue.empty()) {
				if (current_scene != CUTSCENE) {
					// temp. speech bubble doesn't get unrendered during cutscenes
					remove_speech_bubble(elapsed_ms_since_last_update);
				}
				else {
					// set flag
					is_dialogue_complete = true;
				}
			}
			else {
				remove_speech_bubble_text();

				std::string next_message = messages_queue.front();
				set_current_text(next_message);
				messages_queue.erase(messages_queue.begin());

				printf(next_message.c_str());
				printf("\n");
;			}
		}
	}

	// little guy animation should automatically update by itself
}


// set regular dialogue bubble settings
void DialogueSystem::set_standard_settings() {
	SPEECH_BUBBLE_TEXTURE = REGULAR_SPEECH_BUBBLE;
	SPEECH_BUBBLE_DIMENSIONS = DIMENSIONS;
	SPEECH_BUBBLE_POSITION = POSITION;
	SPEECH_BUBBLE_TEXT_POSITION = TEXT_POSITION;
	SPEECH_BUBBLE_FONT_SIZE = FONT_SIZE;
	SPEECH_BUBBLE_PADDING = PADDING;
	MAX_CHARACTERS_PER_LINE = CHARACTER_LIMIT;
}


// set cutscene dialogue bubble settings
void DialogueSystem::set_cutscene_settings() {
	SPEECH_BUBBLE_TEXTURE = CUTSCENE_SPEECH_BUBBLE;
	SPEECH_BUBBLE_DIMENSIONS = CUTSCENE_DIMENSIONS;
	SPEECH_BUBBLE_POSITION = CUTSCENE_POSITION;
	SPEECH_BUBBLE_TEXT_POSITION = CUTSCENE_TEXT_POSITION;
	SPEECH_BUBBLE_FONT_SIZE = CUTSCENE_FONT_SIZE;
	SPEECH_BUBBLE_PADDING = CUTSCENE_PADDING;
	MAX_CHARACTERS_PER_LINE = CUTSCENE_CHARACTER_LIMIT;
}


// set tutorial dialogue bubble settings
void DialogueSystem::set_tutorial_settings() {
	SPEECH_BUBBLE_TEXTURE = TUTORIAL_SPEECH_BUBBLE;
	SPEECH_BUBBLE_DIMENSIONS = TUTORIAL_DIMENSIONS;
	SPEECH_BUBBLE_POSITION = TUTORIAL_POSITION;
	SPEECH_BUBBLE_TEXT_POSITION = TUTORIAL_TEXT_POSITION;
	SPEECH_BUBBLE_FONT_SIZE = TUTORIAL_FONT_SIZE;
	SPEECH_BUBBLE_PADDING = TUTORIAL_PADDING;
	MAX_CHARACTERS_PER_LINE = TUTORIAL_CHARACTER_LIMIT;
}


// returns the status of the dialogue system completion flag
bool DialogueSystem::is_complete() {
	return is_dialogue_complete;
}


// interrupt whatever the player was saying before (if they were saying something before)
// and make them say something else instead
void DialogueSystem::make_player_interrupt(std::string msg) {
	// interrupt the current dialogue to give priority to the player
	messages_queue.clear();
	remove_speech_bubble_text();

	// render speech bubble
	set_speech_bubble(msg);
}


void DialogueSystem::make_character_say(Entity entity, std::string msg) {
	// quick check to make sure dialogue system has been initialized
	// assert(registry.players.has(player) && "Dialogue System has not been initialized yet!");

	// printf("making player say something... \n");

	std::string current_talking_entity = registry.cutscene_elements.has(character) ? registry.cutscene_elements.get(character).name : "ERROR_14";
	std::string new_talking_entity = registry.cutscene_elements.get(entity).name;

	if (!speech_lines.empty()) {
		is_dialogue_complete = false;
		messages_queue.push_back(msg);

		if (!(current_talking_entity == new_talking_entity)) {
			registry.remove_all_components_of(character);
			Text& name_to_update = registry.texts.get(character_name_bubble);
			name_to_update.text_lines[0] = new_talking_entity;
			create_character_sprite(entity);
		}
	}
	else {

		// printf("NOT adding to queue... \n");

		// reset dialogue system to give full dialogue controls to the player right now
		reset_dialogue_system();

		// render player assets
		create_character_sprite(entity);
		create_character_portrait();

		// render speech bubble
		set_speech_bubble(msg);
	}
}


// make the player sprite say something
void DialogueSystem::make_player_say(std::string msg) {
	// quick check to make sure dialogue system has been initialized
	assert(registry.players.has(player) && "Dialogue System has not been initialized yet!");

	// printf("making player say something... \n");

	if (!speech_lines.empty()) {
		is_dialogue_complete = false;
		// printf("adding to queue... \n");
		messages_queue.push_back(msg);
	}
	else {

		// printf("NOT adding to queue... \n");

		// reset dialogue system to give full dialogue controls to the player right now
		reset_dialogue_system();

		// render player assets
		create_player_sprite();
		create_character_portrait();

		// render speech bubble
		set_speech_bubble(msg);
	}
}


// interrupts the current quote to say the phase quote
void DialogueSystem::make_player_say_phase_quote(phase_type phase) {
	std::vector<std::string> quotes = player_quotes[phase];

	if (quotes.size() > 0) {
		std::random_device rand;
		std::mt19937 gen(rand());
		std::uniform_int_distribution<int> distribution(0, (quotes.size() - 1));

		int random_index = distribution(gen);

		// printf("Size: %i \n", quotes.size());
		// printf("Random Index: %i \n", random_index);

		std::string selected_message = quotes[random_index];
		make_player_interrupt(selected_message);
	}
}


Entity DialogueSystem::create_character_sprite(Entity entity) {
	// set render and animation of little guy
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE_ANIMATION);
	if (!registry.meshPtrs.has(character)) registry.meshPtrs.emplace(character, &mesh);
	TEXTURE_ASSET_ID character_texture = registry.renderRequests.get(entity).used_texture;

	if (!registry.renderRequests.has(character)) {
		registry.renderRequests.insert(
			character, {
				character_texture,
				EFFECT_ASSET_ID::TEXTURED_ANIMATION,
				GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
				"CHARACTER_PORTRAIT",
			}
		);
	}
	else {
		RenderRequest& render_request = registry.renderRequests.get(character);
		render_request.used_texture = character_texture;
	}

	if (registry.animations.has(entity)) {
		Animation& animations_new = registry.animations.has(character) ? registry.animations.get(character) : registry.animations.emplace(character);
		const Animation& animations_old = registry.animations.get(entity);

		animations_new.sprite_h = animations_old.sprite_h;
		animations_new.sprite_w = animations_old.sprite_w;

		animations_new.curr_frame = animations_old.curr_frame;
		animations_new.num_frames[0] = animations_old.num_frames[0];
		animations_new.num_frames[1] = animations_old.num_frames[1];
		animations_new.time_elapsed_ms = animations_old.time_elapsed_ms;
		animations_new.repeating = animations_old.repeating;
	}

	// set scale and position of player
	Motion& character_motion = registry.motions.has(character) ? registry.motions.get(character) : registry.motions.emplace(character);
	character_motion.position = { (150 / 2) + 10, default_window_size.y - (200 / 2) };
	character_motion.scale = vec2((10.f / 6.f) * (200.f / 7.f), (10.f / 6.f) * 40.f);

	// add ui stickiness
	registry.uiComponents.emplace(character);

	// player sprite ignores pause
	registry.animations_ignore_pause.emplace(character);

	if (current_scene == CUTSCENE) {
		CutsceneElement& element = registry.cutscene_elements.has(character) ? registry.cutscene_elements.get(character) : registry.cutscene_elements.emplace(character);
		element.name = registry.cutscene_elements.get(entity).name;
	}

	return character;
}


// render the player sprite
Entity DialogueSystem::create_player_sprite() {
	// create player sprite entity
	registry.players.emplace(player);

	// set render and animation of little guy
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE_ANIMATION);
	registry.meshPtrs.emplace(player, &mesh);
	registry.renderRequests.insert(
		player, {
			TEXTURE_ASSET_ID::LILGUY,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"CHARACTER_PORTRAIT",
		}
	);

	Animation& animations = registry.animations.emplace(player);
	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 4;
	animations.time_elapsed_ms = 0;
	animations.repeating = true;

	// set scale and position of player
	Motion& player_motion = registry.motions.emplace(player);
	player_motion.position = { (150 / 2) + 10, default_window_size.y - (200 / 2)};
	player_motion.scale = vec2((10.f / 6.f) * (200.f / 7.f), (10.f / 6.f) * 40.f);

	// add ui stickiness
	registry.uiComponents.emplace(player);

	// player sprite ignores pause
	registry.animations_ignore_pause.emplace(player);

	return player;
}


// render the portrait background
Entity DialogueSystem::create_character_portrait() {
	// set render and position of background portrait
	registry.players.emplace(portrait);
	registry.renderRequests.insert(
		portrait, {
			TEXTURE_ASSET_ID::PORTRAIT_WINDOW,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"PORTRAIT",
		}
	);

	Motion& portrait_motion = registry.motions.emplace(portrait);
	portrait_motion.position = { (150 / 2) + 10, default_window_size.y - (200 / 2) };
	portrait_motion.scale = { 100, 125 };

	// add ui stickiness
	registry.uiComponents.emplace(portrait);

	return portrait;
}


// render the speech bubble
Entity DialogueSystem::create_speech_bubble() {
	// temporarily storing player-related entities in players
	registry.players.emplace(speech_bubble);

	// set render and position of speech bubble
	registry.renderRequests.insert(
		speech_bubble, {
			SPEECH_BUBBLE_TEXTURE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"SPEECH_BUBBLE",
		}
	);

	Motion& portrait_motion = registry.motions.emplace(speech_bubble);
	portrait_motion.position = { 15, default_window_size.y - 200 - (SPEECH_BUBBLE_DIMENSIONS.y / 2) - 20 };
	portrait_motion.scale = { 0, SPEECH_BUBBLE_DIMENSIONS.y };

	// add text component
	Text text = Text{};
	text.text_lines[0] = "";
	text.position = { SPEECH_BUBBLE_PADDING, portrait_motion.position.y - 50 };
	text.colour = { 0.0, 0.0, 0.0 };
	text.scale = 1.f;
	registry.texts.insert(speech_bubble, text);

	// add ui stickiness
	registry.uiComponents.emplace(speech_bubble);

	return speech_bubble;
}


// render the entire speech bubble, including any text
void DialogueSystem::set_speech_bubble(std::string msg) {
	// cannot set an empty msg
	assert(msg.size() > 0 && "Cannot set an empty message!");

	// set flag for dialogue in progress
	is_dialogue_complete = false;

	// create speech bubble
	if (!registry.players.has(speech_bubble)) {
		create_speech_bubble();
	}

	// update text/motion data of speech bubble if necessary
	Motion& speech_bubble_motion = registry.motions.get(speech_bubble);
	speech_bubble_motion.position = SPEECH_BUBBLE_POSITION;

	if (SPEECH_BUBBLE_TEXTURE == CUTSCENE_SPEECH_BUBBLE) {
		speech_bubble_motion.scale = { 0, SPEECH_BUBBLE_DIMENSIONS.y };
	}
	else {
		speech_bubble_motion.scale = { SPEECH_BUBBLE_DIMENSIONS.x, 0 };
	}

	Text& speech_bubble_text = registry.texts.get(speech_bubble);
	speech_bubble_text.position = SPEECH_BUBBLE_TEXT_POSITION;
	speech_bubble_text.scale = SPEECH_BUBBLE_FONT_SIZE;

	// set character name bubble
	// only render it for cutscene speech bubbles
	/*if (!registry.players.has(character_name_bubble) && 
		SPEECH_BUBBLE_TEXTURE == CUTSCENE_SPEECH_BUBBLE) {
		render_character_name(player);
	}*/

	// set the speech bubble's text
	set_current_text(msg);
}


void DialogueSystem::set_current_text(std::string msg) {
	// calculate the time needed to display the message
	// based off of average human reading speed of 4 words/sec
	// there is always at least 1 word in a given msg
	int num_of_words = 1;
	int pos_of_last_space = -1;
	int curr_starting_pos = 0;
	int curr_num_of_characters = 0;

	for (int i = 0; i < msg.size(); i++) {
		// keep track of current line length
		char c = msg[i];
		curr_num_of_characters++;

		// if current character is space, note this down
		// and also increment word count (because words are separated by spaces)
		if (c == ' ') {
			num_of_words++;
			pos_of_last_space = i;
		}

		// if current line is too long, start the splitting and appending process
		// ie. finish current line and start on new one
		if (curr_num_of_characters > MAX_CHARACTERS_PER_LINE) {
			// find previous word to end on
			if (pos_of_last_space == -1) { pos_of_last_space = MAX_CHARACTERS_PER_LINE; }
			int pos_to_substr_on = pos_of_last_space - curr_starting_pos;

			// get the finalized line
			std::string curr_line = msg.substr(curr_starting_pos, pos_to_substr_on);

			// add it to the queue
			speech_lines.push_back(curr_line);

			// debugging
			/*printf("Current Message: %s \n", curr_line.c_str());
			printf("Current Number of Characters: %i \n", curr_num_of_characters);
			printf("Current Starting Position: %i \n", curr_starting_pos);
			printf("Position of Last Space: %i \n\n", pos_of_last_space);*/

			// set the current number of characters to include any characters after the last white space
			// because those will be put on the next line
			curr_num_of_characters = i - pos_of_last_space;

			// current starting position is after the whitespace, so add 1
			curr_starting_pos = pos_of_last_space + 1;

			// reset last whitespace
			pos_of_last_space = -1;
		}
	}

	// handles the last part of the string that may or may not have been appended to the list
	// (it's only correctly appended if the last line fits perfectly)
	if (curr_starting_pos < msg.size()) {
		int pos_to_substr_on = msg.size() - curr_starting_pos;
		std::string curr_line = msg.substr(curr_starting_pos, pos_to_substr_on);
		speech_lines.push_back(curr_line);
	}

	// speed up timer in cutscenes
	if (current_scene == CUTSCENE) {
		bubble_timer = 1000.f;
	}
	else {
		bubble_timer = READING_SPEED * num_of_words;
	}

	text_timer = text_rendering_speed;
}

// shrink the speech bubble in width, aka make it disappear
void DialogueSystem::remove_speech_bubble(float elapsed_ms_since_last_update) {
	remove_speech_bubble_text();

	Motion& speech_bubble_motion = registry.motions.get(speech_bubble);

	if (SPEECH_BUBBLE_TEXTURE == CUTSCENE_SPEECH_BUBBLE) {
		float new_width = speech_bubble_motion.scale.x - (SPEECH_BUBBLE_DIMENSIONS.x * (elapsed_ms_since_last_update / 300.f));

		speech_bubble_motion.position.x = (150 + 10) + 20 + max(0.f, new_width / 2);
		speech_bubble_motion.scale.x = max(0.f, new_width);
	}
	else {
		float new_height = speech_bubble_motion.scale.y - (SPEECH_BUBBLE_DIMENSIONS.y * (elapsed_ms_since_last_update / 300.f));

		speech_bubble_motion.position.y = SPEECH_BUBBLE_POSITION.y - max(0.f, new_height / 2);
		speech_bubble_motion.scale.y = max(0.f, new_height);
	}

	// reached end of speech bubble
	if (speech_bubble_motion.scale.x == 0 || speech_bubble_motion.scale.y == 0) {
		is_dialogue_complete = true;
		registry.remove_all_components_of(speech_bubble);
	}
}


// reset the speech bubble's text so that a new message can be displayed
// specifically, resets each line to empty. it's not actually removing the text component
void DialogueSystem::remove_speech_bubble_text() {
	if (registry.texts.has(speech_bubble)) {
		Text& text = registry.texts.get(speech_bubble);
		for (std::string& line : text.text_lines) {
			line = "";
		}
	}

	reset_dialogue_timers();
}


// reset the dialogue system's timers and counters such that a new message can begin being displayed
void DialogueSystem::reset_dialogue_timers() {
	// reset dialogue system fields to set up for next set of values
	speech_lines.clear();
	current_line_index = 0;
	current_letter_index = 0;
	text_timer = 0;
	bubble_timer = 0.f;
}


// reset the dialogue system's settings to default (null)
void DialogueSystem::reset_dialogue_system() {
	// reset all related values of the dialogue system
	registry.remove_all_components_of(player);
	registry.remove_all_components_of(character);
	registry.remove_all_components_of(portrait);
	registry.remove_all_components_of(speech_bubble);
	registry.remove_all_components_of(character_name_bubble);
	hide_advancement_sign();
	messages_queue.clear();

	// reset dialogue system fields to set up for next set of values
	reset_dialogue_timers();
}


// grow the speech bubble in width, aka make it appear
void DialogueSystem::render_speech_bubble(float elapsed_ms_since_last_update) {
	Motion& speech_bubble_motion = registry.motions.get(speech_bubble);

	// for wide bubbles
	if (SPEECH_BUBBLE_TEXTURE == CUTSCENE_SPEECH_BUBBLE) {

		float new_width = speech_bubble_motion.scale.x + (SPEECH_BUBBLE_DIMENSIONS.x * (elapsed_ms_since_last_update / 400.f));

		speech_bubble_motion.position.x = SPEECH_BUBBLE_POSITION.x + min(SPEECH_BUBBLE_DIMENSIONS.x / 2, new_width / 2);
		speech_bubble_motion.scale.x = min(SPEECH_BUBBLE_DIMENSIONS.x, new_width);

	}

	// for tall bubbles
	else {
		float new_height = speech_bubble_motion.scale.y + (SPEECH_BUBBLE_DIMENSIONS.y * (elapsed_ms_since_last_update / 400.f));

		speech_bubble_motion.position.y = SPEECH_BUBBLE_POSITION.y - min(SPEECH_BUBBLE_DIMENSIONS.y / 2, new_height / 2);
		speech_bubble_motion.scale.y = min(SPEECH_BUBBLE_DIMENSIONS.y, new_height);
	}
}


// type the next letter of the message onto the speech bubble
void DialogueSystem::type_text() {
	// if we've reached the end of the line, move to the next one
	if (current_letter_index >= speech_lines[current_line_index].size()) {
		current_line_index++;
		current_letter_index = 0;
	}
	// else, check if we've finished the text timer and print the next letter
	else if (text_timer == 0) {
		Text& speech_bubble_text = registry.texts.get(speech_bubble);

		char current_letter = (speech_lines[current_line_index])[current_letter_index];
		speech_bubble_text.text_lines[current_line_index] += current_letter;

		current_letter_index++;
		text_timer = text_rendering_speed;
	}
	// else, decrement the timer
	else {
		text_timer--;
	}
}


// renders a message to let the user know that they can click on the message to advance to the next on
void DialogueSystem::show_advancement_sign() {
	if (!registry.renderRequests.has(advancement_sign)) {
		registry.renderRequests.insert(
			advancement_sign, {
				TEXTURE_ASSET_ID::DMG_BG,
				EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE,
				"ABOVE_SPEECH_BUBBLE",
			}
		);

		Motion& motion = registry.motions.emplace(advancement_sign);

		Text text = Text{};
		text.text_lines[0] = "[click to advance]";
		text.position = { SPEECH_BUBBLE_POSITION.x + 405, SPEECH_BUBBLE_POSITION.y - 175 };
		text.colour = { 146.f / 255.f, 131.f / 255.f, 112.f / 255.f };
		text.scale = 0.9f;
		registry.texts.insert(advancement_sign, text);
	}
}

// renders a message to let the user know that they can click on the message to advance to the next on
void DialogueSystem::hide_advancement_sign() {
	registry.remove_all_components_of(advancement_sign);
}


void DialogueSystem::render_character_name(Entity entity) {
	std::string name = registry.cutscene_elements.has(entity) ? registry.cutscene_elements.get(entity).name : "ERROR";

	// temporarily storing player-related entities in players
	registry.players.emplace(character_name_bubble);

	// set render and position of speech bubble
	registry.renderRequests.insert(
		character_name_bubble, {
			TEXTURE_ASSET_ID::CHARACTER_NAME_BUBBLE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"ABOVE_SPEECH_BUBBLE",
		}
	);

	Motion& name_motion = registry.motions.emplace(character_name_bubble);
	name_motion.position = { SPEECH_BUBBLE_POSITION.x + (SPEECH_BUBBLE_DIMENSIONS.x / 6), 
							 SPEECH_BUBBLE_POSITION.y - (SPEECH_BUBBLE_DIMENSIONS.y / 2)};
	name_motion.scale = { 150, 50 };

	// add text component
	Text text = Text{};
	text.text_lines[0] = name;
	text.position = { SPEECH_BUBBLE_POSITION.x - 15.5, SPEECH_BUBBLE_POSITION.y - 310 };
	text.colour = { 0.0, 0.0, 0.0 };
	text.scale = 1.20f;
	registry.texts.insert(character_name_bubble, text);

	// add ui stickiness
	registry.uiComponents.emplace(character_name_bubble);
}