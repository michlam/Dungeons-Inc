///////////////
//  imports  //
///////////////
#include <phase/phase_system.hpp>
#include <level/level_system.hpp>
#include <level_select/level_select_system.hpp>
#include <phase/intro_system.hpp>
#include <animation_system.hpp>

#include "../ai_system.hpp"
#include "map_system.hpp"
#include "../utils.hpp"

// Declaration of phase system. DON'T DECLARE IT ANYWHERE ELSE!
PhaseSystem phase_system;
CombatSystem combat;

// A variable to keep track if the phase is paused
// Intitially set to true for the intro cutscene
bool is_paused = true;

// A variable to keep track of a tutorial's pause request
bool is_tutorial_paused = false;

// keeps track of the number of card draws
// TODO!!!! move to level system
int NUM_OF_DRAWS = 4;


void PhaseSystem::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;
	this->current_room = registry.room.entities[0];
	registry.currentRoom.emplace(this->current_room);

	this->audio_player = Audio_system::get_audio();
	//AISystem::Difficulty d = utils::load_AI_config();
	this->ai_system = AISystem::get_ai_system();
	//ai_system->set_difficulty(d);

	phase_ui = create_phase_ui(renderer_arg);
	phase_textbox = create_phase_textbox(renderer_arg);

	if (registry.levels.has(*current_level)) {
		Level& current_level_component = registry.levels.get(*current_level);
		bool is_tutorial = current_level_component.is_tutorial;
		if (is_tutorial) {
			dialogue_system.set_tutorial_settings();

			update_tutorial_textbox();
		}
		else {
			dialogue_system.set_standard_settings();

			dialogue_system.make_player_say_phase_quote(registry.phases.get(phase_ui).type);
		}
	}

	pause_overlay = create_pause_overlay(renderer_arg);
	//play_overlay = create_play_overlay(renderer_arg);

}

void PhaseSystem::delete_phase() {
	//registry.currentRoom.remove(this->current_room);
	registry.remove_all_components_of(phase_ui);
	registry.phases.remove(phase_ui); //
	registry.remove_all_components_of(phase_textbox);

	if (registry.levels.has(*current_level)) {
		if (registry.levels.get(*current_level).is_tutorial) {
			registry.remove_all_components_of(lore_ui);
			registry.remove_all_components_of(lore_textbox);
		}
	}
	registry.remove_all_components_of(tutorial_ui);
	registry.remove_all_components_of(tutorial_textbox);
	registry.remove_all_components_of(pause_overlay);

	for (Entity weapon : registry.weapons.entities) {
		registry.remove_all_components_of(weapon);
	}

	for (Entity statBox : registry.statusBoxes.entities) {
		registry.remove_all_components_of(statBox);
	}

	for (Entity health_bar : registry.healthBars.entities) {
		registry.remove_all_components_of(health_bar);
	}

	for (Entity adventurer : registry.adventurerClass.entities) {
		registry.remove_all_components_of(adventurer);
	}
	
	for (Entity mob : registry.mob.entities) {
		registry.remove_all_components_of(mob);
	}
	\
}

void PhaseSystem::delete_cards() {
	for (Entity card : registry.cards.entities) {
		registry.remove_all_components_of(card);
	}

	for (Entity card_effect : registry.card_effects.entities) {
		registry.remove_all_components_of(card_effect);
	}

	for (Entity deck : registry.deck.entities) {
		registry.remove_all_components_of(deck);
	}

	for (Entity hand : registry.hand.entities) {
		registry.remove_all_components_of(hand);
	}

	for (Entity card_in_play : registry.cards_in_play.entities) {
		registry.remove_all_components_of(card_in_play);
	}
}

void PhaseSystem::delete_map() {
	for (Entity room_entity : registry.room.entities) {
		registry.remove_all_components_of(room_entity);
	}

	for (Entity map_entity : registry.map.entities) {
		registry.remove_all_components_of(map_entity);
	}

	for (Entity room : registry.roomEntities.entities) {
		registry.remove_all_components_of(room);
	}

	for (Entity card_on_room : registry.cards_on_room.entities) {
		registry.remove_all_components_of(card_on_room);
	}

	for (Entity currentRoom_entity : registry.currentRoom.entities) {
		registry.remove_all_components_of(currentRoom_entity);
	}

	for (Entity renderedRoom_entity : registry.renderedRoom.entities) {
		registry.remove_all_components_of(renderedRoom_entity);
	}

	for (Entity correspondingRoom_entity : registry.correspondingRoom.entities) {
		registry.remove_all_components_of(correspondingRoom_entity);
	}

	for (Entity fullMapViewButton_entity : registry.fullMapViewButton.entities) {
		registry.remove_all_components_of(fullMapViewButton_entity);
	}

	for (Entity fullMapViewBackground_entity : registry.fullMapViewBackground.entities) {
		registry.remove_all_components_of(fullMapViewBackground_entity);
	}

	for (Entity fullMapViewRoom_entity : registry.fullMapViewRoom.entities) {
		registry.remove_all_components_of(fullMapViewRoom_entity);
	}

	for (Entity fullMapViewLine_entity : registry.fullMapViewLine.entities) {
		registry.remove_all_components_of(fullMapViewLine_entity);
	}

	for (Entity currentRender_entity : registry.currentRender.entities) {
		registry.remove_all_components_of(currentRender_entity);
	}
}

void PhaseSystem::delete_characters()
{
	for (Entity stat : registry.stats.entities) {
		registry.remove_all_components_of(stat);
	}

	for (Entity character : registry.characters.entities) {
		registry.remove_all_components_of(character);
	}

	for (Entity target_ent : registry.target.entities) {
		registry.remove_all_components_of(target_ent);
	}

	for (Entity enemy_ent : registry.enemy.entities) {
		registry.remove_all_components_of(enemy_ent);
	}

}

void PhaseSystem::next_phase() {
	Phase& phase = registry.phases.get(phase_ui);
	
	// set next phase type (https://stackoverflow.com/questions/40979513/changing-enum-to-next-value-c11)
	phase.type = static_cast<phase_type>((phase.type + 1) % (END + 1));

	// if start of new phase, increment round number
	if (phase.type == START) {
		phase.round++;
	}

	// set new time limit
	phase.time_limit = TIME_LIMITS[phase.type];

	// set new time remaining
	phase.time_remaining = TIME_LIMITS[phase.type];


	// CODE FOR HANDLING CARDS
	// MR. TA I KNOW ITS BAD TO HARDCODE BUT THIS IS TEMPORARY PLEASE DONT LOOK AT IT TOO HARD
	// !!!! TODO: FIX THIS GARBAGE 
	phase_type current_phase = phase.type;
	//auto& curr_path = *path;

	Level& current_level_component = registry.levels.get(*current_level);
	bool is_tutorial = current_level_component.is_tutorial;
	Entity mob;


	if (phase.round <= (current_level_component.depth + 2)) {
		switch (current_phase) {
		case START: {

			if ((phase.round == 2 && is_tutorial) || 
				registry.enemy.size() == 0 || 
				(phase.round == (current_level_component.depth + 2))) {
				// If it is tutorial, or if the level is won, unlock the next level!
				if (is_tutorial || registry.enemy.size() == 0) level_system.save_level_json();

				delete_phase();
				delete_cards();
				delete_map();
				delete_characters();
				adventurers.clear();

				level_system.removeVictoryScreen();

				dialogue_system.reset_dialogue_system();

				level_select_system.init(renderer);
			}
			break; }
		case DRAW: {
			// draw cards to hand at the beginning of the phase
			for (int i = NUM_OF_DRAWS; i > 0; i--) {
				if (registry.hand.size() < 8) {
					draw_card_from_deck(renderer);
				}
			}
			this->audio_player->play_draw();

			// IF ROUND 1 PAUSE THE PHASE AND SHOW TUTORIAL BOX
			if (phase.round == 1 && is_tutorial) {
				is_tutorial_paused = true;
			}
			break; }
		case PLAN: {
			// cards can be applied
			if (phase.round == 1 && is_tutorial) {
				is_tutorial_paused = true;

				// bandaid fix to remove glow from tutorial cards
				for (Entity card : registry.cards.entities) {
					RenderRequest& card_render = registry.renderRequests.get(card);
					card_render.used_effect = EFFECT_ASSET_ID::TEXTURED;
				}
			}
			break; }
		case PRE_COMBAT: {
			// remove applied cards (visually and from hand)
			for (Entity in_play : registry.cards_in_play.entities) {
				registry.renderRequests.remove(in_play);
			}
			reformat_hand();

			// remove cursor and secondary focus
			registry.cursorFocus.clear();

			// also reset glow (if it has any)
			for (Entity entity : registry.secondaryFocus.entities) {
				if (registry.renderRequests.has(entity)) {
					RenderRequest& entity_render = registry.renderRequests.get(entity);
					entity_render.used_effect = EFFECT_ASSET_ID::TEXTURED;
				}
			}
			registry.secondaryFocus.clear();

			// unglow previous room
			registry.renderRequests.get(current_room).used_effect = EFFECT_ASSET_ID::TEXTURED;

			this->current_room = this->ai_system->move_to_next_room(current_room);

			Entity actual_rendered_room;

			std::vector<Entity> rendered_rooms = registry.renderedRoom.entities;
			for (int i = 0; i < rendered_rooms.size(); i++) {
				Entity corresponding_room = registry.correspondingRoom.get(rendered_rooms[i]).room;
				if (corresponding_room == current_room) {
					actual_rendered_room = rendered_rooms[i];
					break;
				}
			}

			this->audio_player->play_door();

			vec2 goal_room_position = registry.motions.get(actual_rendered_room).position;


			for (int i = 0; i < registry.movements.size(); i++) {
				Entity& adventurer = registry.movements.entities[i];
				Motion& adv_motion = registry.motions.get(adventurer);

			}



			// check which direction the entities are moving in and add the movements in the right order
			std::vector<std::vector<vec2>> path_order;
			if (goal_room_position.x < 595.0) {
				path_order = character_paths[0];
			}
			else if (goal_room_position.x > 605.0) {
				path_order = character_paths[1];
			}
			else {
				path_order = character_paths[2];
			}

			// move adventurers to the next room in the path
			// figure out which character path thing to take and add the movements to a queue where
			// each element in the queue is (eneity id, goal position)
			for (int i = 0; i < registry.movements.size(); i++) {
				MovementInstructions& movement = registry.movement_instructions.emplace(registry.movements.entities[i]);
				int char_index = position_to_index(registry.motions.get(registry.movements.entities[i]).position);
				movement.char_offset = character_positions[char_index];
				movement.goal_positions = path_order[char_index];
				registry.movements.components[i].goal_pos = movement.goal_positions[movement.curr_goal];
			}
			

			if (phase.round == 1 && is_tutorial) {
				is_tutorial_paused = true;
			}

			// update current room
			registry.currentRoom.clear();
			registry.currentRoom.emplace(this->current_room);

			// glow current room on full map view
			registry.renderRequests.get(current_room).used_effect = EFFECT_ASSET_ID::GLOWING;

			break; 
		}

		case COMBAT: {
			map_system.render_new_current_room_and_connecting_rooms(current_room);

			MobClass::MOBCLASS mob_class = MobClass::BARBARIAN;

			if (!is_tutorial && current_level_component.name != "Level One" && current_level_component.name != "Level Two" && current_level_component.name != "Level Three") {
				std::random_device rand;
				std::mt19937 gen(rand());
				std::uniform_int_distribution<int> distribution(0, 2);

				int random_num = distribution(gen);

				switch (random_num) {
				case 0:
					mob_class = MobClass::BARBARIAN;
					break;
				case 1:
					mob_class = MobClass::BRUTE;
					break;
				case 2:
					mob_class = MobClass::SHAMAN;
					break;
				default:
					mob_class = MobClass::BARBARIAN;
					break;
				}
			}

			mob = createMob(renderer, vec2(600.f, 550.f), mob_class);

			std::vector <Entity> adventurers_list = registry.adventurerClass.entities;
			for (int i = 0; i < adventurers_list.size(); i++) {
				registry.motions.get(adventurers_list[i]).position = vec2(600.0, 550.0) + character_positions[i];
				Motion& motion = registry.motions.get(adventurers_list[i]);
				Entity& weapon = registry.characters.get(adventurers_list[i]).weapon;
				Motion& weapon_motion = registry.motions.get(weapon);
				if (motion.scale.x > 0) {
					weapon_motion.angle = M_PI / 8;
				}
				else {
					weapon_motion.angle = -M_PI / 8;
				}


				if (registry.adventurerClass.get(adventurers_list[i]).advClass == Class::RANGER) {
					weapon_motion.scale.x = motion.scale.x;
				}
				else {
					weapon_motion.scale.x = motion.scale.x / 3 * 2;

				}
				//weapon_motion.scale.x = motion.scale.x / 3 * 2;

				float weapon_offset_x = motion.scale.x / 3;
				float weapon_offset_y = motion.scale.y / 4;

				weapon_motion.position = vec2{ motion.position.x + weapon_offset_x,  motion.position.y + weapon_offset_y };
			}
			// Make the miniboss
			if (phase.round == (current_level_component.depth + 1) && current_level_component.name != "Level One") {
				Motion& mob_motion = registry.motions.get(mob);
				mob_motion.scale = { mob_motion.scale.x * 2, mob_motion.scale.y * 2 };

				registry.motions.get((registry.characters.get(mob).weapon)).scale =
					vec2{ mob_motion.scale.x / 3 * 2, mob_motion.scale.y * 1.5 };
			}

			combat_system.init_combat(mob);
			// combat_system.init_combat(createMob(renderer, registry.motions.get(this->current_room).position, Race::GOBLIN));
			// do nothing else ig cuz step func does it

			if (phase.round == 1 && is_tutorial) {
				is_tutorial_paused = true;
			}
			break; }
		case DAMAGE: {
			// returnString = "DAMAGE";

			break; }
		case END: {
			// read them back idk why we have to do this...
			for (Entity adventurer : registry.adventurerClass.entities) {
				if (!registry.movements.has(adventurer)) {
					registry.movements.emplace(adventurer);
				}
			}

			// remove cards that were in play
			for (Entity in_play : registry.cards_in_play.entities) {
				remove_card_from_room(in_play);
				registry.remove_all_components_of(in_play);
			}

			if (phase.round == 1 && is_tutorial) {
				is_tutorial_paused = true;
			}

			// victory condition
			if (registry.enemy.size() == 0) {
				level_system.showVictoryScreen(renderer);
			}
			// defeat condition
			if (phase.round == (current_level_component.depth + 1) && registry.enemy.size() != 0) {
				level_system.showDefeatScreen(renderer);
			}

			break; }
		default: {
			assert(!"Invalid phase type");
			break; }
		}

		// handle tutorial box
		if (is_tutorial_paused && !is_paused && is_tutorial) {
			toggle_pause();
			update_tutorial_textbox();
		}
		else if (!is_tutorial) {
			dialogue_system.make_player_say_phase_quote(phase.type);
		}
	}
}


void PhaseSystem::toggle_pause() {
	is_paused = !is_paused;
	std::cout << "is paused: " << is_paused << std::endl;
	//audio_player->play_intro();

	// Shows the paused overlay if paused and in game
	if (current_scene == IN_GAME) {
		RenderRequest& rr = registry.renderRequests.get(pause_overlay);
		//RenderRequest& rr_play = registry.renderRequests.get(play_overlay);

		if (is_paused) {
			rr.z_val = "PAUSE";
			//rr_play.z_val = "INVIS";

		}
		else {
			rr.z_val = "INVIS";
			//rr_play.z_val = "PAUSE";

		}
	}
}


void PhaseSystem::keyboard_handler(int key, int action) {
	Phase& phase = registry.phases.get(phase_ui);

	// Handle key inputs
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ENTER:
				// If it is un-paused starting phase 1, enter should skip.
				if (phase.round == 1 && phase.type == START && !is_paused) {
					next_phase();
				}

				// If it is un-paused and plan phase, enter should skip.
				if (phase.type == PLAN && !is_paused) {
					next_phase();
				}
				break;
			case GLFW_KEY_SPACE:
				// toggle pause!
				// If it is during intro, don't unpause:
				if (is_intro_complete) {
					is_tutorial_paused = false;
					toggle_pause();
				}

				break;
		}
	}
}


void PhaseSystem::step(float elapsed_ms_since_last_update) {

	if (current_scene == IN_GAME && !is_paused) {

		// Loop through every entity with a phase component
		Phase& phase = registry.phases.get(phase_ui);
		phase.time_remaining -= (elapsed_ms_since_last_update / 1000);
		if (phase.time_remaining < 0) {
			phase.time_remaining = 0;
		}

		update_phase_textbox();

		if (phase.type == COMBAT) {
			combat.process_combat(elapsed_ms_since_last_update);
		}

		Level& current_level_component = registry.levels.get(*current_level);
		bool is_tutorial = current_level_component.is_tutorial;

		if (phase.time_remaining <= 0) {
			next_phase();
			std::cout << "Round: " << phase.round << ", Phase: " << phase.type << std::endl;
		}

		if ((phase.round == 1 && phase.type == START) && !is_tutorial) {
			// TODO: IMPROVE THIS FOR LEVEL SELECTION LATER!
			// get rid of tutorial ui and tutorial textbox
			// dialogue_system.reset_dialogue_system();
		}
	}
}


void PhaseSystem::reset_phase() {
	Phase& phase = registry.phases.get(phase_ui);
	phase.round = 1;
	phase.type = START;
	phase.time_limit = START_TIME;
	phase.time_remaining = phase.time_limit;
}


Entity PhaseSystem::create_phase_ui(RenderSystem* renderer) {
	auto entity = Entity();

	// Set initial phase values
	Phase& phase = registry.phases.emplace(entity);
	phase.round = 1;
	phase.type = START;

	phase.time_limit = START_TIME;
	if (*current_level == tutorial_level) {
		phase.time_limit *= 5;
	}

	phase.time_remaining = phase.time_limit;

	// The following handles the rendering, following the createTurtle template
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = { WINDOW_WIDTH/2, 20 };

	// Setting initial scale values
	motion.scale = vec2({ 550, 100 });

	// Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::PHASE_UI,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"TIMER"
		}
	);

	// Phase UI should be unmoving
	registry.uiComponents.emplace(entity);

	return entity;
}


Entity PhaseSystem::create_phase_textbox(RenderSystem* renderer) {
	Motion& parent_motion = registry.motions.get(phase_ui);
	// Initialize the text
	Text text = Text{};
	text.text_lines[0] = "";
	text.colour = { 0.0, 0.0, 0.0 };

	text.position = parent_motion.position + vec2(-360, -47);
	text.scale = 1.f;
	registry.texts.insert(phase_ui, text);


	return phase_ui;
}


Entity PhaseSystem::create_pause_overlay(RenderSystem* renderer) {
	Entity entity = Entity();

	// The following handles the rendering, following the createTurtle template
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2});

	// Setting initial scale values
	motion.scale = vec2({ WINDOW_WIDTH, WINDOW_HEIGHT });

	// Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::PAUSE_OVERLAY,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"INVIS"
		}
	);

	// Lore UI should be unmoving.
	registry.uiComponents.insert(entity, {});

	return entity;
}

Entity PhaseSystem::create_play_overlay(RenderSystem* renderer) {
	Entity entity = Entity();

	// The following handles the rendering, following the createTurtle template
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = vec2({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 });

	// Setting initial scale values
	motion.scale = vec2({ WINDOW_WIDTH, WINDOW_HEIGHT });

	// Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::PLAY_OVERLAY,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"PAUSE"
		}
	);

	// Lore UI should be unmoving.
	registry.uiComponents.insert(entity, {});

	return entity;
}


void PhaseSystem::update_phase_textbox() {
	Phase& phase = registry.phases.get(phase_ui);
	// auto& textsRegistry = registry.texts;

	// String stream taken from https://stackoverflow.com/questions/10219225/c-create-string-of-text-and-variables

	Text& text = registry.texts.get(phase_ui);
	std:std::ostringstream oss0, oss1, oss2, oss3;
	/*oss << "Round: " << phase.round << " Phase: " << phase.type << " Time Remaining : " << round(phase.time_remaining);
	text.text = oss.str();*/

	Level& current_level_component = registry.levels.get(*current_level);

	oss3 << " " << current_level_component.name << "     ROUND " << phase.round << " - " << get_phase_type(phase.type) << "     TIME: " << round(phase.time_remaining) << "s";
	text.text_lines[3] = oss3.str(); // Phase textline

	//oss1 << "Round: " << phase.round;
	//text.text_lines[1] = oss1.str(); // Phase textline

	//oss2 << "Phase: " << get_phase_type(phase.type);
	//text.text_lines[2] = oss2.str(); // Round textline

	//oss3 << "Time Remaining: " << round(phase.time_remaining);
	//text.text_lines[3] = oss3.str(); // Timer textline
	
}


void PhaseSystem::update_tutorial_textbox() {
	Phase& phase = registry.phases.get(phase_ui);

	// We only want this to happen during ROUND 1
	if (phase.round == 1) {
		switch (phase.type) {
			case START:
				dialogue_system.make_player_interrupt("What the heck? Why are there adventurers at my door already???");
				dialogue_system.make_player_say("What do I do? What do I do???");
				dialogue_system.make_player_say("Let me see if Grandpa left me any useful notes...");
				dialogue_system.make_player_say("One page says that, if I think hard enough, I can imagine some sort of SPACEBAR in my mind.");
				dialogue_system.make_player_say("And if I try hard enough, I might be able to press SPACEBAR to freeze time...");
				break;

			case DRAW:
				dialogue_system.make_player_interrupt("Oh - what are these? They're... cards? From Grandpa's deck of cards?");
				dialogue_system.make_player_say("Why did they just appear in my hand like that? This is all so weird...");
				break;

			case PLAN:
				dialogue_system.make_player_interrupt("Let me see if Grandpa left any useful information on how to use these cards.");
				dialogue_system.make_player_say("Hmm, it says I can cast cards onto rooms. If I'm having trouble doing that, I can try imagining a MOUSE and using it to CLICK and DRAG a card...");
				dialogue_system.make_player_say("Why would I imagine a rat???");
				dialogue_system.make_player_say("It also says I can merge two cards of the same type and tier into a stronger version of the card.");
				dialogue_system.make_player_say("But it doesn't say how to do this. Maybe I also need to imagine a MOUSE, and try CLICKING and DRAGGING the right cards on top of each other?");
				dialogue_system.make_player_say("Hey, it also says that I can imagine an ENTER button to fast-forward time... I wonder what that means?");
				break;

			case PRE_COMBAT:
				dialogue_system.make_player_interrupt("Hey, those adventurers just moved! It... looks like they waited to see what cards I played?");
				dialogue_system.make_player_say("Does that mean the adventurers know what cards I play on each room? I guess I need to use my cards wisely...");
				dialogue_system.make_player_say("I should probably also keep track of which rooms the adventurers can enter in, so I can play cards that force them down certain rooms and pathways.");
				dialogue_system.make_player_say("Grandpa's notes say that there should be some BUTTON in the TOP-RIGHT corner of my vision that I can use to display a map of the dungeon...");
				break;

			case COMBAT:
				dialogue_system.make_player_interrupt("Oh hey, that's my goblin pal Fred! What's he doing there with those adventurers?");
				dialogue_system.make_player_say("WAIT NO DON'T HURT HIM! PLEASE!!!");
				dialogue_system.make_player_say("FREDDY!! FIGHT BACK!!! YOU CAN DO IT!!!!!");
				break;

			case END:
				dialogue_system.make_player_interrupt("Oh the misery... Freddy... I swear I'll avenge you......");
				dialogue_system.make_player_say("I'll grow stronger... I'll train super hard... I'll build my dungeon so that no adventurer can take me down!");
				dialogue_system.make_player_say("Just you wait, you damned adventurers! I'm gonna surpass my grandfather and be the greatest dungeon master you've ever seen!");
				break;

			default:
				dialogue_system.reset_dialogue_system();
				break;

		}
	}
}


std::string PhaseSystem::get_phase_type(phase_type type) {
	std::string returnString = "";

	switch (type) {
	case START:
		returnString = "START";
		break;
	case DRAW:
		returnString = "DRAW";
		break;
	case PLAN:
		returnString = "PLAN";
		break;
	case PRE_COMBAT:
		returnString = "PRE-COMBAT";
		break;
	case COMBAT:
		returnString = "COMBAT";
		break;
	case DAMAGE:
		returnString = "DAMAGE";
		break;
	case END:
		returnString = "END";
		break;
	default:
		assert(!"Invalid phase type");
		break;
	}

	return returnString;
}


phase_type PhaseSystem::string_to_phase_type(std::string str) {
	static const std::map<std::string, phase_type> string_to_phase_type_map = {
		{"START", START},
		{"DRAW", DRAW},
		{"PLAN", PLAN},
		{"PRE-COMBAT", PRE_COMBAT},
		{"COMBAT", COMBAT},
		{"DAMAGE", DAMAGE},
		{"END", END}
	};

	auto it = string_to_phase_type_map.find(str);
	if (it != string_to_phase_type_map.end()) {
		return it->second;
	}

	// Handle the case where the string doesn't map to any enum value
	throw std::invalid_argument("Invalid enum string: " + str);
}

int PhaseSystem::position_to_index(vec2 position) {
	if (position.x == 550 && position.y == 500) {
		return 0;
	}
	else if (position.x == 550 && position.y == 600) {
		return 1;
	}
	else if (position.x == 650 && position.y == 600) {
		return 3;
	}
	else {
		return 2;
	}
}