#pragma once


///////////////
//  imports  //
/////////////// 
#include <render_system.hpp>
#include <tiny_ecs_registry.hpp>
#include <phase/phase_system.hpp>
#include <utils.hpp>


///////////////
//   class   //
///////////////
class DialogueSystem {
public:
	DialogueSystem() {
		// empty constructor
	}

	void init(RenderSystem* renderer_arg);
	void step(float elapsed_ms_since_last_update);

	void set_standard_settings();
	void set_tutorial_settings();
	void set_cutscene_settings();
	bool is_complete();

	void make_player_say(std::string msg);
	void make_player_say_phase_quote(phase_type phase);
	void make_player_interrupt(std::string msg);

	void make_character_say(Entity entity, std::string msg);

	void show_advancement_sign();
	void hide_advancement_sign();

	void reset_dialogue_system();

	Entity speech_bubble;

private:
	RenderSystem* renderer;

	Entity player;
	Entity character;
	Entity character_name_bubble;
	Entity portrait;

	Entity advancement_sign;

	Entity create_player_sprite();
	Entity create_character_sprite(Entity entity);
	Entity create_character_portrait();

	Entity create_speech_bubble();
	void set_speech_bubble(std::string msg);
	void remove_speech_bubble(float elapsed_ms_since_last_update);
	void remove_speech_bubble_text();

	void reset_dialogue_timers();

	void set_current_text(std::string msg);
	void type_text();
	void render_speech_bubble(float elapsed_ms_since_last_update);
	void render_character_name(Entity entity);
};

extern DialogueSystem dialogue_system;