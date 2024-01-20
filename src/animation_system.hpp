#pragma once

#include "common.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"
#include "render_system.hpp"



const float WEAPON_ANIM_TIME = 25;


///////////////
//   class   //
/////////////// 


class AnimationSystem {
public:
	
	// Constructor
	AnimationSystem() {

	}

	void AnimationSystem::step(float elapsed_ms);

	void AnimationSystem::init();

	void AnimationSystem::animate_blood(Entity entity, float angle);
	void AnimationSystem::animate_death(Entity entity, float time_elapsed);

	void AnimationSystem::animate_swing(Entity entity, float time_elapsed);
	void AnimationSystem::animate_cast(Entity entity, float time_elapsed);
	void AnimationSystem::animate_shoot(Entity entity, float time_elapsed);
	void AnimationSystem::animate_stab(Entity entity, float time_elapsed);

	Entity AnimationSystem::create_blood_particle(vec2 pos, float angle);

	// weapons
	// Entity AnimationSystem::create_sword(Entity parent);


private:
	void AnimationSystem::update_cutscene_animations();

	void AnimationSystem::animate_love_effect(Entity entity);
	void AnimationSystem::animate_anger_effect(Entity entity);
	void AnimationSystem::animate_confusion_effect(Entity entity);
	void AnimationSystem::animate_surprise_effect(Entity entity);
};

extern AnimationSystem animation_system;
