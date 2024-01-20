#pragma once

///////////////
//  imports  //
///////////////
// common
#include "common.hpp"
#include "tiny_ecs.hpp"

// stdlib
#include <array>
#include <utility>
#include <map>

// components
#include "components.hpp"
#include <freetype/freetype.h>


///////////////
//   class   //
/////////////// 
class RenderSystem {

// System responsible for setting up OpenGL and for rendering all the
// visual entities in the game
	/**
	 * The following arrays store the assets the game will use. They are loaded
	 * at initialization and are assumed to not be modified by the render loop.
	 *
	 * Whenever possible, add to these lists instead of creating dynamic state
	 * it is easier to debug and faster to execute for the computer.
	 */
	std::array<GLuint, texture_count> texture_gl_handles;
	std::array<ivec2, texture_count> texture_dimensions;

	// Make sure these paths remain in sync with the associated enumerators.
	// Associated id with .obj path
	const std::vector < std::pair<GEOMETRY_BUFFER_ID, std::string>> mesh_paths =
	{
		std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::LOGO_TEXT, mesh_path("logo.obj"))
	};

	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, texture_count> texture_paths = {
		// ui
		textures_path("ui_window.png"),

		// dialogue
		textures_path("player_portrait.png"),
		textures_path("speech bubble.png"),
		textures_path("character name bubble.png"),
		textures_path("cutscene speech bubble.png"),
		textures_path("cutscene choice bubble.png"),
		textures_path("tutorial speech bubble.png"),
		textures_path("dungeon_background_cutscene.png"),

		// cutscene emotions
		textures_path("cutscene_actions/love.png"),
		textures_path("cutscene_actions/anger.png"),
		textures_path("cutscene_actions/confusion.png"),
		textures_path("cutscene_actions/surprise.png"),

		// phase
		textures_path("pause_overlay.png"),
		textures_path("play_overlay.png"),

		textures_path("phase_ui.png"),

		// dungeon map
		textures_path("dungeon_background.jpg"),
		textures_path("dungeon_room.png"),

		// card system
		textures_path("cards/wither_card1.png"),
		textures_path("cards/wither_card2.png"),
		textures_path("cards/wither_card3.png"),

		textures_path("cards/shield_break_card1.png"),
		textures_path("cards/shield_break_card2.png"),
		textures_path("cards/shield_break_card3.png"),

		textures_path("cards/sword_be_gone_card1.png"),
		textures_path("cards/sword_be_gone_card2.png"),
		textures_path("cards/sword_be_gone_card3.png"),

		textures_path("cards/reckoning_day_card1.png"),
		textures_path("cards/reckoning_day_card2.png"),
		textures_path("cards/reckoning_day_card3.png"),

		textures_path("cards/card1.png"),
		textures_path("cards/card2.png"),
		textures_path("cards/card3.png"),

		textures_path("deck_cover.png"),
		textures_path("hand_empty.jpg"),

		// characters
		textures_path("character_sheets/warrior sheet.png"),
		textures_path("character_sheets/mage sheet.png"),
		textures_path("character_sheets/ranger sheet.png"),
		textures_path("character_sheets/barbarian sheet.png"),
		textures_path("character_sheets/brute sheet.png"),
		textures_path("character_sheets/shaman sheet.png"),
		textures_path("character_sheets/lilguy sheet.png"),

		// intro textures
		textures_path("intro/window.png"),
		textures_path("intro/running goblin.png"),
		textures_path("intro/ground.png"),

		// level select menu textures
		textures_path("level_select/lsm_bg_1.png"),
		textures_path("level_select/lsm_bg_2.png"),
		textures_path("level_select/lsm_bg_3.png"),
		textures_path("level_select/lsm_bg_4.png"),
		textures_path("level_select/window.png"),
		textures_path("level_select/button.png"),
		textures_path("level_select/popup_window.png"),
		textures_path("level_select/start_button.png"),
		textures_path("level_select/popup_tutorial.png"),
		textures_path("level_select/popup_1.png"),
		textures_path("level_select/popup_2.png"),
		textures_path("level_select/popup_3.png"),
		textures_path("level_select/popup_4.png"),
		textures_path("level_select/popup_5.png"),
		textures_path("level_select/popup_6.png"),
		textures_path("level_select/popup_7.png"),
		textures_path("level_select/lsm_button_tutorial.png"),
		textures_path("level_select/lsm_button_one.png"),
		textures_path("level_select/lsm_button_two.png"),
		textures_path("level_select/lsm_button_three.png"),
		textures_path("level_select/lsm_button_four.png"),
		textures_path("level_select/lsm_button_five.png"),
		textures_path("level_select/lsm_button_six.png"),
		textures_path("level_select/lsm_button_seven.png"),
		textures_path("level_select/level_select.png"),
		textures_path("level_select/selector_testing.png"),

		// map switching textures
		textures_path("switch_map_view_button.png"),
		textures_path("map_bg.png"),

		//textures_path("black_background.jpg"),
		textures_path("dungeon_room_on_overall_map_view2.png"),

		// particles and other stuff??
		textures_path("red particle.png"),
		textures_path("blue particle.png"),
		textures_path("transparent.png"),


		// health bars
		textures_path("mob healthbar.png"),
		textures_path("adventurer healthbar.png"),

		// weapons
		textures_path("weapons/arrow.png"),
		textures_path("weapons/club.png"),
		textures_path("weapons/bow.png"),
		textures_path("weapons/spear.png"),
		textures_path("weapons/staff.png"),
		textures_path("weapons/staff2.png"),
		textures_path("weapons/sword.png"),

		// win/loss textures
		textures_path("victory.png"),
		textures_path("defeat.png"),

		// menu textures
		textures_path("menu/open_menu_button.png"),
		textures_path("menu/menu_ui.png"),
		textures_path("menu/reset_data_button.png"),
		textures_path("menu/exit_level_button.png"),
		textures_path("menu/menu_settings_button.png"),
		textures_path("menu/close_menu_button.png"),
		textures_path("menu/music_enabled_icon.png"),
		textures_path("menu/music_disabled_icon.png"),
		textures_path("menu/sfx_enabled_icon.png"),
		textures_path("menu/sfx_disabled_icon.png"),
		textures_path("menu/settings_ui.png"),
		textures_path("menu/settings_back_button.png"),
		textures_path("menu/diff_1_icon.png"),
		textures_path("menu/diff_2_icon.png"),
		textures_path("menu/diff_3_icon.png"),

		textures_path("completed_rooms/dungeon_map_L.png"),
		textures_path("completed_rooms/dungeon_map_M.png"),
		textures_path("completed_rooms/dungeon_map_R.png"),
		textures_path("completed_rooms/dungeon_map_LM.png"),
		textures_path("completed_rooms/dungeon_map_MR.png"),
		textures_path("completed_rooms/dungeon_map_LR.png"),
		textures_path("completed_rooms/dungeon_map_LMR.png"),
		textures_path("completed_rooms/dungeon_map_NONE.png")
	};

	std::array<GLuint, effect_count> effects;
	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, effect_count> effect_paths = {
		shader_path("coloured"),
		shader_path("text"),
		shader_path("textured"),
		shader_path("textured_animation"),
		shader_path("water"),
		shader_path("pebble"),
		shader_path("glowing"),
		shader_path("logo"),
		shader_path("lsm_greyed"),
		shader_path("particle"),
		shader_path("lsm_textured"),
		shader_path("lsm_little_guy"),
		
		shader_path("bg0"),
		shader_path("bg1"),
		shader_path("bg2"),
		shader_path("bg3"),
		shader_path("healthbar"),



	};

	std::array<GLuint, geometry_count> vertex_buffers;
	std::array<GLuint, geometry_count> index_buffers;
	std::array<Mesh, geometry_count> meshes;

	// Store FreeType's character data in a map
	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	std::map<char, Character> Characters;

public:
	// important fields
	mat3 projection_2D;
	mat3 projection_view_matrix;

	// Initialize the window
	bool init(GLFWwindow* window);

	template <class T>
	void bindVBOandIBO(GEOMETRY_BUFFER_ID gid, std::vector<T> vertices, std::vector<uint16_t> indices);

	template<class T>
	void dynamic_bind_vbo_and_ibo(GEOMETRY_BUFFER_ID gid, std::vector<T> vertices, std::vector<uint16_t> indices);

	void initializeGlTextures();

	void initializeGlEffects();

	void initializeGlMeshes();
	Mesh& getMesh(GEOMETRY_BUFFER_ID id) { return meshes[(int)id]; };

	void initializeGlGeometryBuffers();
	// Initialize the screen texture used as intermediate render target
	// The draw loop first renders to this texture, then it is used for the water
	// shader
	bool initScreenTexture();

	// Initialize the font face
	int init_text_fonts();

	// Destroy resources associated to one or all entities created by the system
	~RenderSystem();

	// Draw all entities
	void draw();

	// helper funcs
	void renderEntity(Entity entity, mat3 projection_matrix);
	mat3 createProjectionMatrix();

	void on_keyboard(int key, int, int action, int mod);
	void on_mouse_click(GLFWwindow* window, int key, int action, int mod);
	void on_mouse_move(vec2 pos);

	static void handle_char_hover(vec2 mouse_position);


private:
	// Internal drawing functions for each entity type
	void drawTexturedMesh(Entity entity, const mat3& projection);
	void draw_particles(const mat3& projection);
	void drawAnimatedSprite(Entity entity);

	void drawToScreen();
	void draw_text(Entity entity, const mat3& projection);

	void handle_card_drag(vec2 mouse_position);

	// level selector
	void handle_lsm_selector_drag(vec2 mouse_position);
	void lsm_mouse_click_handler(GLFWwindow* window, int key, int action, int mod);
	void lsm_selector_mouse_click_handler(GLFWwindow* window, int key, int action, int mod);

	void map_view_mouse_click_handler(GLFWwindow* window, int key, int action, int mod);

	// menu 
	void menu_mouse_click_handler(GLFWwindow* window, int key, int action, int mod);


	// Window handle
	GLFWwindow* window;

	// Screen texture handles
	GLuint frame_buffer;
	GLuint off_screen_render_buffer_color;
	GLuint off_screen_render_buffer_depth;

	Entity screen_state_entity;
};

bool loadEffectFromFile(const std::string& vs_path, const std::string& fs_path, GLuint& out_program);
void reset_camera_position();
void update_camera_position(float xpos, float ypos);
vec2 get_camera_matrix();

// Enum of renderable scenes
const enum scene {
	INTRO, LEVEL_SELECT, CUTSCENE, IN_GAME, IN_FULL_MAP_VIEW
};

// A string to hold the current scene
extern scene current_scene;

