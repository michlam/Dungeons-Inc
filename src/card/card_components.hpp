#pragma once

///////////////
//  imports  //
///////////////
#include <map>
#include "common.hpp"
#include "phase/phase_components.hpp"
#include "character_components.hpp"

///////////////
//  structs  //
/////////////// 
// represents a card's level
enum card_level {
    BASIC = 1,
    INTERMEDIATE = 2,
    ADVANCED = 3
};


// TODO: add effect_type enum with buff, debuff, status
struct CardEffect {
    bool is_buff;               // true if buff, false if debuff
    Stats stat_effects;         // stores stat-changing card effects
    // std::string character_type; // type of character the card can be played on
};


struct Card {
    std::string name;           // name of the card
    std::string description;    // card description

    card_level tier;            // tier (strength) of the card
    phase_type phase;           // which phase the card can be played in: planning or combat
    CardEffect effect;          // card's effects, self-explanatory

    // std::string image_path;     // relative path of the card's icon image 
    Text text;                // associated text box entity with the card --- !!!! TODO: extract behaviour

    bool operator<(const Card& card) const {
        if (name < card.name) {
            return true;
        }
        else if (name == card.name) {
            return description < card.description;
        }
        else {
            return false;
        }
    }
};


struct CardDeck {
    int num_of_cards;
    std::map<Card, int> cards; // mapping of card to quantity
};


struct PlayerHand {
    // flags which cards are in the player's hand
};

struct InPlay {
    // flags which cards are currently in play / on the board
};


struct HasCardApplied {
    std::vector<Entity> cards;  // which card was applied to the room
    int num_of_instances;       // number of times this exact card has been applied to the room
    std::set<Entity> entities;  // list of characters that the card effect has been applied to...
};