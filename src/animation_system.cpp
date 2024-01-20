#include "animation_system.hpp"


void AnimationSystem::step(float elapsed_ms) {
	float step_seconds = elapsed_ms / 1000.f;
	
	update_cutscene_animations();

	//update fading weapon animation
	auto& fade_weapon_container = registry.fadingWeapons;
	for (int i = static_cast<int>(fade_weapon_container.size()) - 1; i >= 0; --i) {
		Entity entity = fade_weapon_container.entities[i];
		FadingWeapon& fading_weapon = fade_weapon_container.components[i];

		float weapon_angle = fading_weapon.angle;
		fading_weapon.drop *= 1.05;
		Motion& weapon_motion = registry.motions.get(entity);

		if (cos(weapon_angle) > 0) {
			weapon_motion.angle += .05f;
			weapon_motion.position.x += 3 * cos(-M_PI/4);
			weapon_motion.position.y += 3 * sin(-M_PI/4) + fade_weapon_container.get(entity).drop;
			//weapon_motion.position.y += 3 * sin(weapon_angle) + fade_weapon_container.get(entity).drop;

		}
		else {
			weapon_motion.angle -= .05f;

			weapon_motion.position.x += 3 * cos(M_PI/4 * 5);
			weapon_motion.position.y += 3 * sin(M_PI/4 * 5) + fade_weapon_container.get(entity).drop;

		}
	}




	auto& damage_num_container = registry.damageNumbers;

	for (int i = static_cast<int>(damage_num_container.size()) - 1; i >= 0; --i) {
		Entity entity = damage_num_container.entities[i];

		Text& text = registry.texts.get(entity);
		DamageNumber& damage_num = damage_num_container.components[i];

		damage_num.decrement *= 0.8;

		text.position.y -= damage_num.decrement;
	}



	// update death animations/fading stuff
	auto& fade_container = registry.fading;
	for (int i = static_cast<int>(fade_container.size()) - 1; i >= 0; --i) {
		Entity entity = fade_container.entities[i];


		//Animation& animation = registry.animations.get(entity);
		Fading& fade = registry.fading.get(entity);

		fade.time_elapsed += elapsed_ms / 25;
		fade.fade_factor -= step_seconds;
	
		if (fade.fade_factor < 0) {
			
			/*
			// remove weapon components
			if (registry.characters.has(entity)) {
				registry.remove_all_components_of(registry.characters.get(entity).weapon);
			}
			*/
			registry.remove_all_components_of(entity);
		}
	}



	auto& characters_container = registry.characters;
	auto& stat_box_container = registry.statusBoxes;

	for (int i = static_cast<int>(characters_container.size()) - 1; i >= 0; --i) {
		//update health bar positions (this probably shouldn't be here but idk where else to update it)

		Entity character = characters_container.entities[i];
		Motion& character_motion = registry.motions.get(character);

		Entity hb = characters_container.get(character).health_bar;
		Motion& hb_motion = registry.motions.get(hb);
		hb_motion.position = { character_motion.position.x , character_motion.position.y - character_motion.scale.y / 2 };

	}




	// update blood particles
	auto& blood_container = registry.blood;
	for (int i = static_cast<int>(blood_container.size()) - 1; i >= 0; --i) {
		Entity entity = blood_container.entities[i];

		Blood& blood = registry.blood.get(entity);
		Motion& motion = registry.motions.get(entity);
		motion.velocity.y += step_seconds * 100.f;

		blood.time_elapsed += elapsed_ms;
		blood.alpha -= step_seconds;

		if (blood.time_elapsed > 1000) {
			registry.remove_all_components_of(entity);
		}
	}

	// update weapons
	auto& weapons = registry.weapons;
	for (int i = static_cast<int>(weapons.size()) - 1; i >= 0; --i) {
		Entity entity = weapons.entities[i];

		if (registry.swinging.has(entity)) { //animate swinging weapons
			animate_swing(entity, elapsed_ms);
		}
		else if (registry.casting.has(entity)) {
			animate_cast(entity, elapsed_ms);
		}
		else if (registry.shooting.has(entity)) {
			animate_shoot(entity, elapsed_ms);
		}
		else if (registry.stabbing.has(entity)) {
			animate_stab(entity, elapsed_ms);
		}
	}
};


void AnimationSystem::init() {
	// 
}


void AnimationSystem::animate_blood(Entity entity, float angle) {
	// generate 10 blood particles 
	for (uint i = 0; i < 10; i++) {
		Entity particle = create_blood_particle(registry.motions.get(entity).position, angle);
	}
};


void AnimationSystem::animate_death(Entity entity, float time_elapsed) {
	// 
};


void AnimationSystem::animate_swing(Entity entity, float time_elapsed) {
	Swinging& swinging = registry.swinging.get(entity);
	Motion& motion = registry.motions.get(entity);

	swinging.time_elapsed += time_elapsed / WEAPON_ANIM_TIME;
	if (swinging.time_elapsed > swinging.max_time) {
		//reset
		registry.swinging.remove(entity);
		motion.angle = (motion.scale.x / abs(motion.scale.x)) * M_PI / 8;
	}
	else {
		float angle = (sin(swinging.time_elapsed - M_PI / 2));
		float dir = (motion.scale.x / abs(motion.scale.x));
		//motion.angle = dir * M_PI / 8 + dir * (angle + dir) / 4;
		motion.angle = swinging.target_angle + dir * (angle + dir) / 4;

		
	}
};


void AnimationSystem::animate_cast(Entity entity, float time_elapsed) {
	Casting& casting = registry.casting.get(entity);
	Motion& motion = registry.motions.get(entity);

	casting.time_elapsed += time_elapsed / WEAPON_ANIM_TIME;
	if (casting.time_elapsed > casting.max_time) {
		//reset
		registry.casting.remove(entity);
		motion.angle = (motion.scale.x / abs(motion.scale.x)) * M_PI / 8;
	}
	else {
		float angle = (sin(casting.time_elapsed - M_PI / 2));
		float dir = (motion.scale.x / abs(motion.scale.x));
		//motion.angle = dir * M_PI / 8 +  dir * (angle + dir) / 4;
		motion.angle = casting.target_angle  + dir * (angle + dir) / 4;

	}
};


void AnimationSystem::animate_shoot(Entity entity, float time_elapsed) {
	Shooting& shooting = registry.shooting.get(entity);
	Motion& motion = registry.motions.get(entity);
	Animation& animation = registry.animations.get(entity);

	Entity parent = registry.weapons.get(entity).parent;
	Motion& parent_motion = registry.motions.get(parent);

	shooting.time_elapsed += time_elapsed / WEAPON_ANIM_TIME;
	if (shooting.time_elapsed > shooting.max_time) {
		//reset

		animation.animation_type = 0;
		animation.repeating = true;
		animation.ms_per_frame = 300;

		//need to resync animation with parent;
		animation.curr_frame = registry.animations.get(parent).curr_frame;
		animation.time_elapsed_ms = registry.animations.get(parent).time_elapsed_ms;

		registry.shooting.remove(entity);
		motion.angle = (motion.scale.x / abs(motion.scale.x)) * M_PI / 8;
		motion.position = shooting.original_pos;

	}
	else {
		float dir = (motion.scale.x / abs(motion.scale.x));
		motion.angle = shooting.target_angle;

		motion.position = { shooting.original_pos.x + 5 * dir, shooting.original_pos.y - 10};

		animation.animation_type = 1;
		animation.curr_frame = 2;

		animation.repeating = false;
		animation.ms_per_frame = 25;



	}
};


void AnimationSystem::animate_stab(Entity entity, float time_elapsed) {
	Stabbing& stabbing = registry.stabbing.get(entity);
	Motion& motion = registry.motions.get(entity);
	Animation& animation = registry.animations.get(entity);

	stabbing.time_elapsed += time_elapsed / WEAPON_ANIM_TIME;
	if (stabbing.time_elapsed > stabbing.max_time) {
		//reset
		
		animation.animation_type = 0;
		animation.repeating = true;
		animation.ms_per_frame = 300;
		
		registry.stabbing.remove(entity);

		motion.angle = (motion.scale.x / abs(motion.scale.x)) * M_PI / 6;

	}
	else {

		motion.angle = stabbing.target_angle;

		animation.animation_type = 1;
		//animation.curr_frame = 0;
		animation.repeating = false;
		animation.ms_per_frame = 30;
	}
};


Entity AnimationSystem::create_blood_particle(vec2 pos, float angle) {
	Entity entity = Entity();

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;

	
	float random_angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (M_PI/2) - (M_PI/4);

	float velocity_magnitude = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 50.0f + 100.0f;

	motion.velocity = { velocity_magnitude * cos(angle + random_angle), velocity_magnitude * sin(angle + random_angle) };

	motion.scale = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) + 0.5f) * vec2{ 5, 5 };

	
	Blood& blood = registry.blood.emplace(entity);

	/*
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::RED_PARTICLE, 
			EFFECT_ASSET_ID::PARTICLE,
			GEOMETRY_BUFFER_ID::SPRITE,
			layers[4] });
	*/
	return entity;
};



void AnimationSystem::update_cutscene_animations() {
	for (int i = 0; i < registry.cutscene_effects.size(); i++) {
		CutsceneEffect& effect = registry.cutscene_effects.components[i];
		Entity entity = registry.cutscene_effects.entities[i];

		if (effect.effect == "LOVE") {
			animate_love_effect(entity);
		}
		else if (effect.effect == "ANGER") {
			animate_anger_effect(entity);
		}
		else if (effect.effect == "CONFUSION") {
			animate_confusion_effect(entity);
		}
		else if (effect.effect == "SURPRISE") {
			animate_surprise_effect(entity);
		}
		else if (effect.effect == "NONE") {
			// don't do anything
		}
		else {
			assert("INVALID CUTSCENE EFFECT TYPE");
		}
	}
}


void AnimationSystem::animate_love_effect(Entity entity) {
	Motion& motion = registry.motions.get(entity);
	CutsceneEffect& effect = registry.cutscene_effects.get(entity);

	if (effect.ticks == 0) {
		effect.ticks = 201;
	}
	else if (effect.ticks == 150 || effect.ticks == 100) {
		motion.position.y += 5.f;
	}
	else if (effect.ticks == 200 || effect.ticks == 50) {
		motion.position.y -= 5.f;
	}

	--effect.ticks;
}


void AnimationSystem::animate_anger_effect(Entity entity) {
	Motion& motion = registry.motions.get(entity);
	CutsceneEffect& effect = registry.cutscene_effects.get(entity);

	if (effect.ticks == 0) {
		effect.ticks = 250;
	}
	else if (effect.ticks == 195 || effect.ticks == 190) {
		motion.position.y += 10.f;
	}
	else if (effect.ticks == 200 || effect.ticks == 185) {
		motion.position.y -= 10.f;
	}

	--effect.ticks;
}


void AnimationSystem::animate_confusion_effect(Entity entity) {
	Motion& motion = registry.motions.get(entity);
	CutsceneEffect& effect = registry.cutscene_effects.get(entity);

	if (effect.ticks == 0) {
		effect.ticks = 201;
	}
	else if (effect.ticks == 150 || effect.ticks == 100) {
		motion.angle += 0.5f;
	}
	else if (effect.ticks == 200 || effect.ticks == 50) {
		motion.angle -= 0.5f;
	}

	--effect.ticks;
}


void AnimationSystem::animate_surprise_effect(Entity entity) {
	Motion& motion = registry.motions.get(entity);
	CutsceneEffect& effect = registry.cutscene_effects.get(entity);

	if (effect.ticks == 0) {
		effect.ticks = 300;
	}
	else if (effect.ticks == 155 || effect.ticks == 130 || effect.ticks == 105) {
		motion.scale = { motion.scale.x - 10, motion.scale.y - 10 };
	}
	else if (effect.ticks == 200 || effect.ticks == 175 || effect.ticks == 165) {
		motion.scale = { motion.scale.x + 10, motion.scale.y + 10 };
	}

	--effect.ticks;
}