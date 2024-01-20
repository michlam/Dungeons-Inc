#pragma once
///////////////
//  imports  //
///////////////
#include "tiny_ecs_registry.hpp"
#include "./audio/audio_system.hpp"
#include "animation_system.hpp"



class CombatSystem {
private:
	Audio_system* audio_player;
public:

	// constructor
	CombatSystem() {
	}

	// flag character as InCombat
	void init_combat(Entity character);

	void calculate_damage(Entity adventurer, Entity mob, Entity room);

	void process_combat(float elapsed_ms_since_last_update);
	Entity CombatSystem::create_damage_number(vec2 pos, float num);

};


// Declaration of combat system. DON'T DECLARE IT ANYWHERE ELSE!
extern CombatSystem combat_system;

