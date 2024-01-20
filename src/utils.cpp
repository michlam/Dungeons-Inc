#include "utils.hpp"
#include "phase/phase_system.hpp"
#include "level/level_system.hpp"
#include "./ai_system.hpp"
#include "cutscene/cutscene_components.hpp"


void utils::update_level(int level) {
	// Create a JSON object
	nlohmann::json data;

	// Add data to the JSON object
	data["curr_level"] = level;

	// Specify the path to the JSON file where you want to write the data
	std::string file_path = data_path() + "/save_data/user_data.json";

	// Open a file stream for writing
	std::ofstream outputFile(file_path);

	// Check if the file stream is open
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file: " << file_path << std::endl;
		return;
	}

	// Write the JSON data to the file
	outputFile << data.dump(4); // The "4" is for pretty printing with indentation

	// Close the file
	outputFile.close();
}

int utils::get_curr_level() {
	std::string user_data_file_path = data_path() + "/save_data/user_data.json";
	nlohmann::json json_data = read_json_data(user_data_file_path);
	return json_data["curr_level"];
}

void utils::load_level_data(int level) {
	std::string level_file_path = data_path() + "/save_data/level_data.json";

	nlohmann::json json_data = read_json_data(level_file_path);

	// Access and use the JSON data
	if (!json_data.is_null()) {

		// Access specific values in the JSON data
		nlohmann::json curr_level_data = json_data[std::to_string(level)];
		Entity* level_entity = level_list[level - 1];
		*level_entity = Entity();

		std::string name = curr_level_data["name"];
		int depth = curr_level_data["depth"];
		int difficulty = curr_level_data["difficulty"];
		bool is_unlocked = curr_level_data["is_unlocked"];
		bool is_tutorial = curr_level_data["is_tutorial"];

		Enemy enemy_1;
		Enemy enemy_2;
		Enemy enemy_3;
		Enemy enemy_4;
		Enemy* enemies[NUM_ENEMIES] = { &enemy_1, &enemy_2, &enemy_3, &enemy_4 };

		for (int i = 0; i < curr_level_data["enemies"].size(); i++) {
			enemies[i]->enemy_class = curr_level_data["enemies"][i]["enemy_class"];
			enemies[i]->difficulty_scale = curr_level_data["enemies"][i]["difficulty_scale"];
			enemies[i]->exp_dropped = curr_level_data["enemies"][i]["exp_dropped"];
			enemies[i]->is_alive = curr_level_data["enemies"][i]["is_alive"];
		}

		registry.levels.insert(*level_entity,
			{
				name,
				depth,
				difficulty,
				is_unlocked,
				is_tutorial,
				{
					enemy_1,
					enemy_2,
					enemy_3,
					enemy_4
				}
			}
		);

	}
	else {
		std::cerr << "Failed to parse JSON data from the file." << std::endl;
		return;
	}
}

void utils::load_deck_data(int level) {
	std::string level_file_path = data_path() + "/save_data/deck_data.json";

	nlohmann::json json_data = read_json_data(level_file_path);

	// Access and use the JSON data
	if (!json_data.is_null()) {

		// Access specific values in the JSON data
		std::map<Card, int> deck_cards;
		nlohmann::json deck_data = json_data[std::to_string(level)]["cards"];
		int total_cards = 0;
		for (auto& card : deck_data) {
			bool is_buff = card["card_effect"]["is_buff"];
			int hp_change = card["card_effect"]["hp_change"];
			int atk_change = card["card_effect"]["atk_change"];
			int def_change = card["card_effect"]["def_change"];
			CardEffect curr_effect = create_card_effect(is_buff, hp_change, atk_change, def_change);

			std::string name = card["name"];
			std::string description = card["description"];
			std::string phase_str = card["phase"];
			phase_type phase = phase_system.string_to_phase_type(phase_str);
			std::string tier_str = card["tier"];
			card_level tier = string_to_enum(tier_str);
			Card curr_card = create_card(name, description, tier, phase, curr_effect);

			int num_cards = card["num_cards"];
			deck_cards.insert(std::make_pair(curr_card, num_cards));
			total_cards += num_cards;
		}

		create_deck(deck_cards, total_cards, DECK_POSITION);
	}
	else {
		std::cerr << "Failed to parse JSON data from the file." << std::endl;
		return;
	}
}

// Code copied from above
void utils::update_audio_config(bool bgm_on, bool sound_effect_on) {
	// Create a JSON object
	nlohmann::json data;

	// Add data to the JSON object
	data["bgm_on"] = bgm_on;
	data["sound_effect_on"] = sound_effect_on;

	// Specify the path to the JSON file where you want to write the data
	std::string file_path = data_path() + "/save_data/audio_config.json";

	// Open a file stream for writing
	std::ofstream outputFile(file_path);

	// Check if the file stream is open
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file: " << file_path << std::endl;
		return;
	}

	// Write the JSON data to the file
	outputFile << data.dump(4); // The "4" is for pretty printing with indentation

	// Close the file
	outputFile.close();
}

// Code copied from above
void utils::load_audio_config() {
	std::string level_file_path = data_path() + "/save_data/audio_config.json";

	nlohmann::json json_data = read_json_data(level_file_path);

	// Access and use the JSON data
	if (!json_data.is_null()) {
		Audio_system* audio_player = Audio_system::get_audio();
		audio_player->set_bgm(json_data["bgm_on"]);
		audio_player->set_sound_effect(json_data["sound_effect_on"]);
	}
}

void utils::update_AI_config(AISystem::Difficulty diff) {
	// Create a JSON object
	nlohmann::json data;

	std::string difficulty_string = "MEDIUM";
	switch (diff) {
	case AISystem::EASY:
		difficulty_string = "EASY";
		break;
	case AISystem::MEDIUM:
		difficulty_string = "MEDIUM";
		break;
	case AISystem::HARD:
		difficulty_string = "HARD";
		break;
	default:
		difficulty_string = "MEDIUM";
	}

	// Add data to the JSON object
	data["difficulty"] = difficulty_string;
	
	// Specify the path to the JSON file where you want to write the data
	std::string file_path = data_path() + "/save_data/AI_config.json";

	// Open a file stream for writing
	std::ofstream outputFile(file_path);

	// Check if the file stream is open
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file: " << file_path << std::endl;
		return;
	}

	// Write the JSON data to the file
	outputFile << data.dump(4); // The "4" is for pretty printing with indentation

	// Close the file
	outputFile.close();
}

AISystem::Difficulty utils::load_AI_config() {
	std::string level_file_path = data_path() + "/save_data/AI_config.json";

	nlohmann::json json_data = read_json_data(level_file_path);

	// Access and use the JSON data
	if (!json_data.is_null()) {
		std::string loaded_difficulty_str;
		json_data["difficulty"].get_to(loaded_difficulty_str);

		if (loaded_difficulty_str == "EASY") {
			return AISystem::EASY;
		}
		else if (loaded_difficulty_str == "MEDIUM") {
			return AISystem::MEDIUM;
		}
		else if (loaded_difficulty_str == "HARD") {
			return AISystem::HARD;
		}
		else {
			return AISystem::MEDIUM;
		}
	}
	return AISystem::MEDIUM;
}


std::map<phase_type, std::vector<std::string>> utils::load_dialogue_data() {
	std::string dialogue_file_path = data_path() + "/save_data/dialogue_data.json";

	nlohmann::json json_data = read_json_data(dialogue_file_path);

	std::vector<phase_type> all_phases = { START, DRAW, PLAN, PRE_COMBAT, COMBAT, DAMAGE, END };
	std::map<phase_type, std::vector<std::string>> results;

	if (!json_data.is_null()) {
		for (phase_type phase : all_phases) {
			std::vector<std::string> curr_dialogue = json_data[phase_to_str(phase)]["dialogue"];
			results[phase] = curr_dialogue;
		}
	}
	else {
		std::cerr << "Failed to parse JSON data from the file." << std::endl;
	}

	return results;
}


// std::vector<std::string, CutsceneEvent> utils::load_cutscene_data(int cutscene_to_play) {
	/*
	std::string cutscene_file_path = data_path() + "/save_data/cutscene_data.json";

	nlohmann::json json_data = read_json_data(cutscene_file_path);

	std::vector<std::string, CutsceneEvent> results;

	if (!json_data.is_null()) {
		nlohmann::json cutscene_to_play_json = json_data[std::to_string(cutscene_to_play)]["events"];

		if (!cutscene_to_play_json.is_null()) {
			for (nlohmann::json data : cutscene_to_play_json) {
				std::string entity = data["entity"];
				printf("Entity Name: %s", entity.c_str());

				std::vector<Action> actions;
				std::vector<std::string> dialogues = data["dialogue"];

				for (nlohmann::json action_data : data["actions"]) {
					Action action;
					Motion motion;
					Movement movement;

					motion.position.x = action_data["motion"]["position"]["x"];
					motion.position.y = action_data["motion"]["position"]["y"];

					motion.scale.x = action_data["motion"]["scale"]["x"];
					motion.scale.y = action_data["motion"]["scale"]["y"];

					movement.goal_pos.x = action_data["movement"]["goal_pos"]["x"];
					movement.goal_pos.y = action_data["movement"]["goal_pos"]["y"];

					action.motion = motion;
					action.movement = movement;

					actions.push_back(action);
				}

				printf("Motion 1: %i", actions[0].motion.position.x);
				printf("Motion 2: %i", actions[0].motion.scale.x);
				printf("Movement: %i", actions[0].movement.goal_pos.x);
				printf("Dialogue: %s", dialogues[0].c_str());
				
			}
		}
		else {
			std::cerr << "No cutscene data available for specified cutscene number." << std::endl;
		}
	}
	else {
		std::cerr << "Failed to parse JSON data from the file." << std::endl;
	}

	return results;
	*/
// }


nlohmann::json utils::read_json_data(std::string file_path) {
	// Create an ifstream object to read from the file
	std::ifstream inputFile(file_path);

	// Check if the file is open
	if (!inputFile.is_open()) {
		std::cerr << "Error opening file: " << file_path << std::endl;
		nlohmann::json empty;
		return empty;
	}

	// Parse the JSON data from the file
	nlohmann::json json_data;
	inputFile >> json_data;

	// Close the file
	inputFile.close();

	return json_data;
}


// converts phase type to string form
std::string utils::phase_to_str(phase_type phase) {
	std::string str_to_return;

	switch (phase) {
	case START:
		str_to_return = "START";
		break;
	case DRAW:
		str_to_return = "DRAW";
		break;
	case PLAN:
		str_to_return = "PLAN";
		break;
	case PRE_COMBAT:
		str_to_return = "PRE_COMBAT";
		break;
	case COMBAT:
		str_to_return = "COMBAT";
		break;
	case DAMAGE:
		str_to_return = "DAMAGE";
		break;
	case END:
		str_to_return = "END";
		break;
	default:
		str_to_return = "ERROR";
		break;
	}

	return str_to_return;
}