///////////////
//  imports  //
///////////////
#include "card/card_functions.hpp"
#include "level/level_system.hpp"



///////////////
// functions //
/////////////// 
Card create_card(std::string name, std::string description, card_level tier, phase_type phase, CardEffect effect) {
	// create the card entity
	Card new_card = Card();
	new_card.name = name;
	new_card.description = description;
	new_card.tier = tier;
	new_card.phase = phase;
	new_card.effect = effect;

	return new_card;
}


// set a card's effect to the given stat changes
CardEffect create_card_effect(bool is_buff, int hp_change, int atk_change, int def_change) {
	CardEffect effect = CardEffect();
	effect.is_buff = is_buff;
	effect.stat_effects.hp = hp_change;
	effect.stat_effects.atk = atk_change;
	effect.stat_effects.def = def_change;

	return effect;
}


// creates a deck of cards
Entity create_deck(std::map<Card, int> cards, int num_of_cards, vec2 position) {
	// create the entity and components with necessary values
	auto entity = Entity();
	auto& new_deck = registry.deck.emplace(entity);
	new_deck.cards = cards;
	new_deck.num_of_cards = num_of_cards;

	// hiding deck sprite for now because its kind of pointless to have
	//// render the deck sprite
	//registry.renderRequests.insert(
	//	entity,
	//	{ TEXTURE_ASSET_ID::DECK,
	//	 EFFECT_ASSET_ID::TEXTURED,
	//	 GEOMETRY_BUFFER_ID::SPRITE,
	//	"DECK" }
	//);

	//// position and scale the deck sprite
	//Motion& deck_position = registry.motions.emplace(entity);
	//deck_position.position = position;
	//deck_position.scale = { 150 , 200 };

	//// stickies the deck sprite to the game window
	//registry.uiComponents.emplace(entity);

	return entity;
}


//////////////////////////////
// AND AND REMOVE FUNCTIONS //
//////////////////////////////
// updates card effect based on new tier
void update_card_tier(Entity target_card, card_level tier) {
	Card& card = registry.cards.get(target_card);
	card.tier = tier;
	Stats& card_effect = card.effect.stat_effects;

	RenderRequest& rr = registry.renderRequests.get(target_card);
	Text& card_text = registry.texts.get(target_card);


	int multiplier = 1;
	switch (card.tier) {
	case INTERMEDIATE:
		multiplier = 3;
		break;
	case ADVANCED:
		multiplier = 5;
		break;
	default:
		// base multiplier, ie. 1
		break;
	}

	card_effect.hp = card_effect.hp * multiplier;
	card_effect.atk = card_effect.atk * multiplier;
	card_effect.def = card_effect.def * multiplier;

	// update text colour
	std::string card_tier = "NULL";
	vec3 text_color = { 0.0, 0.0, 0.0 };

	TEXTURE_ASSET_ID asset_id = TEXTURE_ASSET_ID::CARD1;
	

	switch (card.tier) {
	case BASIC:
		card_tier = "I";
		

		if (card_text.text_lines[0] == "SWORD BE GONE") {
			asset_id = TEXTURE_ASSET_ID::CARD_ATK1;
		}
		else if (card_text.text_lines[0] == "SHIELD BREAK") {
			asset_id = TEXTURE_ASSET_ID::CARD_DEF1;
		}
		else if (card_text.text_lines[0] == "WITHER") {
			asset_id = TEXTURE_ASSET_ID::CARD_HP1;
		}
		else if (card_text.text_lines[0] == "RECKONING DAY") {
			asset_id = TEXTURE_ASSET_ID::CARD_ALL1;
		}
		else {
			asset_id = TEXTURE_ASSET_ID::CARD1;
		}

		break;
	case INTERMEDIATE:
		card_tier = "II"; 
		if (card_text.text_lines[0] == "SWORD BE GONE") {
			asset_id = TEXTURE_ASSET_ID::CARD_ATK2;
		}
		else if (card_text.text_lines[0] == "SHIELD BREAK") {
			asset_id = TEXTURE_ASSET_ID::CARD_DEF2;
		}
		else if (card_text.text_lines[0] == "WITHER") {
			asset_id = TEXTURE_ASSET_ID::CARD_HP2;
		}
		else if (card_text.text_lines[0] == "RECKONING DAY") {
			asset_id = TEXTURE_ASSET_ID::CARD_ALL2;
		}
		else {
			asset_id = TEXTURE_ASSET_ID::CARD2;
		}
		//text_color = { 215, 248, 252 };
		break;
	case ADVANCED:
		card_tier = "III";
		if (card_text.text_lines[0] == "SWORD BE GONE") {
			asset_id = TEXTURE_ASSET_ID::CARD_ATK3;
		}
		else if (card_text.text_lines[0] == "SHIELD BREAK") {
			asset_id = TEXTURE_ASSET_ID::CARD_DEF3;
		}
		else if (card_text.text_lines[0] == "WITHER") {
			asset_id = TEXTURE_ASSET_ID::CARD_HP3;
		}
		else if (card_text.text_lines[0] == "RECKONING DAY") {
			asset_id = TEXTURE_ASSET_ID::CARD_ALL3;
		}
		else {
			asset_id = TEXTURE_ASSET_ID::CARD3;
		}
		//text_color = { 212, 165, 25 };
		break;
	default:
		break;
	}
	rr.used_texture = asset_id;



	Audio_system* audio_player = Audio_system::get_audio();
	audio_player->play_merge_card();

	std::string stat_symbol = (card.effect.is_buff) ? "+" : "-";

	std::string stat_symbol_hp = (card_effect.hp == 0) ? " " : stat_symbol;
	std::string stat_symbol_atk = (card_effect.atk == 0) ? " " : stat_symbol;
	std::string stat_symbol_def = (card_effect.def == 0) ? " " : stat_symbol;

	card_text.colour = text_color / 255.f;
	card_text.text_lines[1] = "Tier " + card_tier;
	card_text.text_lines[5] = " H/P ATK DEF ";
	card_text.text_lines[6] = " " + stat_symbol_hp + std::to_string(card_effect.hp) +
							  "  " + stat_symbol_atk + std::to_string(card_effect.atk) +
							  "  " + stat_symbol_def + std::to_string(card_effect.def);
}


// reformats the position of the cards in the player's hand based on the hand size
void reformat_cards(std::vector<Entity> entities, vec2 center_point) {
	// figure out the number of cards in the player's hand that aren't in play
	// start at -1 because we want 1 less than the size to determine the number of offsets
	int num_of_offsets = entities.size() - 1;
	int x_offset = 120 / 2;
	int curr_offset = num_of_offsets * x_offset;
	int curr_x_pos = center_point.x - curr_offset;

	for (Entity card : entities) {
		if (registry.motions.has(card) && registry.texts.has(card)) {
			auto& card_motion = registry.motions.get(card);
			auto& text_motion = registry.texts.get(card);
			card_motion.position = { curr_x_pos, center_point.y };

			vec2 scaler = vec2(1.f / 3.f);

			text_motion.position = { (2.095 * scaler.x) * curr_x_pos - (90 * scaler.x), (2.095 * scaler.y) * center_point.y - (110.5 * scaler.y) };

			curr_x_pos += 120;
		}
	}
}


void reformat_hand() {
	std::vector<Entity> cards_in_hand_only;
	for (Entity card : registry.hand.entities) {
		if (!registry.cards_in_play.has(card)) {
			cards_in_hand_only.push_back(card);
			if (!registry.uiComponents.has(card)) {
				registry.uiComponents.emplace(card);
			}
		}
	}

	reformat_cards(cards_in_hand_only, HAND_POSITION);
}


// adds a given card to the hand from a given deck
bool add_card_to_hand(Card target_card, RenderSystem* renderer) {
	// create card entity and add it to the player's hand
	Entity card_entity = Entity();
	Card& new_card = registry.cards.insert(card_entity, target_card);
	registry.hand.emplace(card_entity);

	// add motion to the newly-added card
	Motion& card_position = registry.motions.emplace(card_entity);
	card_position.position = HAND_POSITION;
	card_position.scale = { 120, 160 };

	

	// render the newly-added card
	// set to false to note that duplicate cards can be rendered
	Level& current_level_component = registry.levels.get(*current_level);
	bool is_tutorial = current_level_component.is_tutorial;
	enum class EFFECT_ASSET_ID effect_used = is_tutorial ? EFFECT_ASSET_ID::GLOWING : EFFECT_ASSET_ID::TEXTURED;

	// render the newly-added card
	// set to false to note that duplicate cards can be rendered



	TEXTURE_ASSET_ID asset_id = TEXTURE_ASSET_ID::CARD1;

	/*
	if (text.text_lines[0] == "SWORD BE GONE") {
		asset_id = TEXTURE_ASSET_ID::CARD_ATK1;
	}
	else if (text.text_lines[0] == "SHIELD BREAK") {
		asset_id = TEXTURE_ASSET_ID::CARD_DEF1;
	}
	else if (text.text_lines[0] == "WITHER") {
		asset_id = TEXTURE_ASSET_ID::CARD_HP1;
	}
	*/
	registry.renderRequests.insert(
		card_entity,
		{ asset_id,
		 effect_used,
		 GEOMETRY_BUFFER_ID::SPRITE,
		 "CARDS" }
	);

	// also create the text entity too, associated with the same entity
	Text text = Text{};

	text.text_lines[0] = registry.cards.get(card_entity).name;
	text.position = { 2.1 * card_position.position.x,  1.81 * card_position.position.y };
	text.scale = 0.865;

	auto& text_component = registry.texts.insert(card_entity, text);
	update_card_tier(card_entity, target_card.tier);
	// give the card ui-stickiness
	registry.uiComponents.emplace(card_entity);

	reformat_hand();

	return true;

}


// removes a given card from the hand
bool remove_card_from_hand(Entity target_card) {
	// check if card is in player's hand AND player's hand exists (ie. has size = 1)
	if (registry.hand.has(target_card)) {
		registry.hand.remove(target_card);
		return true;

		// card does not exist; return false
	}
	else {
		return false;
	}
}


void remove_card_from_deck(Card target_card) {
	// fetch the deck
	// quick check to verify that the deck exists
	assert(registry.deck.size() == 1);
	auto& deck = registry.deck.components[0];

	// find the quantity of the required card in the deck
	// quick check to verify that the card does exist in the deck
	assert(deck.cards.count(target_card) > 0);
	int& num_of_cards = deck.cards.at(target_card);

	// else, decrement by 1
	num_of_cards--;
	deck.num_of_cards--;
}


// randomly draws a card from the deck and adds it to the player's hand
bool draw_card_from_deck(RenderSystem* renderer) {
	// fetch deck and hand entities

	// NOTE: should i store the vector<entity> from the deck entries component container in deck?
	// eg. store a pointer to it in deck so that i dont have to access deck_entries for it 
	//     or store an actual reference to it
	auto& deck = registry.deck.components[0].cards;
	auto& deck_size = registry.deck.components[0].num_of_cards;
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// no more cards then return false!
	assert(deck_size > 0 && "No more cards in deck!");

	// determine card to draw
	int random_number = std::rand() % deck_size + 1;
	for (auto& card_iterator : deck) {

		// fetch current card entity
		Card curr_card = card_iterator.first;
		int num_of_card = card_iterator.second;

		if (random_number <= num_of_card && num_of_card != 0) {
			add_card_to_hand(curr_card, renderer);
			remove_card_from_deck(curr_card);
			return true;
		}
		else {
			random_number -= num_of_card;
			if (random_number <= 0) {
				return false;
			}
		}
	}

	return false;
}


bool remove_card_from_room(Entity target_card) {
	for (Entity room : registry.cards_on_room.entities) {
		std::vector<Entity>& cards_to_go_through = registry.cards_on_room.get(room).cards;
		for (int i = 0; i < cards_to_go_through.size(); i++) {
			Entity card = cards_to_go_through[i];
			if (card == target_card) {
				cards_to_go_through.erase(cards_to_go_through.begin() + i);

				// remove card from being in play
				registry.cards_in_play.remove(target_card);
				// change its layer (if needed)
				if (registry.renderRequests.has(target_card)) {
					registry.renderRequests.get(target_card).z_val = "CARDS";
				}
				return true;
			}
		}
	}

	// could not find card, so return false
	return false;
}


// applies the card to room
bool apply_card_to_room(Entity target_card, Entity target_room) {
	if (registry.hand.has(target_card)) {
		remove_card_from_room(target_card);

		// flag the card as in play so we know which cards to remove at the end of the game
		// can apply a card that was already in play so need to check for that
		if (!registry.cards_in_play.has(target_card)) {
			registry.cards_in_play.emplace(target_card);
		}

		// apply card to room
		// multiple components per room
		auto& card_to_room = registry.cards_on_room.has(target_room) ? registry.cards_on_room.get(target_room) : registry.cards_on_room.emplace(target_room);
		card_to_room.cards.push_back(target_card);
		registry.renderRequests.get(target_card).z_val = "CARDS_ON_ROOM";

		// since the card has been used, remove it from the player's hand
		// actually dont do this because cards shouldnt be used until the PLAN or COMBAT phases are over
		// remove_card_from_hand(target_card);

		Audio_system* audio_player = Audio_system::get_audio();
		audio_player->play_attach_card();

		return true;
	}

	return false;
}


card_level string_to_enum(std::string card_tier) {
	static const std::map<std::string, card_level> string_to_card_tier_map = {
		{"BASIC", BASIC},
		{"INTERMEDIATE", INTERMEDIATE},
		{"ADVANCED", ADVANCED}
	};

	auto it = string_to_card_tier_map.find(card_tier);
	if (it != string_to_card_tier_map.end()) {
		return it->second;
	}

	// Handle the case where the string doesn't map to any enum value
	throw std::invalid_argument("Invalid enum string: " + card_tier);
}


// helper to verify if two cards have the same effect
bool is_same_card_effect(const CardEffect& effect_1, const CardEffect& effect_2) {
	return (effect_1.is_buff == effect_2.is_buff &&
		effect_1.stat_effects.hp == effect_2.stat_effects.hp &&
		effect_1.stat_effects.atk == effect_2.stat_effects.atk &&
		effect_1.stat_effects.def == effect_2.stat_effects.def);
}


// checks if two given cards can be merged
bool is_mergeable(Entity wants_to_merge, Entity target_card) {
	// first check if the two entities are cards
	// cards can only be merged if they're currently in the player's hand
	if (registry.hand.has(wants_to_merge) && registry.hand.has(target_card)) {
		// first card is the card on top, ie. card being dragged
		Card& first_card = registry.cards.get(wants_to_merge);
		// second card is the card on the bottom, ie. not the card being dragged/stationary card
		Card& second_card = registry.cards.get(target_card);

		// next verify that all the fields are equivalent to each other
		if (first_card.name == second_card.name &&
			first_card.description == second_card.description &&
			first_card.tier == second_card.tier &&
			// need this check here because you cant merge two cards that are tier III cuz tier III the highest
			first_card.tier <= ADVANCED &&
			first_card.phase == second_card.phase &&
			is_same_card_effect(first_card.effect, second_card.effect)) {

			// if all equivalent, cards are mergeable
			return true;
		}
	}

	// some if-condition failed, so cards are not mergeable
	return false;
}


bool merge_cards(Entity wants_to_merge, Entity target_card) {
	// the second card (card on bottom) is what we use as the output card

	// check if cards can be merged
	if (is_mergeable(wants_to_merge, target_card)) {
		// if they can be merged, remove all existence of the first card
		remove_card_from_room(wants_to_merge);
		registry.remove_all_components_of(wants_to_merge);
		// is this how u delete entities??????
		// delete &wants_to_merge;

		// now "merge" the merged_card, ie. upgrade it to the next tier
		// because of is_mergeable, can confirm that cards here are tier II or lower
		Card& curr_card = registry.cards.get(target_card);
		card_level new_tier = BASIC;

		if (curr_card.tier == BASIC) {
			new_tier = INTERMEDIATE;
		} else {
			new_tier = ADVANCED;
		}

		update_card_tier(target_card, new_tier);

		return true;
	}

	// something went wrong or cards were not able to be merged; return false
	return false;
}