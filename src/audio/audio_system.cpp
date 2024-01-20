#include "audio_system.hpp"

Audio_system* Audio_system::audio_system = nullptr;

Audio_system::Audio_system() {
	//////////////////////////////////////
	// Loading music and sounds with SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Failed to initialize SDL Audio");
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
		fprintf(stderr, "Failed to open audio device");
	}
	this->bgm_on = true;
	this->sound_effect_on = true;

	intro_bgm = Mix_LoadWAV(audio_path("intro.wav").c_str());
	game_bgm = Mix_LoadMUS(audio_path("music.wav").c_str()); 
	draw = Mix_LoadWAV(audio_path("deck_shuffle.wav").c_str()); 
	door = Mix_LoadWAV(audio_path("explosion.wav").c_str());
	click_card = Mix_LoadWAV(audio_path("plin.wav").c_str()); 
	attach_card = Mix_LoadWAV(audio_path("jump.wav").c_str());
	merge_card = Mix_LoadWAV(audio_path("upgrade.wav").c_str());
	sword = Mix_LoadWAV(audio_path("sword.wav").c_str());
	shooting = Mix_LoadWAV(audio_path("shooting.wav").c_str());
	magic = Mix_LoadWAV(audio_path("magic.wav").c_str());
	adv_hurt = Mix_LoadWAV(audio_path("dead.wav").c_str());
	monster_hurt = Mix_LoadWAV(audio_path("hurt.wav").c_str());

}

Audio_system* Audio_system::get_audio() {
	if (!audio_system) {
		audio_system = new Audio_system();
	}
	return audio_system;
}


void Audio_system::loop_game_bgm() {
	Mix_PlayMusic(game_bgm, -1);
}

void Audio_system::set_bgm(bool new_status) {
	if (new_status == true) {
		Mix_ResumeMusic();
		loop_game_bgm();
		this->bgm_on = true;
	} else if (new_status == false) {
		Mix_PauseMusic();  // Pause the BGM
		this->bgm_on = false;
	}
}

void Audio_system::set_sound_effect(bool new_status) {
	this->sound_effect_on = new_status;
}



void Audio_system::play_adv_hurt() {
	if (sound_effect_on) {
		Mix_PlayChannel(-1, adv_hurt, 0);
	}
	
}

void Audio_system::play_monster_hurt() {
	if (sound_effect_on) {
		Mix_PlayChannel(-1, monster_hurt, 0);
	}
	
}

bool Audio_system::get_bgm_status() {
	return this->bgm_on;
}

bool Audio_system::get_sound_effect_status() {
	return this->sound_effect_on;
}


void Audio_system::play_intro() {
	if (sound_effect_on) {
		Mix_PlayChannel(-1, intro_bgm, 0);
	}
	
}

void Audio_system::play_draw() {
	if (sound_effect_on) {
		Mix_PlayChannel(-1, draw, 1);
	}
	
}

void Audio_system::play_door(){
	if (sound_effect_on) {
		Mix_PlayChannel(-1, door, 0);
	}
	
}

void Audio_system::play_click_card() {
	if (sound_effect_on) {
		Mix_PlayChannel(-1, click_card, 0);
	}
	
}

void Audio_system::play_attach_card() {
	if (sound_effect_on) {
		Mix_PlayChannel(-1, attach_card, 0);
	}
	
}

void Audio_system::play_merge_card() {
	if (sound_effect_on) {
		Mix_PlayChannel(-1, merge_card, 0);
	}
	
}

void Audio_system::play_sword() {
	if (sound_effect_on) {
		Mix_PlayChannel(-1, sword, 0);
	}
	

}

void Audio_system::play_shooting() {
	if (sound_effect_on) {
		Mix_PlayChannel(-1, shooting, 0);
	}
	
}

void Audio_system::play_magic() {
	if (sound_effect_on) {
		Mix_PlayChannel(-1, magic, 0);
	}
	
}

Audio_system::~Audio_system() {
	if (game_bgm != nullptr) {
		Mix_FreeMusic(game_bgm);
	}
	if (intro_bgm != nullptr) {
		Mix_FreeChunk(intro_bgm);
	}
	if (draw != nullptr) {
		Mix_FreeChunk(draw);
	}
	if (door != nullptr) {
		Mix_FreeChunk(door);
	}
	if (click_card != nullptr) {
		Mix_FreeChunk(click_card);
	}
	if (attach_card != nullptr) {
		Mix_FreeChunk(attach_card);
	}
	if (merge_card != nullptr) {
		Mix_FreeChunk(merge_card);
	}
	if (sword != nullptr) {
		Mix_FreeChunk(sword);
	}
	if (shooting != nullptr) {
		Mix_FreeChunk(shooting);
	}
	if (magic != nullptr) {
		Mix_FreeChunk(magic);
	}
    if (monster_hurt != nullptr) {
        Mix_FreeChunk(monster_hurt);
    }
    if (adv_hurt != nullptr) {
        Mix_FreeChunk(adv_hurt);
    }
	Mix_CloseAudio();
}
