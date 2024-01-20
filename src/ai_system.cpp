///////////////
//  imports  //
/////////////// 
#include "ai_system.hpp"
#include <random>

AISystem* AISystem::ai_system = nullptr;

///////////////
// functions //
/////////////// 
void AISystem::step(float elapsed_ms) {

	(void)elapsed_ms;
}

void AISystem::init(Difficulty new_difficulty) {
	this->difficulty_level = new_difficulty;
}

int AISystem::random_integer(int lower_bound, int upper_bound) {
	std::random_device random;
	std::mt19937 mt(random());


	std::uniform_int_distribution<int> distribution(lower_bound, upper_bound);
	return distribution(mt);
}

AISystem::Difficulty AISystem::get_difficulty() {
	return this->difficulty_level;
}

Entity AISystem::move_to_next_room(Entity room) {
	if (registry.room.get(room).connections.size() == 0) {
		return room;
	}
	std::vector<Entity> connections = registry.room.get(room).connections;
	float minimum_damage = FLT_MAX;
	int branch_to_take = 0;
	int path_count = connections.size();

	if (this->difficulty_level == EASY) {
		return connections[random_integer(0, path_count - 1)];
	}
	else if (this->difficulty_level == MEDIUM) {
		int will_pick_optimal_path = random_integer(0, 1);
		if (!will_pick_optimal_path) {
			return connections[random_integer(0, path_count - 1)];
		}
	}

	for (int i = 0; i < path_count; i++) {
		Entity current_room = connections[i];

			float finalDmg = 0.f;
			if (registry.cards_on_room.has(current_room)) {
				auto& card_room_info = registry.cards_on_room.get(current_room);
				std::vector<Entity> cards_on_room = card_room_info.cards;
				for (Entity card_on_room : cards_on_room) {
					auto& current_card = registry.cards.get(card_on_room);
					// scalers applied to determine how impactful certain buffs are
					// flat hp down is pretty impactful, flat def down is impactful, and flat atk doesn't really affect adventurers too much
					finalDmg += current_card.effect.stat_effects.hp;
					finalDmg += current_card.effect.stat_effects.def / 2.f;
					finalDmg += current_card.effect.stat_effects.atk / 4.f;
				}
			}

			// Document current path when it is safer
			if (finalDmg < minimum_damage) {
				branch_to_take = i;
				minimum_damage = finalDmg;
			}
			printf("Branch #%i - ", i);
			printf("Final DMG: %f\n", finalDmg);

	}

	printf("Chosen Branch: %i\n", branch_to_take);
	return registry.room.get(room).connections[branch_to_take];
}

void AISystem::set_difficulty(AISystem::Difficulty new_difficulty) {
    this->difficulty_level = new_difficulty;
}

AISystem* AISystem::get_ai_system() {
	if (!ai_system) {
		//AISystem::Difficulty d = utils::load_AI_config();
		ai_system = new AISystem();
	}
	return ai_system;
}
