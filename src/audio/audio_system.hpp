#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>
#include <cstdio>
#include <cstdlib>
#include "../common.hpp"
class Audio_system {

private:
	Audio_system();

	static Audio_system* audio_system;

	Mix_Chunk* intro_bgm;
	Mix_Music* game_bgm;
	Mix_Chunk* draw;
	Mix_Chunk* door;
	Mix_Chunk* click_card;
	Mix_Chunk* attach_card;
	Mix_Chunk* merge_card;
	Mix_Chunk* sword;
	Mix_Chunk* shooting;
	Mix_Chunk* magic;
	Mix_Chunk* adv_hurt;
	Mix_Chunk* monster_hurt;

	bool bgm_on;
	bool sound_effect_on;

public:
	static Audio_system* get_audio();

	void set_bgm(bool new_status);

	void set_sound_effect(bool new_status);

	void loop_game_bgm();

	void play_intro();
	
	void play_draw();

	void play_door();

	void play_click_card();

	void play_attach_card();

	void play_merge_card();

	void play_sword();

	void play_shooting();

	void play_magic();

	void play_adv_hurt();

	void play_monster_hurt();

	bool get_bgm_status();

	bool get_sound_effect_status();

	~Audio_system();
};