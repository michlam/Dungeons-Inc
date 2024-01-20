#pragma once

///////////////
//  imports  //
/////////////// 
// stdlib
#include <vector>
#include "tiny_ecs.hpp"

// components
#include "components.hpp"
#include <phase/phase_components.hpp>
#include <map_components.hpp>
#include <card/card_components.hpp>
#include <level/level_components.hpp>
#include <animation_components.hpp>
#include <cutscene/cutscene_components.hpp>


///////////////
//   class   //
/////////////// 

class ECSRegistry
{
	// Callbacks to remove a particular or all entities in the system
	std::vector<ContainerInterface*> registry_list;

public:
	// Manually created list of all components this game has

	// universal
	ComponentContainer<Player> players;
	ComponentContainer<CursorFocus> cursorFocus;
	ComponentContainer<SecondaryFocus> secondaryFocus;
	ComponentContainer<Hovered> hovered;   // !!!! TODO: replace this
	ComponentContainer<DebugComponent> debugComponents;


	// rendering
	ComponentContainer<Mesh*> meshPtrs;
	ComponentContainer<RenderRequest> renderRequests;
	ComponentContainer<ScreenState> screenStates;
	ComponentContainer<vec3> colors;
	ComponentContainer<Text> texts;
	ComponentContainer<UI> uiComponents;
	ComponentContainer<Animation> animations;
	ComponentContainer<Animation> animations_ignore_pause;
	ComponentContainer<Line> lines;
	ComponentContainer<ResettingText> resetting_texts;


	// physics
	ComponentContainer<Motion> motions;
	ComponentContainer<Movement> movements;
	ComponentContainer<Collision> collisions;
	ComponentContainer<MovementInstructions> movement_instructions;


	// phase components
	ComponentContainer<Phase> phases;


	// level components
	ComponentContainer<Level> levels;


	// cutscene components
	ComponentContainer<CutsceneEvent> cutscene_events;
	ComponentContainer<CutsceneElement> cutscene_elements;
	ComponentContainer<CutsceneChoice> cutscene_choices;
	ComponentContainer<CutsceneEffect> cutscene_effects;


	// dungeon map components
	ComponentContainer<Room> room;
	ComponentContainer<Map> map;
	ComponentContainer<RoomEntities> roomEntities;
	ComponentContainer<HasCardApplied> cards_on_room;
	ComponentContainer<CurrentRoom> currentRoom;
	ComponentContainer< FullMapViewButton> fullMapViewButton;
	ComponentContainer< FullMapViewBackground> fullMapViewBackground;
	ComponentContainer< FullMapViewRoom> fullMapViewRoom;
	ComponentContainer< FullMapViewLine> fullMapViewLine;
	ComponentContainer< CurrentRender> currentRender;
	ComponentContainer< RenderedRoom> renderedRoom;
	ComponentContainer< CorrespondingRoom> correspondingRoom;


	// character components
	ComponentContainer<Stats> stats;
	ComponentContainer<Character> characters;
	ComponentContainer<Target> target;
	ComponentContainer<Class> adventurerClass;
	ComponentContainer<MobClass> mobClass;
	ComponentContainer<InCombat> inCombat;
	ComponentContainer<StatusBox> statusBoxes;
	//ComponentContainer<StatusText> statusTexts;
	ComponentContainer<StatChange> stat_changes;
	ComponentContainer<Mob> mob;
	ComponentContainer<Enemy> enemy;
	ComponentContainer<HealthBar> healthBars;


	// card system components
	ComponentContainer<Card> cards;
	ComponentContainer<CardEffect> card_effects;
	ComponentContainer<CardDeck> deck;
	ComponentContainer<PlayerHand> hand;
	ComponentContainer<InPlay> cards_in_play;


	// Level select menu components
	ComponentContainer<LSMButton> lsmButtons;
	ComponentContainer<LSMPopup> lsmPopups;
	ComponentContainer<LSMStartButton> lsmStartButtons;
	ComponentContainer<LSMBackground> lsmBackgrounds;
	ComponentContainer<LSMSelector> lsmSelectors;
	ComponentContainer<LSMLittleGuy> lsmLittleGuy;


	// animation components
	ComponentContainer<Weapon> weapons;
	ComponentContainer<Fading> fading;
	ComponentContainer<FadingWeapon> fadingWeapons;
	ComponentContainer<DamageNumber> damageNumbers;
	ComponentContainer<Casting> casting;
	ComponentContainer<Swinging> swinging;
	ComponentContainer<Shooting> shooting;
	ComponentContainer<Stabbing> stabbing;
	ComponentContainer<Blood> blood;


	// menu components
	ComponentContainer<OpenMenuButton> openMenuButton;
	ComponentContainer<MenuUi> menuUis;
	ComponentContainer<MenuButtons> menuButtons;
	ComponentContainer<SettingsUI> settingsUI;
	ComponentContainer<SettingsButtons> settingsButtons;
	ComponentContainer<SettingsIcons> settingsIcons;





	// constructor that adds all containers for looping over them
	// IMPORTANT: Don't forget to add any newly added containers!
	ECSRegistry() {
		// universal
		registry_list.push_back(&players);
		registry_list.push_back(&cursorFocus);
		registry_list.push_back(&secondaryFocus);
		registry_list.push_back(&hovered);   // !!!! TODO: replace this
		registry_list.push_back(&debugComponents);

		// rendering
		registry_list.push_back(&meshPtrs);
		registry_list.push_back(&renderRequests);
		registry_list.push_back(&screenStates);
		registry_list.push_back(&colors);

		registry_list.push_back(&texts);
		registry_list.push_back(&uiComponents);
		registry_list.push_back(&animations);
		registry_list.push_back(&animations_ignore_pause);
		registry_list.push_back(&lines);
		registry_list.push_back(&resetting_texts);

		// physics
		registry_list.push_back(&motions);
		registry_list.push_back(&movements);
		registry_list.push_back(&collisions);
		registry_list.push_back(&movement_instructions);

		// phase components
		registry_list.push_back(&phases);

		// level components
		registry_list.push_back(&levels);

		// cutscene components
		registry_list.push_back(&cutscene_events);
		registry_list.push_back(&cutscene_elements);
		registry_list.push_back(&cutscene_choices);
		registry_list.push_back(&cutscene_effects);

		// dungeon map components
		registry_list.push_back(&map);
		registry_list.push_back(&room);
		registry_list.push_back(&roomEntities);
		registry_list.push_back(&cards_on_room);
		registry_list.push_back(&currentRoom);
		registry_list.push_back(&fullMapViewButton);
		registry_list.push_back(&fullMapViewBackground);
		registry_list.push_back(&fullMapViewRoom);
		registry_list.push_back(&fullMapViewLine);
		registry_list.push_back(&currentRender);
		registry_list.push_back(&renderedRoom);
		registry_list.push_back(&correspondingRoom);

		// character components
		registry_list.push_back(&stats);
		registry_list.push_back(&characters);
		registry_list.push_back(&target);
		registry_list.push_back(&adventurerClass);
		registry_list.push_back(&mobClass);
		registry_list.push_back(&inCombat);
		registry_list.push_back(&statusBoxes);
		//registry_list.push_back(&statusTexts);

		registry_list.push_back(&stat_changes);
		registry_list.push_back(&mob);
		registry_list.push_back(&enemy);
		registry_list.push_back(&healthBars);


		// card system components
		registry_list.push_back(&cards);
		registry_list.push_back(&card_effects);
		registry_list.push_back(&deck);
		registry_list.push_back(&hand);
		registry_list.push_back(&cards_in_play);

		// level select menu components
		registry_list.push_back(&lsmButtons);
		registry_list.push_back(&lsmPopups);
		registry_list.push_back(&lsmStartButtons);
		registry_list.push_back(&lsmBackgrounds);
		registry_list.push_back(&lsmSelectors);
		registry_list.push_back(&lsmLittleGuy);

		// animation components
		registry_list.push_back(&weapons);
		registry_list.push_back(&fading);
		registry_list.push_back(&fadingWeapons);
		registry_list.push_back(&damageNumbers);


		registry_list.push_back(&swinging);
		registry_list.push_back(&shooting);
		registry_list.push_back(&blood);
		registry_list.push_back(&casting);
		registry_list.push_back(&stabbing);

		// menu components
		registry_list.push_back(&openMenuButton);
		registry_list.push_back(&menuUis);
		registry_list.push_back(&menuButtons);
		registry_list.push_back(&settingsUI);
		registry_list.push_back(&settingsButtons);
		registry_list.push_back(&settingsIcons);


	}


	void clear_all_components() {
		for (ContainerInterface* reg : registry_list)
			reg->clear();
	}


	void list_all_components() {
		printf("Debug info on all registry entries:\n");
		for (ContainerInterface* reg : registry_list)
			if (reg->size() > 0)
				printf("%4d components of type %s\n", (int)reg->size(), typeid(*reg).name());
	}


	void list_all_components_of(Entity e) {
		printf("Debug info on components of entity %u:\n", (unsigned int)e);
		for (ContainerInterface* reg : registry_list)
			if (reg->has(e))
				printf("type %s\n", typeid(*reg).name());
	}


	void remove_all_components_of(Entity e) {
		for (ContainerInterface* reg : registry_list)
			reg->remove(e);
	}
};

extern ECSRegistry registry;