#pragma once

///////////////
//  imports  //
/////////////// 
// common
#include "nlohmann/json.hpp"
#include "phase/phase_components.hpp"
#include "ai_system.hpp"
// stdlib
#include <random>

class utils {
public:

	// Update the current level that the user is on
	static void utils::update_level(int level);

	// Load the meta data for a level
	static void utils::load_level_data(int level);

	// Load the deck data for a specific level
	static void utils::load_deck_data(int level);

	static void utils::update_audio_config(bool bgm_on, bool sound_effect_on);

	static void utils::load_audio_config();

	static void utils::update_AI_config(AISystem::Difficulty diff);

	static AISystem::Difficulty utils::load_AI_config();

	// Load genenric dialogue per level
	static std::map<phase_type, std::vector<std::string>> utils::load_dialogue_data();

	// static std::vector<std::string, CutsceneEvent> utils::load_cutscene_data(int cutscene_to_play);

	// Get the current level that the user is on
	static int utils::get_curr_level();

	// converts phase_type to string
	static std::string utils::phase_to_str(phase_type phase);

private:

	// Read json data from a specified file
	static nlohmann::json read_json_data(std::string file_path);
};