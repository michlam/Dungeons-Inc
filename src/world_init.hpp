#pragma once

///////////////
//  imports  //
/////////////// 
#include "common.hpp"

// component
#include "tiny_ecs.hpp"


// systems
#include "render_system.hpp"
#include "phase/phase_system.hpp"
#include "animation_system.hpp"



///////////////
// constants //
/////////////// 
const float STATBOX_WIDTH = 125.f;
const float STATBOX_HEIGHT = 125.f;


///////////////
// functions //
/////////////// 
Entity createAdventurer(RenderSystem* renderer, vec2 pos, Class::CLASS advClass, Enemy enemy_data);

Entity createMob(RenderSystem* renderer, vec2 pos, MobClass::MOBCLASS mob_class);

Entity createStatBox(RenderSystem* renderer, vec2 pos);

//Entity createAdvStatText(vec2 pos, Stats stats, Stats stat_changes, Class::CLASS advClass);

Entity createMap();

Entity createCutsceneBackground();

Entity createRoomsViewMap(int backgroundNum);

Entity& createRoom(vec2 position);

Entity create_switch_map_view_button();

// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size, float angle);

Entity createFullMapViewRoom(vec2 position);

Entity createFullMapViewLine(vec2 position, vec2 scale, float angle);

//weapons
Entity createSword(Entity parent);

Entity createStaff(Entity parent);

Entity createBow(Entity parent);

Entity createSpear(Entity parent);

Entity createStaff2(Entity parent);

Entity createClub(Entity parent);

Entity createArrow(vec2 pos, vec2 target);

Entity createHealthBar(RenderSystem* renderer, Entity parent, bool isAdv);

Entity& create_rooms_and_paths(TEXTURE_ASSET_ID rooms_texture);

Entity createLittleGuy(RenderSystem* renderer);

Entity createImaginaryMan(RenderSystem* renderer);

Entity createCutsceneMob(RenderSystem* renderer, MobClass::MOBCLASS mob_class);

Entity createCutsceneAdventurer(RenderSystem* renderer, Class::CLASS adv_class);

Entity create_love_effect(Entity character);

Entity create_anger_effect(Entity character);

Entity create_confusion_effect(Entity character);

Entity create_surprise_effect(Entity character);