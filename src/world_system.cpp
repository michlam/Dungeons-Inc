///////////////
//  imports  //
/////////////// 

#include "world_system.hpp"
#include "world_init.hpp"
#include "render_system.hpp"
#include "combat_system.hpp"


// stlib
#include <cassert>
#include <sstream>

// systems
#include "physics_system.hpp"
#include "card/card_functions.hpp"
#include <combat_system.hpp>
#include <phase/phase_system.hpp>
#include <level/level_system.hpp>
#include "animation_system.hpp"



///////////////
// constants //
/////////////// 
// Game configuration
const size_t MAX_TURTLES = 15;
const size_t MAX_FISH = 5;
const size_t TURTLE_DELAY_MS = 2000 * 3;
const size_t FISH_DELAY_MS = 5000 * 3;
const char* GAME_NAME = "Dungeons Inc.";



///////////////
// functions //
/////////////// 
// Create the fish world
WorldSystem::WorldSystem() {
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());
}


WorldSystem::~WorldSystem() {
	// Destroy music components
	if (background_music != nullptr)
		Mix_FreeMusic(background_music);
	Mix_CloseAudio();

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	glfwDestroyWindow(window);
}


// Debugging
namespace {
	void glfw_err_cb(int error, const char* desc) {
		fprintf(stderr, "%d: %s", error, desc);
	}
}


// World initialization
// Note, this has a lot of OpenGL specific things, could be moved to the renderer
GLFWwindow* WorldSystem::create_window() {
	///////////////////////////////////////
	// Initialize GLFW
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW");
		return nullptr;
	}

	//-------------------------------------------------------------------------
	// If you are on Linux or Windows, you can change these 2 numbers to 4 and 3 and
	// enable the glDebugMessageCallback to have OpenGL catch your mistakes for you.
	// GLFW / OGL Initialization
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// Create the main window (for rendering, keyboard, and mouse input)

	// make borderless windowed mode for game
	// source: https://discourse.glfw.org/t/clarification-of-borderless-fullscreen-and-borderless-windowed-modes/1235
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	//screen_width = mode->width;
	//screen_height = mode->height;
	
	window = glfwCreateWindow(window_width_px, window_height_px, GAME_NAME, nullptr, nullptr);

	// stop weird flickering effect???? that occurs when changing window focus. god i hate windows
	// source: http://forum.lwjgl.org/index.php?topic=4785.0
	// GLFWwindow* hGLFWwindow = window;
	HWND hWnd = glfwGetWin32Window(window);
	long style = GetWindowLongPtr(hWnd, GWL_STYLE);
	style &= ~WS_POPUP;
	SetWindowLongPtr(hWnd, GWL_STYLE, style);
	glfwShowWindow(window);

	if (window == nullptr) {
		fprintf(stderr, "Failed to glfwCreateWindow");
		return nullptr;
	}

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((RenderSystem*)glfwGetWindowUserPointer(wnd))->
		on_keyboard(_0, _1, _2, _3);
	// !!!! TODO: add a toggle to switch between borderless fullscreen and window mode 
	//// change from windowed to borderless fullscreen
	//if ((_2 == GLFW_RELEASE) && (_0 == GLFW_KEY_F) && (_3 == 0x0002)) {
	//	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	//	glfwSetWindowSize(wnd, screen_width, screen_height);

	//} else if ((_2 == GLFW_RELEASE) && (_0 == GLFW_KEY_W) && (_3 == 0x0002)) {
	//	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	//	printf("%i\n", window_width_px);
	//	printf("%i\n", window_height_px);
	//	glfwSetWindowSize(wnd, window_width_px, window_height_px);
	//}
	};
	auto mouse_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2) { ((RenderSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_click(wnd, _0, _1, _2); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((RenderSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_move({ _0, _1 }); };
	glfwSetKeyCallback(window, key_redirect);
	glfwSetMouseButtonCallback(window, mouse_redirect);
	glfwSetCursorPosCallback(window, cursor_pos_redirect);


	//////////////////////////////////////
	// Loading music and sounds with SDLw
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Failed to initialize SDL Audio");
		return nullptr;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
		fprintf(stderr, "Failed to open audio device");
		return nullptr;
	}

	background_music = Mix_LoadMUS(audio_path("music.wav").c_str());

	if (background_music == nullptr) {
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("music.wav").c_str(),
			audio_path("salmon_dead.wav").c_str(),
			audio_path("salmon_eat.wav").c_str());
		return nullptr;
	}



	///custom cursor!!
	GLFWimage cursorImage;
	cursorImage.pixels = stbi_load(textures_path("cursor1.png").c_str(), &cursorImage.width, &cursorImage.height, nullptr, 4);

	// Create a custom cursor
	GLFWcursor* customCursor = glfwCreateCursor(&cursorImage, 0, 0);

	// Set the custom cursor
	glfwSetCursor(window, customCursor);



	return window;
}


void WorldSystem::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;
	// Playing background music indefinitely
	// Mix_PlayMusic(background_music, -1);
	// fprintf(stderr, "Loaded music\n");

	// Set all states to default
	this->audio_player = Audio_system::get_audio();
	restart_game();
}


// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update) {

	// Remove debug info from the last step
	/*while (registry.debugComponents.entities.size() > 0)
		registry.remove_all_components_of(registry.debugComponents.entities.back());*/

		// apply cards of a room to characters in that room
	if (registry.room.size() > 0) {
		for (Entity room : registry.room.entities) {
			// we only care about rooms that have a HasCardApplied component
			if (registry.cards_on_room.has(room)) {
				// verify that the card being applied in this component is actually a card
				auto& applied_card_room_info = registry.cards_on_room.get(room);
				for (Entity character_entity : registry.roomEntities.get(room).roomEntities) {
					// verify that the card being applied in this component is actually a card
					for (Entity card_to_apply : applied_card_room_info.cards) {
						if (registry.cards.has(card_to_apply) && registry.characters.has(character_entity)) {
							// CHECK IF CHAR ENTITY IS AN ADVENTURER
							if (registry.adventurerClass.has(character_entity)) {
								// verify if the card has a CardEffect
								if (registry.card_effects.has(card_to_apply)) {
									// if a character has a statChange comp applied, replace it. only 1 stat change at a time (for now)
									// !!!! TODO: fix this like wtf
									registry.stat_changes.remove(character_entity);
									auto& stat_change = registry.stat_changes.emplace(character_entity);

									// fetch card's stat changes
									auto& card_stat_changes = registry.card_effects.get(card_to_apply);

									// set stat change to card's effect
									stat_change.stat_change.hp = card_stat_changes.stat_effects.hp;
									stat_change.stat_change.atk = card_stat_changes.stat_effects.atk;
									stat_change.stat_change.def = card_stat_changes.stat_effects.def;

									// add pointer to char entity 
									// !!!! TODO: fix this implementation its garbage because i am garbage
									applied_card_room_info.entities.insert(character_entity);
								}
							}

						}
					}
					// if a room doesnt have a card applied, remove 
				}
			}
		}	
	}

	updateAnimations(elapsed_ms_since_last_update);


	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	// Print mouse position
	// std::cout << "Mouse Position: X = " << mouseX << ", Y = " << mouseY << std::endl;
	RenderSystem::handle_char_hover({ mouseX, mouseY });
	return true;
}


// Reset the world state to its initial state
void WorldSystem::restart_game() {
	// Debugging for memory/component leaks
	registry.list_all_components();
	printf("Restarting\n");

	// Remove all entities that we created
	// All that have a motion, we could also iterate over all fish, turtles, ... but that would be more cumbersome
	while (registry.motions.entities.size() > 0) {
		registry.remove_all_components_of(registry.motions.entities.back());
	}

	// Debugging for memory/component leaks
	registry.list_all_components();
}


// checks if a room is in the current interactable row of rooms (as cards can only be applied to a certain row at a time)
bool is_room_in_active_row(Entity room) {
	//int curr_round = registry.phases.components[0].round;
	//int num_of_rounds = 3;

	//int starting_pos = curr_round * num_of_rounds;

	for (int i = registry.renderedRoom.size() - 1; i > 0; i--) {
		if (registry.renderedRoom.entities[i] == room) {
			return true;
		}
	}

	return false;
}


// compute collisions between entities
void WorldSystem::handle_collisions() {
	// to handle card-room boundaries, need to check if a card is no longer colliding with a room
	bool has_card_collision = false;

	// Loop over all collisions detected by the physics system
	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other_entity;

		// we only care about the object that the cursor is moving
		if (registry.cursorFocus.has(entity) && registry.cards.has(entity)) {
			// temp check because the cursor can only move cards for now
			// assert(registry.cards.has(entity));
			CursorFocus& cursor_focus = registry.cursorFocus.get(entity);
			Entity secondary_focus = entity_other;

			// check if cursor focus (card) is touching another card entity
			// and verify if the two cards are mergeable
			// OR check if the cursor focus is touching a room entity
			if (is_mergeable(entity, secondary_focus) || 
				// can only play cards on the "active" row
				(is_room_in_active_row(secondary_focus))) {
				// valid collision has been found
				// if the cursor_focus currently has a secondary focus, remove it and any glowing effect it may have had
				if (registry.secondaryFocus.size() > 0) {
					RenderRequest& secondary_focus = registry.renderRequests.get(registry.secondaryFocus.entities[0]);
					secondary_focus.used_effect = EFFECT_ASSET_ID::TEXTURED;
					registry.secondaryFocus.clear();
				}

				// if a valid room or mergeable, add secondary focus to it,
				// check if is room, if it is get corresponding actual room
				//if (registry.renderedRoom.has(secondary_focus)) {
				//	Entity corresponding_room = registry.correspondingRoom.get(secondary_focus).room;
				//	registry.secondaryFocus.emplace(corresponding_room);
				//}
				//else {
					registry.secondaryFocus.emplace(secondary_focus);
				//}
				has_card_collision = true;

				// cast a glow on the secondary focus just so it's more clear to the player
				// what their card is hovering on
				RenderRequest& secondary_focus_render = registry.renderRequests.get(secondary_focus);
				secondary_focus_render.used_effect = EFFECT_ASSET_ID::GLOWING;

				
			}

			// we also need to keep track of card-room collisions to verify that the card has been applied to the room
			// but ONLY after the cursor_focus timer has run out (which means the collision resolution has occurred)
			// do else-if because only one of these options should ever run
		} else if (registry.characters.has(entity) && registry.renderedRoom.has(entity_other)) {
			// ALL ROOMS START WITH AN EMPTY LIST AND A ROOM_ENTITIES COMPONENT but we will check anyways
			Entity corresponding_actual_room = registry.correspondingRoom.get(entity_other).room;
			std::vector<Entity> room = registry.room.get(corresponding_actual_room).connections;
			if (registry.roomEntities.has(corresponding_actual_room)) {
				// fetch list of room entities
				auto& room = registry.roomEntities.get(corresponding_actual_room);
				// check if character entity exists in list of room entities (reach end means did not find); if so, dont add
				// set automatically ensures duplicates aren't added i hope
				room.roomEntities.insert(entity);

			}
		}
	}

	// if there are NO CARD collisions, then remove secondary focus
	if (!has_card_collision) {
		// still need to check if there is a cursorFocus entity to reset
		if (registry.cursorFocus.size() > 0) {
			if (registry.secondaryFocus.size() > 0) {
				RenderRequest& secondary_focus = registry.renderRequests.get(registry.secondaryFocus.entities[0]);
				secondary_focus.used_effect = EFFECT_ASSET_ID::TEXTURED;
				registry.secondaryFocus.clear();
			}
		}
	}

	// Remove all collisions from this simulation step
	registry.collisions.clear();
}


// Should the game be over ?
bool WorldSystem::is_over() const {
	return bool(glfwWindowShouldClose(window));
}


// !!! TODO: MILESTONE 2
void WorldSystem::updateAnimations(float elapsed_ms) {
	// Check for paused and ignore pause components
	for (Entity entity : registry.animations.entities) {
		// Only run animations if not paused, or if animation ignores pause
		if (!is_paused || registry.animations_ignore_pause.has(entity)) {
			Animation& animated = registry.animations.get(entity);
			animated.time_elapsed_ms += elapsed_ms;

			if (animated.time_elapsed_ms > animated.ms_per_frame) {
				animated.time_elapsed_ms = 0;
				if (animated.repeating == true) {
					animated.curr_frame += 1;
					if (animated.curr_frame >= animated.num_frames[animated.animation_type]) {
						animated.curr_frame = 0;
					}
				}
				else if (animated.curr_frame < animated.num_frames[animated.animation_type] - 1) {
					animated.curr_frame += 1;
				}
			}
			// do nothing if not repeating and reach last frame
		}
	}
}