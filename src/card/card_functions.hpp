#pragma once

///////////////
//  imports  //
///////////////
// common
#include "common.hpp"
#include "tiny_ecs_registry.hpp"

// systems
#include "render_system.hpp"
#include "../audio/audio_system.hpp"

// components
#include <phase/phase_components.hpp>


///////////////
// constants //
///////////////
const vec2 DECK_POSITION = { default_window_size.x - (150 / 2) - 10, default_window_size.y - (200 / 2) - 10 };
const vec2 HAND_POSITION = { 600, 800 - (200 / 4) };

///////////////
// functions //
/////////////// 
// create functions
// Entity create_card(std::string name, std::string description, phase_type phase, vec2 position);

Card create_card(std::string name, std::string description, card_level tier, phase_type phase, CardEffect effect);

CardEffect create_card_effect(bool is_buff, int hp_change, int atk_change, int def_change);

Entity create_deck(std::map<Card, int> cards, int num_of_cards, vec2 position);

// add/remove functions
void reformat_cards(std::vector<Entity> entities, vec2 center_point);

void reformat_hand();

bool add_card_to_hand(Card target_card, RenderSystem* renderer);

void update_card_tier(Entity target_card, card_level tier);

bool remove_card_from_hand(Entity target_card);

bool draw_card_from_deck(RenderSystem* renderer);

bool remove_card_from_room(Entity target_card);

bool apply_card_to_room(Entity target_card, Entity target_character);

card_level string_to_enum(std::string card_tier);

bool is_mergeable(Entity to_be_merged, Entity check_if_mergeable);

bool merge_cards(Entity card_to_merge, Entity merged_card);