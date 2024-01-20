///////////////
//  imports  //
///////////////
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"

///////////////
// functions //
/////////////// 

/**
* Assign stats based on class of adventurer
* 
**/
void assignStats(Entity adventurer, int difficulty_scale) {
	srand(time(nullptr)); // sets the seed to generate random numbers
	auto& character = registry.characters.get(adventurer);
	Stats& stats = character.stats;
	Class& advClass = registry.adventurerClass.get(adventurer);
	switch (advClass.advClass) {
		case Class::WARRIOR:
			stats.hp = rand() % ((int)(stats.statSpread * 0.6 - stats.statSpread * 0.5 + 1)) + stats.statSpread * 0.5;
			stats.atk = rand() % ((int)(stats.statSpread * 0.07 - stats.statSpread * 0.05 + 1)) + stats.statSpread * 0.05;
			stats.def = stats.statSpread - stats.hp - stats.atk;
			break;
		case Class::MAGE:
			stats.hp = rand() % ((int)(stats.statSpread * 0.3 - stats.statSpread * 0.25 + 1)) + stats.statSpread * 0.25;
			stats.atk = rand() % ((int)(stats.statSpread * 0.7 - stats.statSpread * 0.6 + 1)) + stats.statSpread * 0.6;
			stats.def = stats.statSpread - stats.hp - stats.atk;
			break;
		case Class::RANGER:
			stats.hp = rand() % ((int)(stats.statSpread * 0.4 - stats.statSpread * 0.3 + 1)) + stats.statSpread * 0.3;
			stats.atk = rand() % ((int)(stats.statSpread * 0.55 - stats.statSpread * 0.45 + 1)) + stats.statSpread * 0.45;
			stats.def = stats.statSpread - stats.hp - stats.atk;
			break;
	}

	stats.hp *= difficulty_scale;
	stats.atk *= difficulty_scale;
	stats.def *= difficulty_scale;
}

/**
* Creates an adventurer entity with render and animation, assigned stats based on class and difficulty_scale
*  renderer: gets mesh for sprite animation
*  pos: position of the entity
*  class: class enumeration of the adventurer defined in character components
**/
Entity createAdventurer(RenderSystem* renderer, vec2 pos, Class::CLASS adv_class, Enemy enemy_data) {
	Entity adventurer = Entity();

	// create mesh or sprite of adventurer here; basic template copied from A1
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE_ANIMATION);
	registry.meshPtrs.emplace(adventurer, &mesh);

	// initial motion values; note we are using Motion for now but will change to pos and scale later
	Motion& motion = registry.motions.emplace(adventurer);
	motion.position = pos;
	motion.scale = vec2(40.f, 56.f);

	Animation& animations = registry.animations.emplace(adventurer);

	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 4;
	animations.time_elapsed_ms = 0;
	animations.repeating = true;

	auto& character = registry.characters.emplace(adventurer);

	Class& adventurerClass = registry.adventurerClass.emplace(adventurer);
	adventurerClass.advClass = adv_class;

	Enemy& enemyClass = registry.enemy.insert(adventurer, enemy_data);
	assignStats(adventurer, enemy_data.difficulty_scale);

	Entity hb = createHealthBar(renderer, adventurer, true);
	character.health_bar = hb;

	character.stat_box = createStatBox(renderer, motion.position);

	TEXTURE_ASSET_ID textureAsset = TEXTURE_ASSET_ID::WARRIOR;

	switch (adv_class) {
	case Class::WARRIOR:
		textureAsset = TEXTURE_ASSET_ID::WARRIOR;
		character.weapon = createSword(adventurer);
		break;
	case Class::MAGE:
		textureAsset = TEXTURE_ASSET_ID::MAGE;
		character.weapon = createStaff(adventurer);

		break;
	case Class::RANGER:
		textureAsset = TEXTURE_ASSET_ID::RANGER;
		character.weapon = createBow(adventurer);

		break;
	default:
		break;
	}

	registry.renderRequests.insert(
		adventurer,
		{ textureAsset,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"CHARACTERS" });

	return adventurer;
}


//'goblins' only right now
Entity createMob(RenderSystem* renderer, vec2 pos, MobClass::MOBCLASS mob_class) {
	Entity mob = Entity();

	// create mesh or sprite of mob here; basic template copied from A1
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE_ANIMATION);
	registry.meshPtrs.emplace(mob, &mesh);

	Motion& motion = registry.motions.emplace(mob);
	motion.position = pos;
	motion.scale = vec2(40.f, 48.f);

	auto& character = registry.characters.emplace(mob);
	registry.mob.emplace(mob);
	Stats& stats = character.stats;
	stats.atk = 20;
	stats.def = 1;
	stats.hp = 500;

	character.health_bar = createHealthBar(renderer, mob, false);

	character.stat_box = createStatBox(renderer, motion.position);

	Animation& animations = registry.animations.emplace(mob);

	animations.sprite_h = 1;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 4;
	animations.num_frames[2] = 3;

	animations.time_elapsed_ms = 0;
	animations.repeating = true;

	MobClass& mobClass = registry.mobClass.emplace(mob);
	mobClass.mobClass = mob_class;

	//character.weapon = createSpear(mob);
	//todo : different sprites for different mobs, currently just goblins


	TEXTURE_ASSET_ID textureAsset;

	switch (mob_class) {
	case MobClass::BARBARIAN:
		textureAsset = TEXTURE_ASSET_ID::BARBARIAN;
		character.weapon = createSpear(mob);
		break;
	case MobClass::BRUTE:
		textureAsset = TEXTURE_ASSET_ID::BRUTE;
		character.weapon = createClub(mob);

		break;
	case MobClass::SHAMAN:
		textureAsset = TEXTURE_ASSET_ID::SHAMAN;
		character.weapon = createStaff2(mob);

		break;
	default:
		break;
	}

	registry.renderRequests.insert(
		mob,
		{ textureAsset,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"CHARACTERS" });

	return mob;
}


Entity createStatBox(RenderSystem* renderer, vec2 pos) {
	Entity entity = Entity();

	// create mesh or sprite of mob here; basic template copied from A1
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	StatusBox& statusBox = registry.statusBoxes.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = vec2(STATBOX_WIDTH, STATBOX_HEIGHT);

	/*
	registry.renderRequests.insert(
		entity,  
		{ TEXTURE_ASSET_ID::UI_WINDOW,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"CARDS_ON_ROOM" });
	*/
	//registry.uiComponents.emplace(entity);

	return entity;
}


/*
//bad very bad -- do not use
Entity createAdvStatText(Entity stat_box_entity, vec2 pos, Stats stats, Stats stat_changes, Class::CLASS advClass) {
	// Initialize the text.
	Text text = Text{};

	char class_name[10];

	switch (advClass) {
	case Class::WARRIOR:
		strcpy(class_name, "WARRIOR");
		break;
	case Class::MAGE:
		strcpy(class_name, "MAGE");
		break;
	case Class::RANGER:
		strcpy(class_name, "RANGER");
		break;
	default:
		strcpy(class_name, "!!ERROR!!");
		break;
	}

	text.text_lines[0] = class_name;
	text.text_lines[1] = "hp: " + std::to_string(stats.hp + stat_changes.hp);
	text.text_lines[2] = "atk: " + std::to_string(stats.atk + stat_changes.atk);
	text.text_lines[3] = "def: " + std::to_string(stats.def + stat_changes.def);

	text.colour = { 0.0, 0.0, 0.0 };

	text.scale = 1;
	vec2 scaler = vec2(1.f / 3.f );
	text.position = { (2 * scaler.x) * pos.x - ((STATBOX_WIDTH / 2.5) * scaler.x), (2 * scaler.y) * pos.y - ((STATBOX_HEIGHT / 2) * scaler.y) };
	// text.position = pos* vec2(0.67, 0.67) + vec2(-30, -20);
	registry.texts.insert(stat_box_entity, text);


	return stat_box_entity;
}
*/

Entity createMap()
{
	Entity entity = Entity();
	// Register with components

	registry.map.emplace(entity);

	Motion& map_position = registry.motions.emplace(entity);

	map_position.scale = { 1.25 * 1200, 2 * 800 };
	map_position.position = { map_position.scale.x / 2 - (map_position.scale.x - 1200) / 2, -1 * map_position.scale.y / 2 + 800 };

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MAP,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE,
		"BACKGROUND"
		});

	return entity;
}

Entity createCutsceneBackground()
{
	Entity entity = Entity();
	// Register with components

	registry.cutscene_elements.emplace(entity);

	Motion& map_position = registry.motions.emplace(entity);

	map_position.scale = { 1200, 800 };
	map_position.position = { map_position.scale.x / 2 - (map_position.scale.x - 1200) / 2, -1 * map_position.scale.y / 2 + 800 };

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::CUTSCENE_BACKGROUND,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE,
		"BACKGROUND"
		});

	return entity;
}

Entity createRoomsViewMap(int backgroundNum) {
	Entity entity = Entity();
	// Register with components

	registry.map.emplace(entity);

	Motion& map_position = registry.motions.emplace(entity);
	map_position.scale = { 1.25 * 1200, 2 * 800 };
	float y_pos = (-1 * map_position.scale.y / 2 + 800) - backgroundNum * map_position.scale.y;
	map_position.position = { map_position.scale.x / 2 - (map_position.scale.x - 1200) / 2, y_pos};

	registry.fullMapViewBackground.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ROOMS_VIEW_DUNGEON_BACKGROUND,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE,
		"HIDE ROOM VIEW MAP"
		});

	return entity;
}

Entity& createRoom(vec2 position) {
	Entity entity = Entity();
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ROOM,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE,
		"ROOMS"
		});

	// Register with components
	Motion& room_position = registry.motions.emplace(entity);
	room_position.position = position;
	room_position.scale = { 352, 252 };

	std::vector<Entity> empty_connections;
	RenderedRoom& room = registry.renderedRoom.emplace(entity);
	room.connections = empty_connections;

	registry.currentRender.emplace(entity);

	std::set<Entity> emptyRoomEntities;
	RoomEntities& room_entities = registry.roomEntities.emplace(entity);
	room_entities.roomEntities = emptyRoomEntities;

	return entity;
}

Entity& create_rooms_and_paths(TEXTURE_ASSET_ID rooms_texture) {
	Entity entity = Entity();
	registry.renderRequests.insert(
		entity,
		{ rooms_texture,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE,
		"PATHWAYS"
		});

	// Register with components
	Motion& room_position = registry.motions.emplace(entity);
	room_position.position = { 600, 375 };
	room_position.scale = { 1145, 605};

	registry.currentRender.emplace(entity);

	return entity;
}

Entity create_switch_map_view_button() {
	Entity entity = Entity();

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = { 1125.f, 50.f };

	// Setting initial scale values
	motion.scale = vec2({ 70.f, 50.f });

	registry.fullMapViewButton.emplace(entity);

	//Set render request values
	registry.renderRequests.insert(
		entity, {
			TEXTURE_ASSET_ID::MAP_VIEW_BUTTON,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"SWITCH MAP VIEW BUTTON"
		}
	);

	// cutscene elements should not move
	registry.uiComponents.insert(entity, {});

	return entity;
}


Entity createLine(vec2 position, vec2 scale, float angle) {
	Entity entity = Entity();

	// TODO: store a reference to the potentially re-used mesh object (the value is stored in the resource cache)???

	// Create motion
	Motion& motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.position = position;
	motion.scale = scale;

	// registry.debugComponents.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::PEBBLE,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE,
		"PATHWAYS"
		//"CUTSCENEOBJECT2"
		});

	registry.currentRender.emplace(entity);

	registry.lines.emplace(entity);

	return entity;
}

Entity createFullMapViewRoom(vec2 position) {
	Entity entity = Entity();
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::FULL_MAP_VIEW_ROOM,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE,
		"HIDE ROOM VIEW ROOM"
		});

	registry.fullMapViewRoom.emplace(entity);

	// Register with components
	Motion& room_position = registry.motions.emplace(entity);
	room_position.position = position;
	room_position.scale = { 200, 200 };

	std::vector<Entity> empty_connections;
	Room& room = registry.room.emplace(entity);
	room.connections = empty_connections;

	std::set<Entity> emptyRoomEntities;
	RoomEntities& room_entities = registry.roomEntities.emplace(entity);
	room_entities.roomEntities = emptyRoomEntities;

	return entity;
}

Entity createFullMapViewLine(vec2 position, vec2 scale, float angle) {
	Entity entity = Entity();

	// TODO: store a reference to the potentially re-used mesh object (the value is stored in the resource cache)???

	registry.fullMapViewLine.emplace(entity);
	
	// Create motion
	Motion& motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.position = position;
	motion.scale = scale;

	// registry.debugComponents.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::PEBBLE,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE,
		"HIDE ROOM VIEW LINE"
		});

	//registry.lines.emplace(entity);

	return entity;
}

Entity createSword(Entity parent) {
	auto entity = Entity();

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	Motion& parent_motion = registry.motions.get(parent);

	float weapon_offset_x = parent_motion.scale.x / 3;
	float weapon_offset_y = parent_motion.scale.y / 4;


	motion.position = vec2{ parent_motion.position.x + weapon_offset_x,  parent_motion.position.y + weapon_offset_y };

	motion.scale = vec2{ parent_motion.scale.x / 3 * 2, parent_motion.scale.y };

	motion.angle = M_PI / 8;

	// Swinging& swinging = registry.swinging.emplace(entity);
	Weapon& weapon = registry.weapons.emplace(entity);

	Animation& animations = registry.animations.emplace(entity);

	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 4;
	animations.time_elapsed_ms = 0;
	animations.repeating = true;
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SWORD,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"WEAPONS" });

	return entity;
};

Entity createStaff(Entity parent) {
	auto entity = Entity();

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	Motion& parent_motion = registry.motions.get(parent);

	float weapon_offset_x = parent_motion.scale.x / 3;
	float weapon_offset_y = parent_motion.scale.y / 4;


	motion.position = vec2{ parent_motion.position.x + weapon_offset_x,  parent_motion.position.y + weapon_offset_y };

	motion.scale = vec2{ parent_motion.scale.x / 3 * 2, parent_motion.scale.y };

	motion.angle = M_PI / 8;


	// Swinging& swinging = registry.swinging.emplace(entity);
	Weapon& weapon = registry.weapons.emplace(entity);

	Animation& animations = registry.animations.emplace(entity);

	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.time_elapsed_ms = 0;
	animations.repeating = true;
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::STAFF,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"WEAPONS" });

	return entity;
}

Entity createBow(Entity parent) {
	auto entity = Entity();

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	Motion& parent_motion = registry.motions.get(parent);

	float weapon_offset_x = parent_motion.scale.x / 3;
	float weapon_offset_y = parent_motion.scale.y / 4;


	motion.position = vec2{ parent_motion.position.x + weapon_offset_x,  parent_motion.position.y + weapon_offset_y };

	motion.scale = vec2{ parent_motion.scale.x , parent_motion.scale.y };

	motion.angle = M_PI / 8;


	// Swinging& swinging = registry.swinging.emplace(entity);
	Weapon& weapon = registry.weapons.emplace(entity);
	weapon.parent = parent;

	Animation& animations = registry.animations.emplace(entity);

	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 4;

	animations.time_elapsed_ms = 0;
	animations.repeating = true;
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BOW,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"WEAPONS" });
	return entity;
}

Entity createSpear(Entity parent) {
	auto entity = Entity();

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	Motion& parent_motion = registry.motions.get(parent);

	float weapon_offset_x = parent_motion.scale.x / 3;
	float weapon_offset_y = parent_motion.scale.y / 4;


	motion.position = vec2{ parent_motion.position.x + weapon_offset_x,  parent_motion.position.y };

	motion.scale = vec2{ parent_motion.scale.x / 3 * 2, parent_motion.scale.y * 1.5 };

	motion.angle = M_PI / 6;


	Weapon& weapon = registry.weapons.emplace(entity);

	Animation& animations = registry.animations.emplace(entity);

	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 4;


	animations.time_elapsed_ms = 0;
	animations.repeating = true;
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SPEAR,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"WEAPONS" });

	return entity;
}


Entity createStaff2(Entity parent) {
	auto entity = Entity();

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	Motion& parent_motion = registry.motions.get(parent);

	float weapon_offset_x = parent_motion.scale.x / 3;
	float weapon_offset_y = parent_motion.scale.y / 4;


	motion.position = vec2{ parent_motion.position.x + weapon_offset_x,  parent_motion.position.y + weapon_offset_y };

	motion.scale = vec2{ parent_motion.scale.x / 3 * 2, parent_motion.scale.y };

	motion.angle = M_PI / 8;


	// Swinging& swinging = registry.swinging.emplace(entity);
	Weapon& weapon = registry.weapons.emplace(entity);

	Animation& animations = registry.animations.emplace(entity);

	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.time_elapsed_ms = 0;
	animations.repeating = true;
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::STAFF2,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"WEAPONS" });

	return entity;
}

Entity createClub(Entity parent) {
	auto entity = Entity();

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	Motion& parent_motion = registry.motions.get(parent);

	float weapon_offset_x = parent_motion.scale.x / 3;
	float weapon_offset_y = parent_motion.scale.y / 4;


	motion.position = vec2{ parent_motion.position.x + weapon_offset_x,  parent_motion.position.y + weapon_offset_y };

	motion.scale = vec2{ parent_motion.scale.x / 3 * 2, parent_motion.scale.y };

	motion.angle = M_PI / 8;

	// Swinging& swinging = registry.swinging.emplace(entity);
	Weapon& weapon = registry.weapons.emplace(entity);

	Animation& animations = registry.animations.emplace(entity);

	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 4;
	animations.time_elapsed_ms = 0;
	animations.repeating = true;
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::CLUB,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"WEAPONS" });

	return entity;
}


Entity createArrow(vec2 pos, vec2 target) {
	auto entity = Entity();
	return entity;
}


Entity createHealthBar(RenderSystem* renderer, Entity parent, bool isAdv) {
	auto entity = Entity();
	
	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	Motion& parent_motion = registry.motions.get(parent);

	motion.position = { parent_motion.position.x , parent_motion.position.y - parent_motion.scale.y / 2 };
	motion.scale = { parent_motion.scale.x , parent_motion.scale.x / 4 };

	
	Stats& stats = registry.characters.get(parent).stats;
	HealthBar& hb = registry.healthBars.emplace(entity);
	hb.max_health = stats.hp;


	if (isAdv) {
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::ADVENTURER_HEALTHBAR,
				EFFECT_ASSET_ID::HEALTHBAR,
				GEOMETRY_BUFFER_ID::HEALTHBAR,
				"HEALTHBAR" });
	}
	else {
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MOB_HEALTHBAR,
				EFFECT_ASSET_ID::HEALTHBAR,
				GEOMETRY_BUFFER_ID::HEALTHBAR,
				"HEALTHBAR" });
	}
	
	return entity;
}


Entity createLittleGuy(RenderSystem* renderer) {
	Entity player = Entity();

	// set render and animation of little guy
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE_ANIMATION);
	registry.meshPtrs.emplace(player, &mesh);
	registry.renderRequests.insert(
		player, {
			TEXTURE_ASSET_ID::LILGUY,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"CHARACTER_PORTRAIT",
		}
	);

	Animation& animations = registry.animations.emplace(player);
	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 4;
	animations.time_elapsed_ms = 0;
	animations.repeating = true;

	return player;
}


Entity createCutsceneMob(RenderSystem* renderer, MobClass::MOBCLASS mob_class) {
	Entity mob = Entity();

	// create mesh or sprite of mob here; basic template copied from A1
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE_ANIMATION);
	registry.meshPtrs.emplace(mob, &mesh);

	Animation& animations = registry.animations.emplace(mob);

	animations.sprite_h = 1;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 4;
	animations.num_frames[2] = 3;

	animations.time_elapsed_ms = 0;
	animations.repeating = true;

	TEXTURE_ASSET_ID textureAsset;

	switch (mob_class) {
	case MobClass::BARBARIAN:
		textureAsset = TEXTURE_ASSET_ID::BARBARIAN;
		break;
	case MobClass::BRUTE:
		textureAsset = TEXTURE_ASSET_ID::BRUTE;
		break;
	case MobClass::SHAMAN:
		textureAsset = TEXTURE_ASSET_ID::SHAMAN;
		break;
	default:
		break;
	}

	registry.renderRequests.insert(
		mob,
		{ textureAsset,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"CHARACTERS" });

	return mob;
}


Entity createCutsceneAdventurer(RenderSystem* renderer, Class::CLASS adv_class) {
	Entity adventurer = Entity();

	// create mesh or sprite of adventurer here; basic template copied from A1
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE_ANIMATION);
	registry.meshPtrs.emplace(adventurer, &mesh);

	Animation& animations = registry.animations.emplace(adventurer);

	animations.sprite_h = 0.5;
	animations.sprite_w = 0.25;

	animations.curr_frame = 0;
	animations.num_frames[0] = 4;
	animations.num_frames[1] = 4;
	animations.time_elapsed_ms = 0;
	animations.repeating = true;

	TEXTURE_ASSET_ID textureAsset = TEXTURE_ASSET_ID::WARRIOR;

	switch (adv_class) {
	case Class::WARRIOR:
		textureAsset = TEXTURE_ASSET_ID::WARRIOR;
		break;

	case Class::MAGE:
		textureAsset = TEXTURE_ASSET_ID::MAGE;
		break;

	case Class::RANGER:
		textureAsset = TEXTURE_ASSET_ID::RANGER;
		break;

	default:
		break;
	}

	registry.renderRequests.insert(
		adventurer,
		{ textureAsset,
			EFFECT_ASSET_ID::TEXTURED_ANIMATION,
			GEOMETRY_BUFFER_ID::SPRITE_ANIMATION,
			"CHARACTERS" });

	return adventurer;
}


Entity createImaginaryMan(RenderSystem* renderer) {
	Entity player = Entity();

	// set render and animation of little guy
	registry.renderRequests.insert(
		player, {
			TEXTURE_ASSET_ID::DMG_BG,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"CHARACTER_PORTRAIT",
		}
	);

	return player;
}


// 
Entity create_love_effect(Entity character) {
	Entity entity = Entity();
	CutsceneEffect& effect = registry.cutscene_effects.emplace(entity);
	effect.effect = "LOVE";
	effect.entity = character;
	registry.cutscene_elements.emplace(entity);

	// add render
	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::CUTSCENE_LOVE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"CHARACTER_PORTRAIT"
		});

	// position above character
	vec2 position = registry.motions.get(character).position;
	Motion& motion = registry.motions.emplace(entity);
	motion.scale = { 50.f, 50.f };
	motion.position = { position.x, position.y - 75 };

	return entity;
}


//
Entity create_anger_effect(Entity character) {
	Entity entity = Entity();
	CutsceneEffect& effect = registry.cutscene_effects.emplace(entity);
	effect.effect = "ANGER";
	effect.entity = character;
	registry.cutscene_elements.emplace(entity);

	// add render
	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::CUTSCENE_ANGER,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"CHARACTER_PORTRAIT"
		});

	// position above character
	vec2 position = registry.motions.get(character).position;
	Motion& motion = registry.motions.emplace(entity);
	motion.scale = { 50.f, 50.f };
	motion.position = { position.x - 20, position.y - 35 };

	return entity;

}


//
Entity create_confusion_effect(Entity character) {
	Entity entity = Entity();
	CutsceneEffect& effect = registry.cutscene_effects.emplace(entity);
	effect.effect = "CONFUSION";
	effect.entity = character;
	registry.cutscene_elements.emplace(entity);

	// add render
	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::CUTSCENE_CONFUSION,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"CHARACTER_PORTRAIT"
		});

	// position above character
	vec2 position = registry.motions.get(character).position;
	Motion& motion = registry.motions.emplace(entity);
	motion.scale = { 50.f, 50.f };
	motion.position = { position.x, position.y - 75 };

	return entity;

}


//
Entity create_surprise_effect(Entity character) {
	Entity entity = Entity();
	CutsceneEffect& effect = registry.cutscene_effects.emplace(entity);
	effect.effect = "SURPRISE";
	effect.entity = character;
	registry.cutscene_elements.emplace(entity);

	// add render
	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::CUTSCENE_SURPRISE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			"CHARACTER_PORTRAIT"
		});

	// position above character
	vec2 position = registry.motions.get(character).position;
	Motion& motion = registry.motions.emplace(entity);
	motion.scale = { 50.f, 50.f };
	motion.position = { position.x, position.y - 75 };

	return entity;

}