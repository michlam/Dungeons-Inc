#pragma once

///////////////
//  imports  //
/////////////// 
// common
#include "common.hpp"

// stdlib
#include <vector>
#include <unordered_map>
#include "../ext/stb_image/stb_image.h"


///////////////
// constants //
///////////////
// Controls the depth at which something is rendered. Layer0 is at the bottom.
// !!!! TODO: change to const ids like the asset/buffer ids

const std::string layers[38] = { "HIDE ROOM VIEW MAP", "HIDE ROOM VIEW ROOM", "HIDE ROOM VIEW LINE", "BACKGROUND", "PATHWAYS", 
								"ROOMS", "CHARACTERS", "WEAPONS", "PARTICLES", "HEALTHBAR",  "DAMAGE NUMBER",
								"CARDS_ON_ROOM", "CARDS", "DECK", "PORTRAIT", "CHARACTER_PORTRAIT", "SPEECH_BUBBLE", "ABOVE_SPEECH_BUBBLE",
								"UI", "TIMER", "TIMER_TEXT", "PRIORITY", "PRIORITY_TEXT", 
								"CUTSCENEBG", "CUTSCENEOBJECT1", "CUTSCENEOBJECT2", "PAUSE", "TESTING2", 
								"TESTING3", "PRIORITY_TEXT", "SHOW ROOM VIEW MAP", "SHOW ROOM VIEW LINE", "SHOW ROOM VIEW ROOM",  
								"SWITCH MAP VIEW BUTTON", "MENU BUTTON", "MENU UI 1", "MENU UI 2", "MENU UI 3"
};


///////////////
//  structs  //
///////////////
// Player component
struct Player {
};


struct CursorFocus {
	// represents the other entity that may be interacting with the current cursor focus
	// NOT GUARANTEED TO BE INITIALIZED
	// ntity secondary_focus;
	// bool is_focus_set = false;
};


struct SecondaryFocus {
	// flag for the secondary focus for visualization reasons
};


// All data relevant to the shape and motion of entities
struct Motion {
	vec2 position = { 0.f, 0.f };
	float angle = 0.f;
	vec2 velocity = { 0.f, 0.f };
	vec2 scale = { 10.f, 10.f };
};

struct Movement {
	vec2 goal_pos = { -1.f, -1.f };
};

struct MovementInstructions {
	std::vector<vec2> goal_positions = {};
	vec2 char_offset = { 0.f, 0.f };
	int curr_goal = 0;
};


// Stucture to store collision information
struct Collision {
	// Note, the first object is stored in the ECS container.entities
	Entity other_entity; // the second object involved in the collision
	Collision(Entity& other_entity) { this->other_entity = other_entity; };
};

// Stucture to store collision information
struct Hovered {
	Entity entity; // the object being hovered
};

// Data structure for toggling debug mode
struct Debug {
	bool in_debug_mode = 0;
	bool in_freeze_mode = 0;
};
extern Debug debugging;

// Sets the brightness of the screen
struct ScreenState {
	float screen_darken_factor = 0.f;
	int ticks = 0;
	bool is_darken = true;
};

// A struct to refer to debugging graphics in the ECS
struct DebugComponent {
	// Note, an empty struct has size 1
};

// Single Vertex Buffer element for non-textured meshes (coloured.vs.glsl & salmon.vs.glsl)
struct ColoredVertex {
	vec3 position;
	vec3 color;
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex {
	vec3 position;
	vec2 texcoord;
};

struct UI {
	// purpose to label which components are part of the UI and thus should use a constant
	// 2d projection matrix in order to be "sticky" to the game window
};

// Mesh datastructure for storing vertex and index buffers
struct Mesh {
	static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
	vec2 original_size = {1,1};
	std::vector<ColoredVertex> vertices;
	std::vector<uint16_t> vertex_indices;
};

struct Animation
{
	float sprite_w;
	float sprite_h;

	int curr_frame = 0;
	// max 3 animations per sprite right now
	int num_frames[3];

	int time_elapsed_ms;
	int ms_per_frame = 300;

	int animation_type = 0;
	bool repeating;
};


// flag component for animations that ignore pause (ex: cutscenes)
struct AnimationIgnorePause
{

};


// Line component flag
struct Line {
};

struct FullMapViewButton {
	bool is_active = false;
};

struct FullMapViewBackground {

};

struct FullMapViewRoom{
	
};

struct FullMapViewLine{

};

struct CurrentRender {
	
};

// Level Select Menu Button flag
struct LSMButton {

};

// LSM Popup window flag
struct LSMPopup {
	bool is_active = false;
};

// LSM Start Button flag
struct LSMStartButton {
};

// LSM Background flag
struct LSMBackground {
};

// LSM Selector Bounding Box flag
struct LSMSelector {

};

// LSM Little Guy flag
struct LSMLittleGuy {

};

// Open Menu Button flag
struct OpenMenuButton {

};

// Menu UI + Buttons flag
struct MenuUi {

};

// Menu Buttons flag
struct MenuButtons {

};

// Settings UI flag
struct SettingsUI {

};

// Settings Buttons flag
struct SettingsButtons {

};

// Settings Icons flag
struct SettingsIcons {

};
/**
 * The following enumerators represent global identifiers refering to graphic
 * assets. For example TEXTURE_ASSET_ID are the identifiers of each texture
 * currently supported by the system.
 *
 * So, instead of referring to a game asset directly, the game logic just
 * uses these enumerators and the RenderRequest struct to inform the renderer
 * how to structure the next draw command.
 *
 * There are 2 reasons for this:
 *
 * First, game assets such as textures and meshes are large and should not be
 * copied around as this wastes memory and runtime. Thus separating the data
 * from its representation makes the system faster.
 *
 * Second, it is good practice to decouple the game logic from the render logic.
 * Imagine, for example, changing from OpenGL to Vulkan, if the game logic
 * depends on OpenGL semantics it will be much harder to do the switch than if
 * the renderer encapsulates all asset data and the game logic is agnostic to it.
 *
 * The final value in each enumeration is both a way to keep track of how many
 * enums there are, and as a default value to represent uninitialized fields.
 */

enum class TEXTURE_ASSET_ID {
	// ui
	UI_WINDOW = 0,

	// dialogue & cutscene
	PORTRAIT_WINDOW = UI_WINDOW + 1,
	SPEECH_BUBBLE = PORTRAIT_WINDOW + 1,
	CHARACTER_NAME_BUBBLE = SPEECH_BUBBLE + 1,
	CUTSCENE_SPEECH_BUBBLE = CHARACTER_NAME_BUBBLE + 1,
	CUTSCENE_CHOICE_BUBBLE = CUTSCENE_SPEECH_BUBBLE + 1,
	TUTORIAL_SPEECH_BUBBLE = CUTSCENE_CHOICE_BUBBLE + 1,
	CUTSCENE_BACKGROUND = TUTORIAL_SPEECH_BUBBLE + 1,

	// cutscene emotions
	CUTSCENE_LOVE = CUTSCENE_BACKGROUND + 1,
	CUTSCENE_ANGER = CUTSCENE_LOVE + 1,
	CUTSCENE_CONFUSION = CUTSCENE_ANGER + 1,
	CUTSCENE_SURPRISE = CUTSCENE_CONFUSION + 1,

	// phase
	PAUSE_OVERLAY = CUTSCENE_SURPRISE + 1,
	PLAY_OVERLAY = PAUSE_OVERLAY + 1,
	PHASE_UI = PLAY_OVERLAY + 1,

	// dungeon map
	MAP = PHASE_UI + 1,
	ROOM = MAP + 1,

	// card system assets
	CARD_HP1 = ROOM + 1,
	CARD_HP2 = CARD_HP1 + 1,
	CARD_HP3 = CARD_HP2 + 1,

	CARD_DEF1 = CARD_HP3 + 1,
	CARD_DEF2 = CARD_DEF1 + 1,
	CARD_DEF3 = CARD_DEF2 + 1,

	CARD_ATK1 = CARD_DEF3 + 1,
	CARD_ATK2 = CARD_ATK1 + 1,
	CARD_ATK3 = CARD_ATK2 + 1,

	CARD_ALL1 = CARD_ATK3 + 1,
	CARD_ALL2 = CARD_ALL1 + 1,
	CARD_ALL3 = CARD_ALL2 + 1,

	CARD1 = CARD_ALL3 + 1,
	CARD2 = CARD1 + 1,
	CARD3 = CARD2 + 1,

	DECK = CARD3 + 1,
	HAND_EMPTY = DECK + 1,

	// characters
	WARRIOR = HAND_EMPTY + 1,
	MAGE = WARRIOR + 1,
	RANGER = MAGE + 1,
	BARBARIAN = RANGER + 1,
	BRUTE = BARBARIAN + 1,
	SHAMAN = BRUTE + 1,

	LILGUY = SHAMAN + 1,

	// intro
	INTRO_WINDOW = LILGUY + 1,
	RUNNING_GOBLIN = INTRO_WINDOW + 1,
	GROUND = RUNNING_GOBLIN + 1,

	// level select menu
	LSM_BG_1 = GROUND + 1,
	LSM_BG_2 = LSM_BG_1 + 1,
	LSM_BG_3 = LSM_BG_2 + 1,
	LSM_BG_4 = LSM_BG_3 + 1,
	LSM_WINDOW = LSM_BG_4 + 1,
	LSM_BUTTON = LSM_WINDOW + 1,
	LSM_POPUP = LSM_BUTTON + 1,
	LSM_START_BUTTON = LSM_POPUP + 1,
	LSM_POPUP_TUTORIAL = LSM_START_BUTTON + 1,
	LSM_POPUP_ONE = LSM_POPUP_TUTORIAL + 1,
	LSM_POPUP_TWO = LSM_POPUP_ONE + 1,
	LSM_POPUP_THREE = LSM_POPUP_TWO + 1,
	LSM_POPUP_FOUR = LSM_POPUP_THREE + 1,
	LSM_POPUP_FIVE = LSM_POPUP_FOUR + 1,
	LSM_POPUP_SIX = LSM_POPUP_FIVE + 1,
	LSM_POPUP_SEVEN = LSM_POPUP_SIX + 1,

	LSM_BUTTON_TUTORIAL = LSM_POPUP_SEVEN + 1,
	LSM_BUTTON_ONE = LSM_BUTTON_TUTORIAL + 1,
	LSM_BUTTON_TWO = LSM_BUTTON_ONE + 1,
	LSM_BUTTON_THREE = LSM_BUTTON_TWO + 1,
	LSM_BUTTON_FOUR = LSM_BUTTON_THREE + 1,
	LSM_BUTTON_FIVE = LSM_BUTTON_FOUR + 1,
	LSM_BUTTON_SIX = LSM_BUTTON_FIVE + 1,
	LSM_BUTTON_SEVEN = LSM_BUTTON_SIX + 1,


	LSM_LEVEL_SELECT = LSM_BUTTON_SEVEN + 1,
	LSM_SELECTOR_TESTING = LSM_LEVEL_SELECT + 1,


	// map switching
	MAP_VIEW_BUTTON = LSM_SELECTOR_TESTING + 1,
	ROOMS_VIEW_DUNGEON_BACKGROUND = MAP_VIEW_BUTTON + 1,
	FULL_MAP_VIEW_ROOM = ROOMS_VIEW_DUNGEON_BACKGROUND + 1,

	// particles
	RED_PARTICLE = FULL_MAP_VIEW_ROOM + 1,
	BLUE_PARTICLE = RED_PARTICLE + 1,
	DMG_BG = BLUE_PARTICLE + 1,

	//healthbars
	MOB_HEALTHBAR = DMG_BG + 1,
	ADVENTURER_HEALTHBAR = MOB_HEALTHBAR + 1,

	// weapons
	ARROW = ADVENTURER_HEALTHBAR + 1,
	CLUB = ARROW + 1,
	BOW = CLUB + 1,
	SPEAR = BOW + 1,
	STAFF = SPEAR + 1,
	STAFF2 = STAFF + 1,

	SWORD = STAFF2 + 1,

	// win-loss
	VICTORY = SWORD + 1,
	DEFEAT = VICTORY + 1,

	// menu
	OPEN_MENU_BUTTON = DEFEAT + 1,
	MENU_UI = OPEN_MENU_BUTTON + 1,
	RESET_DATA_BUTTON = MENU_UI + 1,
	EXIT_LEVEL_BUTTON = RESET_DATA_BUTTON + 1,
	MENU_SETTINGS_BUTTON = EXIT_LEVEL_BUTTON + 1,
	CLOSE_MENU_BUTTON = MENU_SETTINGS_BUTTON + 1,
	MUSIC_ENABLED_ICON = CLOSE_MENU_BUTTON + 1,
	MUSIC_DISABLED_ICON = MUSIC_ENABLED_ICON + 1,
	SFX_ENABLED_ICON = MUSIC_DISABLED_ICON + 1,
	SFX_DISABLED_ICON = SFX_ENABLED_ICON + 1,
	SETTINGS_UI = SFX_DISABLED_ICON + 1,
	SETTINGS_BACK_BUTTON = SETTINGS_UI + 1,
	DIFF_1_ICON = SETTINGS_BACK_BUTTON + 1,
	DIFF_2_ICON = DIFF_1_ICON + 1,
	DIFF_3_ICON = DIFF_2_ICON + 1,

	DUNGEON_MAP_L = DIFF_3_ICON + 1,
	DUNGEON_MAP_M = DUNGEON_MAP_L + 1,
	DUNGEON_MAP_R = DUNGEON_MAP_M + 1,
	DUNGEON_MAP_LM = DUNGEON_MAP_R + 1,
	DUNGEON_MAP_MR = DUNGEON_MAP_LM + 1,
	DUNGEON_MAP_LR = DUNGEON_MAP_MR + 1,
	DUNGEON_MAP_LMR = DUNGEON_MAP_LR + 1,
	DUNGEON_MAP_NONE = DUNGEON_MAP_LMR + 1,

	TEXTURE_COUNT = DUNGEON_MAP_NONE + 1
};

const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
	COLOURED = 0,
	TEXT = COLOURED + 1,
	TEXTURED = TEXT + 1,
	TEXTURED_ANIMATION = TEXTURED + 1,
	WATER = TEXTURED_ANIMATION + 1,
	PEBBLE = WATER + 1,
	GLOWING = PEBBLE + 1,
	LOGO_TEXT = GLOWING + 1,
	LSM_GREYED = LOGO_TEXT + 1,
	PARTICLE = LSM_GREYED + 1,
	LSM_TEXTURED = PARTICLE + 1,
	LSM_LITTLE_GUY = LSM_TEXTURED + 1,
	BG0 = LSM_LITTLE_GUY + 1,
	BG1 = BG0 + 1,
	BG2 = BG1 + 1,
	BG3 = BG2 + 1,
	HEALTHBAR = BG3 + 1,


	EFFECT_COUNT = HEALTHBAR + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
	SPRITE = 0,
	SPRITE_ANIMATION = SPRITE + 1,
	PEBBLE = SPRITE_ANIMATION + 1,
	DEBUG_LINE = PEBBLE + 1,
	SCREEN_TRIANGLE = DEBUG_LINE + 1,
	TEXT_BOX = SCREEN_TRIANGLE + 1,

	LOGO_TEXT = TEXT_BOX + 1,
	GREYED = LOGO_TEXT + 1,
	PARTICLE = GREYED + 1,
	HEALTHBAR = PARTICLE + 1,


	GEOMETRY_COUNT = HEALTHBAR + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct RenderRequest {
	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
	std::string z_val = layers[0];
};
