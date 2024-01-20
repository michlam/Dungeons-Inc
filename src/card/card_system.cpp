///////////////
//  imports  //
///////////////
#include "card_system.hpp"

// Declaration of card system. DON'T DECLARE IT ANYWHERE ELSE!
CardSystem card_system;

void CardSystem::init(RenderSystem* renderer_arg) {
    // create the deck and the required cards
  /*  CardEffect def_card_effect = create_card_effect(false, 0, 0, 1);
    CardEffect atk_card_effect = create_card_effect(false, 0, 1, 0);
    CardEffect hp_card_effect = create_card_effect(false, 1, 0, 0);

    Card basic_card = create_card("Basic Card", "A very simple beginner's card", BASIC, PLAN, def_card_effect);
    Card intermediate_card = create_card("Intermediate Card", "A card that has been improved.", INTERMEDIATE, PLAN, atk_card_effect);
    Card advanced_card = create_card("Advanced Card", "Super OP card!!!!", ADVANCED, COMBAT, hp_card_effect);

    std::map<Card, int> cards;
    cards[basic_card] = 3;
    cards[intermediate_card] = 2;
    cards[advanced_card] = 1;

    Entity deck = create_deck(cards, 6, DECK_POSITION);*/

    utils::load_deck_data(utils::get_curr_level());

    ////////////////
    // TEST CASES //
    ////////////////

    //// handle deck
    //Entity deck = create_deck(0, &registry.deck_entries.entities, DECK_POSITION);
    //registry.uiComponents.emplace(deck);

    //// create card
    //Entity card1 = create_card("Basic Card", "A very simple beginner's card", PLAN);
    //Entity card2 = create_card("Advanced Card", "Super OP card!!!!", COMBAT);

    //// add to deck
    //assert(add_card_to_deck(card1, 2, deck));
    //assert(add_card_to_deck(card2, 2, deck));

    //// verify deck pointer is working correctly
    ///*auto& deck_adr = registry.deck.get(deck);
    //auto cards_ptr1 = *(deck_adr.cards);
    //auto cards_ptr2 = registry.deck_entries.entities;

    //auto card1 = cards_ptr1[0];
    //auto card2 = cards_ptr2[0];

    //printf("Card 1 Name: %s \n", registry.cards.get(card1).name.c_str());
    //printf("Card 2 Name: %s \n", registry.cards.get(card2).name.c_str());*/

    //// create player's hand
    //Entity hand = create_hand(0, &registry.hand_entries.entities, HAND_POSITION);

    //// draw card to hand
    //assert(draw_card_from_deck(deck, hand));

    //// check player's hand and verify draw works
    //auto hand_list = *(registry.hand.get(hand).cards);
    ///*assert(registry.hand.get(hand).card_count == 1);
    //assert(registry.deck.get(deck).card_count == 3);*/
    //auto curr_card = hand_list[0];
    //auto card_name = registry.cards.get(curr_card).name;
    //auto card_description = registry.cards.get(curr_card).description;
    //auto card_phase = registry.cards.get(curr_card).phase;
    ///*assert(registry.hand_entries.get(curr_card).num_in_hand == 1);
    //assert(registry.deck_entries.get(curr_card).num_in_deck == 1);
    //printf("Card Name: %s \n", registry.cards.get(curr_card).name.c_str());*/

    //// play a card on a room
    //assert(apply_card_to_room(curr_card, hand, registry.rooms.entities[0]));

    //// verify that card was properly removed from the hand
    ///*hand_list = *(registry.hand.get(hand).cards);
    //assert(registry.hand.get(hand).card_count == 0);
    //assert(registry.cards_on_room.get(registry.rooms.entities[0]).num_of_instances == 1);
    //auto used_card = registry.cards_on_room.get(registry.rooms.entities[0]).card;
    //assert(registry.cards.get(used_card).name == card_name);
    //assert(registry.cards.get(used_card).description == card_description);
    //assert(registry.cards.get(used_card).phase == card_phase);*/

}