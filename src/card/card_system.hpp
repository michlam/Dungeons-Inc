#pragma once

///////////////
//  imports  //
/////////////// 
// common
#include "common.hpp"
#include "utils.hpp"

#include "card/card_functions.hpp"
#include <phase/phase_components.hpp>


///////////////
//   class   //
/////////////// 
class CardSystem {
public:
	//CardSystem();

	// Releases all associated resources
	//~CardSystem();

	// creates a single card (for now)
	void init(RenderSystem* renderer_arg);

private:

};

// Declaration of card system. DON'T DECLARE IT ANYWHERE ELSE!
extern CardSystem card_system;