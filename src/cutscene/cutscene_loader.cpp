#include <cutscene/cutscene_loader.hpp>
#include <world_init.hpp>


CutsceneLoader cutscene_loader;


void CutsceneLoader::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;
}


// mr ta i know this is bad programming practice but the cutscenes were originally supposed to be loaded via json files
// but for whatever reason i kept getting linker issues when attempting to import the utils file so i just gave up...
// please do not stare at this too hard......


void CutsceneLoader::load_cutscene(int cutscene_to_play) {
	// load_cutscene_data(cutscene_to_play);

	if (cutscene_to_play == 0) {
		load_cutscene_0();
	}
	else if (cutscene_to_play == 2) {
		load_cutscene_2();
	}
	else if (cutscene_to_play == 4) {
		load_cutscene_4();
	}
	else if (cutscene_to_play == 6) {
		load_cutscene_6();
	}
	else if (cutscene_to_play == 7) {
		load_cutscene_7();
	}
	else if (cutscene_to_play == 8) {
		load_dating_sim();
	}
	else {
		// to avoid getting white background flashbanged
		Entity background = createMap();
		CutsceneElement& background_element = registry.cutscene_elements.has(background) ? registry.cutscene_elements.get(background) : registry.cutscene_elements.emplace(background);
		background_element.name = "BACKGROUND";
	}
}

void CutsceneLoader::load_cutscene_0() {
	/////////////////////////////////////////////////////////////////////////////////
	// TEMP LOAD CUTSCENE
	Entity background = createCutsceneBackground();
	CutsceneElement& background_element = registry.cutscene_elements.has(background) ? registry.cutscene_elements.get(background) : registry.cutscene_elements.emplace(background);
	background_element.name = "BACKGROUND";

	Entity little_guy = createLittleGuy(this->renderer);
	Motion& player_motion = registry.motions.emplace(little_guy);
	player_motion.position = { -50, 400 };
	player_motion.scale = { 80, 100 };
	CutsceneElement& player_element = registry.cutscene_elements.emplace(little_guy);
	player_element.name = "LITTLE GUY";



	// TEMP ADD CUTSCENE EVENT 1
	Action action1;

	Motion motion1 = player_motion;
	action1.motion = motion1;

	Movement movement1;
	movement1.goal_pos = { 400, 400 };
	action1.movement = movement1;

	action1.emotion = "LOVE";
	
	CutsceneEvent& event1 = registry.cutscene_events.emplace(little_guy);
	event1.actions.push_back(action1);
	event1.dialogue.push_back("Wow... Is this my very own dungeon?");
	


	// TEMP ADD CUTSCENE EVENT 2
	Action action2;

	Motion motion2;
	motion2.scale = { 80, 100 };
	motion2.position = { 400, 400 };
	action2.motion = motion2;

	Movement movement2;
	movement2.goal_pos = { 300, 400 };
	action2.movement = movement2;

	CutsceneEvent& event2 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event2.actions.push_back(action2);
	event2.dialogue.push_back("It's so... uh... musty! And very extremely dank.");

	// TEMP ADD CUTSCENE EVENT 3
	Action action3;

	Motion motion3;
	motion3.scale = { 80, 100 };
	motion3.position = { 300, 400 };
	action3.motion = motion3;

	Movement movement3;
	movement3.goal_pos = { 500, 400 };
	action3.movement = movement3;

	CutsceneEvent& event3 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event3.actions.push_back(action3);
	event3.dialogue.push_back("I guess things really did deteriorate after Grandpa disappeared...");

	// TEMP ADD CUTSCENE EVENT 4
	Action action4;

	Motion motion4;
	motion4.scale = { 80, 100 };
	motion4.position = { 500, 400 };
	action4.motion = motion4;

	Movement movement4;
	movement4.goal_pos = { 450, 400 };
	action4.movement = movement4;

	action4.emotion = "SURPRISE";

	CutsceneEvent& event4 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event4.actions.push_back(action4);
	event4.dialogue.push_back("Woah! What was that? I guess I better go check it out!");
	/////////////////////////////////////////////////////////////////////////////////
}


void CutsceneLoader::load_cutscene_2() {
	/////////////////////////////////////////////////////////////////////////////////
	// TEMP LOAD CUTSCENE
	Entity background = createCutsceneBackground();
	CutsceneElement& background_element = registry.cutscene_elements.has(background) ? registry.cutscene_elements.get(background) : registry.cutscene_elements.emplace(background);
	background_element.name = "BACKGROUND";

	Entity little_guy = createLittleGuy(this->renderer);
	Motion& player_motion = registry.motions.emplace(little_guy);
	player_motion.position = { 400, 400 };
	player_motion.scale = { 80, 100 };
	CutsceneElement& player_element = registry.cutscene_elements.emplace(little_guy);
	player_element.name = "LITTLE GUY";



	// TEMP ADD CUTSCENE EVENT 1
	Action action_1;

	Motion motion_1;
	motion_1.scale = { 80, 100 };
	motion_1.position = { 400, 400 };
	action_1.motion = motion_1;

	Movement movement_1;
	action_1.movement = movement_1;

	CutsceneEvent& event_1 = registry.cutscene_events.emplace(little_guy);
	event_1.actions.push_back(action_1);
	event_1.dialogue.push_back("That was a tough battle... It feels nice to win against these pesky adventurers, but...");



	// TEMP ADD CUTSCENE EVENT 2
	Action action_2;

	Motion motion_2;
	motion_2.scale = { 80, 100 };
	motion_2.position = { 400, 400 };
	action_2.motion = motion_2;

	Movement movement_2;
	movement_2.goal_pos = { 300, 400 };
	action_2.movement = movement_2;

	CutsceneEvent& event_2 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_2.actions.push_back(action_2);
	event_2.dialogue.push_back("Jerome... Brad... Even little Elijah... I feel so awful... What am I going to tell their families? Their friends? And everyone else?");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_3;

	Motion motion_3;
	motion_3.scale = { 80, 100 };
	motion_3.position = { 300, 400 };
	action_3.motion = motion_3;

	Movement movement_3;
	movement_3.goal_pos = { 350, 400 };
	action_3.movement = movement_3;

	action_3.emotion = "CONFUSION";

	CutsceneEvent& event_3 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_3.actions.push_back(action_3);
	event_3.dialogue.push_back("Huh? What was that noise? Who's there? More adventurers?");



	// TEMP ADD CUTSCENE EVENT 4
	Entity barbara_mob = createCutsceneMob(this->renderer, MobClass::BARBARIAN);
	Motion& barbara_motion = registry.motions.emplace(barbara_mob);
	barbara_motion.position = { default_window_size.x + 50, 400 };
	barbara_motion.scale = { 120, 150 };
	CutsceneElement& barbara_element = registry.cutscene_elements.emplace(barbara_mob);
	barbara_element.name = "BARBARA";

	Action action_4;

	Motion motion_4;
	motion_4.scale = { 120, 150 };
	motion_4.position = { default_window_size.x + 50, 400 };
	action_4.motion = motion_4;

	Movement movement_4;
	movement_4.goal_pos = { 800, 400 };
	action_4.movement = movement_4;

	CutsceneEvent& event_4 = registry.cutscene_events.emplace_with_duplicates(barbara_mob);
	event_4.actions.push_back(action_4);
	event_4.dialogue.push_back("Little Guy? Are you there?");



	// TEMP ADD CUTSCENE EVENT 5
	Action action_5;

	Motion motion_5;
	motion_5.scale = { 80, 100 };
	motion_5.position = { 350, 400 };
	action_5.motion = motion_5;

	Movement movement_5;
	movement_5.goal_pos = { 400, 400 };
	action_5.movement = movement_5;

	action_5.emotion = "SURPRISE";

	CutsceneEvent& event_5 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_5.actions.push_back(action_5);
	event_5.dialogue.push_back("Barbara! What are you doing here? It's not safe here - you should leave quickly!!");



	// TEMP ADD CUTSCENE EVENT 6
	Action action_6;

	Motion motion_6;
	motion_6.scale = { 120, 150 };
	motion_6.position = { 800, 400 };
	action_6.motion = motion_6;

	Movement movement_6;
	movement_6.goal_pos = { 750, 400 };
	action_6.movement = movement_6;

	action_6.emotion = "ANGER";

	CutsceneEvent& event_6 = registry.cutscene_events.emplace_with_duplicates(barbara_mob);
	event_6.actions.push_back(action_6);
	event_6.dialogue.push_back("And leave you alone by yourself? No way! I came here to help you and that's exactly what I'm going to do.");



	// TEMP ADD CUTSCENE EVENT 7
	Action action_7;

	Motion motion_7;
	motion_7.scale = { 80, 100 };
	motion_7.position = { 400, 400 };
	action_7.motion = motion_7;

	Movement movement_7;
	action_7.movement = movement_7;

	CutsceneEvent& event_7 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_7.actions.push_back(action_7);
	event_7.dialogue.push_back("Barbara...");



	// TEMP ADD CUTSCENE EVENT 8
	Action action_8;

	Motion motion_8;
	motion_8.scale = { -120, 150 };
	motion_8.position = { 750, 400 };
	action_8.motion = motion_8;

	Movement movement_8;
	action_8.movement = movement_8;

	CutsceneEvent& event_8 = registry.cutscene_events.emplace_with_duplicates(barbara_mob);
	event_8.actions.push_back(action_8);
	event_8.dialogue.push_back("We'll fight together from now on!");

	/////////////////////////////////////////////////////////////////////////////////
}


void CutsceneLoader::load_cutscene_4() {
	/////////////////////////////////////////////////////////////////////////////////
	// TEMP LOAD CUTSCENE
	Entity background = createCutsceneBackground();
	CutsceneElement& background_element = registry.cutscene_elements.has(background) ? registry.cutscene_elements.get(background) : registry.cutscene_elements.emplace(background);
	background_element.name = "BACKGROUND";

	Entity little_guy = createLittleGuy(this->renderer);
	Motion& player_motion = registry.motions.emplace(little_guy);
	player_motion.position = { -100, 400 };
	player_motion.scale = { 80, 100 };
	CutsceneElement& player_element = registry.cutscene_elements.emplace(little_guy);
	player_element.name = "LITTLE GUY";



	// TEMP ADD CUTSCENE EVENT 1
	Action action_1;

	Motion motion_1;
	motion_1.scale = { 80, 100 };
	motion_1.position = { -100, 400 };
	action_1.motion = motion_1;

	Movement movement_1;
	movement_1.goal_pos = { 400, 400 };
	action_1.movement = movement_1;

	CutsceneEvent& event_1 = registry.cutscene_events.emplace(little_guy);
	event_1.actions.push_back(action_1);
	event_1.dialogue.push_back("Wow, I can't believe I managed to figure out how to draw a new card.");



	// TEMP ADD CUTSCENE EVENT 2
	Action action_2;

	Motion motion_2;
	motion_2.scale = { -80, 100 };
	motion_2.position = { 400, 400 };
	action_2.motion = motion_2;

	Movement movement_2;
	action_2.movement = movement_2;

	CutsceneEvent& event_2 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_2.actions.push_back(action_2);
	event_2.dialogue.push_back("But each passing day brings more and more adventurers to my dungeon... How much longer can we last for?");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_3;

	Motion motion_3;
	motion_3.scale = { 80, 100 };
	motion_3.position = { 400, 400 };
	action_3.motion = motion_3;

	Movement movement_3;
	movement_3.goal_pos = { 350, 400 };
	action_3.movement = movement_3;

	CutsceneEvent& event_3 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_3.actions.push_back(action_3);
	event_3.dialogue.push_back("So many of my friends are putting their lives on the line just to help protect me and my dungeon. I feel so awful. Maybe if Grandpa was here...");



	// TEMP ADD CUTSCENE EVENT 3.1
	Action action_3_1;

	Motion motion_3_1;
	motion_3_1.scale = { -80, 100 };
	motion_3_1.position = { 350, 400 };
	action_3_1.motion = motion_3_1;

	Movement movement_3_1;
	action_3_1.movement = movement_3_1;

	CutsceneEvent& event_3_1 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_3_1.actions.push_back(action_3_1);
	event_3_1.dialogue.push_back("No, I can't think like this. They're doing ME a favour. I have to return it. I need to train every day to master my power over these cards. I can't keep relying on them -");



	// TEMP ADD CUTSCENE EVENT 4
	Entity barbara_mob = createCutsceneMob(this->renderer, MobClass::BARBARIAN);
	Motion& barbara_motion = registry.motions.emplace(barbara_mob);
	barbara_motion.position = { default_window_size.x + 50, 400 };
	barbara_motion.scale = { 120, 150 };
	CutsceneElement& barbara_element = registry.cutscene_elements.emplace(barbara_mob);
	barbara_element.name = "BARBARA";

	Action action_4;

	Motion motion_4;
	motion_4.scale = { 120, 150 };
	motion_4.position = { default_window_size.x + 50, 400 };
	action_4.motion = motion_4;

	Movement movement_4;
	action_4.movement = movement_4;

	CutsceneEvent& event_4 = registry.cutscene_events.emplace_with_duplicates(barbara_mob);
	event_4.actions.push_back(action_4);
	event_4.dialogue.push_back("Little Guy!");



	// TEMP ADD CUTSCENE EVENT 5
	Action action_5;

	Motion motion_5;
	motion_5.scale = { 80, 100 };
	motion_5.position = { 350, 400 };
	action_5.motion = motion_5;

	Movement movement_5;
	movement_5.goal_pos = { 400, 400 };
	action_5.movement = movement_5;

	action_5.emotion = "SURPRISE";

	CutsceneEvent& event_5 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_5.actions.push_back(action_5);
	event_5.dialogue.push_back("Huh? Barbara?");



	// TEMP ADD CUTSCENE EVENT 6
	Action action_6;

	Motion motion_6;
	motion_6.scale = { 120, 150 };
	motion_6.position = { default_window_size.x + 50, 400 };
	action_6.motion = motion_6;

	Movement movement_6;
	movement_6.goal_pos = { 750, 400 };
	action_6.movement = movement_6;

	CutsceneEvent& event_6 = registry.cutscene_events.emplace_with_duplicates(barbara_mob);
	event_6.actions.push_back(action_6);
	event_6.dialogue.push_back("Don't say stuff like that, Little Guy! You're not alone. We're all ready to support you no matter what!");



	// TEMP ADD CUTSCENE EVENT 7
	Action action_7;

	Motion motion_7;
	motion_7.scale = { 80, 100 };
	motion_7.position = { 400, 400 };
	action_7.motion = motion_7;

	Movement movement_7;
	action_7.movement = movement_7;

	action_7.emotion = "CONFUSION";

	CutsceneEvent& event_7 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_7.actions.push_back(action_7);
	event_7.dialogue.push_back("What do you mean, Barbara? Who's \"we\"?");



	// TEMP ADD CUTSCENE EVENT 8
	Action action_8;

	Motion motion_8;
	motion_8.scale = { 120, 150 };
	motion_8.position = { 750, 400 };
	action_8.motion = motion_8;

	Movement movement_8;
	action_8.movement = movement_8;

	CutsceneEvent& event_8 = registry.cutscene_events.emplace_with_duplicates(barbara_mob);
	event_8.actions.push_back(action_8);
	event_8.dialogue.push_back("Well, while you were busy defending the dungeon from adventurers...");



	// TEMP ADD CUTSCENE EVENT 8.1
	Entity regular_mob = createCutsceneMob(this->renderer, MobClass::BARBARIAN);
	Motion& regular_motion = registry.motions.emplace(regular_mob);
	regular_motion.position = { default_window_size.x + 50, 500 };
	regular_motion.scale = { 80, 100 };
	CutsceneElement& regular_element = registry.cutscene_elements.emplace(regular_mob);
	regular_element.name = "BARBARIAN";

	Action action_8_1;

	Motion motion_8_1;
	motion_8_1.scale = { 80, 100 };
	motion_8_1.position = { default_window_size.x + 50, 500 };
	action_8_1.motion = motion_8_1;

	Movement movement_8_1;
	movement_8_1.goal_pos = { 700, 500 };
	action_8_1.movement = movement_8_1;

	CutsceneEvent& event_8_1 = registry.cutscene_events.emplace_with_duplicates(regular_mob);
	event_8_1.actions.push_back(action_8_1);
	event_8_1.dialogue.push_back("Barbara recruited me!");



	// TEMP ADD CUTSCENE EVENT 8.2
	Entity shaman_mob = createCutsceneMob(this->renderer, MobClass::SHAMAN);
	Motion& shaman_motion = registry.motions.emplace(shaman_mob);
	shaman_motion.position = { default_window_size.x + 50, 450 };
	shaman_motion.scale = { 80, 100 };
	CutsceneElement& shaman_element = registry.cutscene_elements.emplace(shaman_mob);
	shaman_element.name = "SHAMAN";

	Action action_8_2;

	Motion motion_8_2;
	motion_8_2.scale = { 80, 100 };
	motion_8_2.position = { default_window_size.x + 50, 450 };
	action_8_2.motion = motion_8_2;

	Movement movement_8_2;
	movement_8_2.goal_pos = { 790, 450 };
	action_8_2.movement = movement_8_2;

	CutsceneEvent& event_8_2 = registry.cutscene_events.emplace_with_duplicates(shaman_mob);
	event_8_2.actions.push_back(action_8_2);
	event_8_2.dialogue.push_back("And me!");



	// TEMP ADD CUTSCENE EVENT 8.3
	Entity brute_mob = createCutsceneMob(this->renderer, MobClass::BRUTE);
	Motion& brute_motion = registry.motions.emplace(brute_mob);
	brute_motion.position = { default_window_size.x + 50, 475 };
	brute_motion.scale = { 80, 100 };
	CutsceneElement& brute_element = registry.cutscene_elements.emplace(brute_mob);
	brute_element.name = "BRUTE";

	Action action_8_3;

	Motion motion_8_3;
	motion_8_3.scale = { 80, 100 };
	motion_8_3.position = { default_window_size.x + 50, 475 };
	action_8_3.motion = motion_8_3;

	Movement movement_8_3;
	movement_8_3.goal_pos = { 840, 475 };
	action_8_3.movement = movement_8_3;

	CutsceneEvent& event_8_3 = registry.cutscene_events.emplace_with_duplicates(brute_mob);
	event_8_3.actions.push_back(action_8_3);
	event_8_3.dialogue.push_back("And me too!");



	// TEMP ADD CUTSCENE EVENT 8.3
	Entity many_goblin_mobs = createCutsceneMob(this->renderer, MobClass::BARBARIAN);
	Motion& many_goblins_motion = registry.motions.emplace(many_goblin_mobs);
	many_goblins_motion.position = { default_window_size.x + 50, 400 };
	many_goblins_motion.scale = { 80, 100 };
	CutsceneElement& many_goblins_element = registry.cutscene_elements.emplace(many_goblin_mobs);
	many_goblins_element.name = "GOBLINS";

	Action action_8_4;

	Motion motion_8_4;
	motion_8_4.scale = { 80, 100 };
	motion_8_4.position = { default_window_size.x + 50, 400 };
	action_8_4.motion = motion_8_4;

	Movement movement_8_4;
	movement_8_4.goal_pos = { 900, 400 };
	action_8_4.movement = movement_8_4;

	CutsceneEvent& event_8_4 = registry.cutscene_events.emplace_with_duplicates(many_goblin_mobs);
	event_8_4.actions.push_back(action_8_4);
	event_8_4.dialogue.push_back("And all of us!");



	// TEMP ADD CUTSCENE EVENT 9
	Action action_9;

	Motion motion_9;
	motion_9.scale = { 80, 100 };
	motion_9.position = { 400, 400 };
	action_9.motion = motion_9;

	Movement movement_9;
	movement_9.goal_pos = { 450, 400 };
	action_9.movement = movement_9;

	action_9.emotion = "LOVE";

	CutsceneEvent& event_9 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_9.actions.push_back(action_9);
	event_9.dialogue.push_back("Barbara... You guys... *sniff*");



	// TEMP ADD CUTSCENE EVENT 9
	Action action_9_1;

	Motion motion_9_1;
	motion_9_1.scale = { 80, 100 };
	motion_9_1.position = { 450, 400 };
	action_9_1.motion = motion_9_1;

	Movement movement_9_1;
	action_9_1.movement = movement_9_1;

	action_9_1.emotion = "LOVE";

	CutsceneEvent& event_9_1 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_9_1.actions.push_back(action_9_1);
	event_9_1.dialogue.push_back("How did you manage all of this?");



	// TEMP ADD CUTSCENE EVENT 10
	Action action_10;

	Motion motion_10;
	motion_10.scale = { -120, 150 };
	motion_10.position = { 750, 400 };
	action_10.motion = motion_10;

	Movement movement_10;
	action_10.movement = movement_10;

	CutsceneEvent& event_10 = registry.cutscene_events.emplace_with_duplicates(barbara_mob);
	event_10.actions.push_back(action_10);
	event_10.dialogue.push_back("I visited all of the tribes nearby and told them about your story! Everyone I talked to said that they wanted to come and support you.");



	// TEMP ADD CUTSCENE EVENT 11
	Action action_11;

	Motion motion_11;
	motion_11.scale = { 80, 100 };
	motion_11.position = { 790, 450 };
	action_11.motion = motion_11;

	Movement movement_11;
	movement_11.goal_pos = { 750, 450 };
	action_11.movement = movement_11;

	CutsceneEvent& event_11 = registry.cutscene_events.emplace_with_duplicates(shaman_mob);
	event_11.actions.push_back(action_11);
	event_11.dialogue.push_back("And when we're free, some of us will travel to the rest of the goblins to spread word about your heroic advancement against the adventurer kingdom!");



	// TEMP ADD CUTSCENE EVENT 12
	Action action_12;

	Motion motion_12;
	motion_12.scale = { 80, 100 };
	motion_12.position = { 450, 400 };
	action_12.motion = motion_12;

	Movement movement_12;
	action_12.movement = movement_12;

	action_12.emotion = "LOVE";

	CutsceneEvent& event_12 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_12.actions.push_back(action_12);
	event_12.dialogue.push_back("Everyone... I appreciate you all so much... I promise to you all: no matter what, we will bring the end of the adventurer kingdom!");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_12_1;

	Motion motion_12_1;
	motion_12_1.scale = { 80, 100 };
	motion_12_1.position = { 450, 400 };
	action_12_1.motion = motion_12_1;

	Movement movement_12_1;
	movement_12_1.goal_pos = { 400, 400 };
	action_12_1.movement = movement_12_1;

	CutsceneEvent& event_12_1 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_12_1.actions.push_back(action_12_1);
	event_12_1.dialogue.push_back("Now let's go kick some adventurer ass - together!");

	/////////////////////////////////////////////////////////////////////////////////
}



void CutsceneLoader::load_cutscene_6() {
	/////////////////////////////////////////////////////////////////////////////////
	// TEMP LOAD CUTSCENE
	Entity background = createCutsceneBackground();
	CutsceneElement& background_element = registry.cutscene_elements.has(background) ? registry.cutscene_elements.get(background) : registry.cutscene_elements.emplace(background);
	background_element.name = "BACKGROUND";

	Entity little_guy = createLittleGuy(this->renderer);
	Motion& player_motion = registry.motions.emplace(little_guy);
	player_motion.position = { 400, 400 };
	player_motion.scale = { 80, 100 };
	CutsceneElement& player_element = registry.cutscene_elements.emplace(little_guy);
	player_element.name = "LITTLE GUY";



	// TEMP ADD CUTSCENE EVENT 1
	Action action_1;

	Motion motion_1;
	motion_1.scale = { 80, 100 };
	motion_1.position = { 400, 400 };
	action_1.motion = motion_1;

	Movement movement_1;
	action_1.movement = movement_1;

	CutsceneEvent& event_1 = registry.cutscene_events.emplace(little_guy);
	event_1.actions.push_back(action_1);
	event_1.dialogue.push_back("I think I've been able to master my card draws now! This Wither card has been so incredibly helpful these past few days.");



	// TEMP ADD CUTSCENE EVENT 2
	Action action_2;

	Motion motion_2;
	motion_2.scale = { -80, 100 };
	motion_2.position = { 400, 400 };
	action_2.motion = motion_2;

	Movement movement_2;
	action_2.movement = movement_2;

	CutsceneEvent& event_2 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_2.actions.push_back(action_2);
	event_2.dialogue.push_back("Especially since it feels like the adventurers seem to know my dungeon layout beforehand...");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_3;

	Motion motion_3;
	motion_3.scale = { 80, 100 };
	motion_3.position = { 400, 400 };
	action_3.motion = motion_3;

	Movement movement_3;
	movement_3.goal_pos = { 450, 400 };
	action_3.movement = movement_3;

	action_3.emotion = "CONFUSION";

	CutsceneEvent& event_3 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_3.actions.push_back(action_3);
	event_3.dialogue.push_back("It makes no sense. I've made sure to change up my dungeon layout every day to make it harder for them to find their way through.");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_4;

	Motion motion_4;
	motion_4.scale = { 80, 100 };
	motion_4.position = { 450, 400 };
	action_4.motion = motion_4;

	Movement movement_4;
	movement_4.goal_pos = { 375, 400 };
	action_4.movement = movement_4;

	action_4.emotion = "CONFUSION";

	CutsceneEvent& event_4 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_4.actions.push_back(action_4);
	event_4.dialogue.push_back("There's no way one of my friends are leaking my dungeon plans... right?");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_5;

	Motion motion_5;
	motion_5.scale = { 80, 100 };
	motion_5.position = { 375, 400 };
	action_5.motion = motion_5;

	Movement movement_5;
	action_5.movement = movement_5;

	action_5.emotion = "SURPRISE";

	CutsceneEvent& event_5 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_5.actions.push_back(action_5);
	event_5.dialogue.push_back("Woah! What was that noise?");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_6;

	Motion motion_6;
	motion_6.scale = { 80, 100 };
	motion_6.position = { 375, 400 };
	action_6.motion = motion_6;

	Movement movement_6;
	movement_6.goal_pos = { 600, 400 };
	action_6.movement = movement_6;

	CutsceneEvent& event_6 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_6.actions.push_back(action_6);
	event_6.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_7;

	Motion motion_7;
	motion_7.scale = { 80, 100 };
	motion_7.position = { 600, 400 };
	action_7.motion = motion_7;

	Movement movement_7;
	action_7.movement = movement_7;

	CutsceneEvent& event_7 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_7.actions.push_back(action_7);
	event_7.dialogue.push_back("Nothing's there. I guess it was just the wind or something.");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_8;

	Motion motion_8;
	motion_8.scale = { -80, 100 };
	motion_8.position = { 600, 400 };
	action_8.motion = motion_8;

	Movement movement_8;
	action_8.movement = movement_8;

	CutsceneEvent& event_8 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_8.actions.push_back(action_8);
	event_8.dialogue.push_back("I should probably head back. I don't want to leave my friends alone for too long.");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_9;

	Motion motion_9;
	motion_9.scale = { 80, 100 };
	motion_9.position = { 600, 400 };
	action_9.motion = motion_9;

	Movement movement_9;
	movement_9.goal_pos = { -100, 400 };
	action_9.movement = movement_9;

	CutsceneEvent& event_9 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_9.actions.push_back(action_9);
	event_9.dialogue.push_back("Oh Grandpa, wherever you are - please keep watching over all of us...");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_10;

	Motion motion_10;
	motion_10.scale = { 80, 100 };
	motion_10.position = { -100, 400 };
	action_10.motion = motion_10;

	Movement movement_10;
	action_10.movement = movement_10;

	CutsceneEvent& event_10 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_10.actions.push_back(action_10);
	event_10.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 3
	CutsceneEvent& event_10_1A = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_10_1A.actions.push_back(action_10);
	event_10_1A.dialogue.push_back("...");
	CutsceneEvent& event_10_1B = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_10_1B.actions.push_back(action_10);
	event_10_1B.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 3
	Entity imaginary_man = createCutsceneAdventurer(this->renderer, Class::WARRIOR);
	Motion& imaginary_motion = registry.motions.emplace(imaginary_man);
	imaginary_motion.position = { -400, -400 };
	imaginary_motion.scale = { 80, 100 };
	CutsceneElement& imaginary_element = registry.cutscene_elements.emplace(imaginary_man);
	imaginary_element.name = "???";

	Action action_10_2;

	Motion motion_10_2;
	motion_10_2.scale = { 80, 100 };
	motion_10_2.position = { -400, -400 };
	action_10_2.motion = motion_10_2;

	Movement movement_10_2;
	action_10_2.movement = movement_10_2;

	CutsceneEvent& event_10_2 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_10_2.actions.push_back(action_10_2);
	event_10_2.dialogue.push_back("...");
}


void CutsceneLoader::load_cutscene_7() {
	/////////////////////////////////////////////////////////////////////////////////
	// TEMP LOAD CUTSCENE
	Entity background = createCutsceneBackground();
	CutsceneElement& background_element = registry.cutscene_elements.has(background) ? registry.cutscene_elements.get(background) : registry.cutscene_elements.emplace(background);
	background_element.name = "BACKGROUND";

	Entity little_guy = createLittleGuy(this->renderer);
	Motion& player_motion = registry.motions.emplace(little_guy);
	player_motion.position = { 400, 400 };
	player_motion.scale = { 80, 100 };
	CutsceneElement& player_element = registry.cutscene_elements.emplace(little_guy);
	player_element.name = "LITTLE GUY";

	Entity imaginary_man = createCutsceneAdventurer(this->renderer, Class::WARRIOR);
	Motion& imaginary_motion = registry.motions.emplace(imaginary_man);
	imaginary_motion.position = { 750, 400 };
	imaginary_motion.scale = { 80, 100 };
	CutsceneElement& imaginary_element = registry.cutscene_elements.emplace(imaginary_man);
	imaginary_element.name = "???";



	// TEMP ADD CUTSCENE EVENT 1
	Action action_1;

	Motion motion_1;
	motion_1.scale = { 80, 100 };
	motion_1.position = { 400, 400 };
	action_1.motion = motion_1;

	Movement movement_1;
	action_1.movement = movement_1;

	action_1.emotion = "ANGER";

	CutsceneEvent& event_1 = registry.cutscene_events.emplace(little_guy);
	event_1.actions.push_back(action_1);
	event_1.dialogue.push_back("Who are you? Explain yourself!");



	// TEMP ADD CUTSCENE EVENT 2
	Action action_2;

	Motion motion_2;
	motion_2.scale = { 80, 100 };
	motion_2.position = { 750, 400 };
	action_2.motion = motion_2;

	Movement movement_2;
	action_2.movement = movement_2;

	CutsceneEvent& event_2 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_2.actions.push_back(action_2);
	event_2.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_3;

	Motion motion_3;
	motion_3.scale = { 80, 100 };
	motion_3.position = { 400, 400 };
	action_3.motion = motion_3;

	Movement movement_3;
	movement_3.goal_pos = { 450, 400 };
	action_3.movement = movement_3;

	action_1.emotion = "ANGER";

	CutsceneEvent& event_3 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_3.actions.push_back(action_3);
	event_3.dialogue.push_back("How did you get into my dungeon? No - how did you get this far into my dungeon? Especially to this secret meeting room?");



	// TEMP ADD CUTSCENE EVENT 3
	Action action_4;

	Motion motion_4;
	motion_4.scale = { 80, 100 };
	motion_4.position = { 750, 400 };
	action_4.motion = motion_4;

	Movement movement_4;
	action_4.movement = movement_4;


	CutsceneEvent& event_4 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_4.actions.push_back(action_4);
	event_4.dialogue.push_back("...");





	// TEMP ADD CUTSCENE EVENT 5
	Action action_5;

	Motion motion_5;
	motion_5.scale = { 80, 100 };
	motion_5.position = { 450, 400 };
	action_5.motion = motion_5;

	Movement movement_5;
	movement_5.goal_pos = { 475, 400 };
	action_5.movement = movement_5;

	action_5.emotion = "ANGER";

	CutsceneEvent& event_5 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_5.actions.push_back(action_5);
	event_5.dialogue.push_back("Answer me!");



	// TEMP ADD CUTSCENE EVENT 6
	Action action_6;

	Motion motion_6;
	motion_6.scale = { 80, 100 };
	motion_6.position = { 750, 400 };
	action_6.motion = motion_6;

	Movement movement_6;
	action_6.movement = movement_6;

	CutsceneEvent& event_6 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_6.actions.push_back(action_6);
	event_6.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 7
	Action action_7;

	Motion motion_7;
	motion_7.scale = { 80, 100 };
	motion_7.position = { 475, 400 };
	action_7.motion = motion_7;

	Movement movement_7;
	movement_7.goal_pos = { 490, 400 };
	action_7.movement = movement_7;

	CutsceneEvent& event_7 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_7.actions.push_back(action_7);
	event_7.dialogue.push_back("I'm - I'm not scared of you!");



	// TEMP ADD CUTSCENE EVENT 8
	Action action_8;

	Motion motion_8;
	motion_8.scale = { 80, 100 };
	motion_8.position = { 750, 400 };
	action_8.motion = motion_8;

	Movement movement_8;
	action_8.movement = movement_8;

	CutsceneEvent& event_8 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_8.actions.push_back(action_8);
	event_8.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 8.1
	Action action_8_1;

	Motion motion_8_1;
	motion_8_1.scale = { 80, 100 };
	motion_8_1.position = { 750, 400 };
	action_8_1.motion = motion_8_1;

	Movement movement_8_1;
	movement_8_1.goal_pos = { 700, 400 };
	action_8_1.movement = movement_8_1;

	CutsceneEvent& event_8_1 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_8_1.actions.push_back(action_8_1);
	event_8_1.dialogue.push_back("Little Guy...");



	// TEMP ADD CUTSCENE EVENT 8.2
	Action action_8_2;

	Motion motion_8_2;
	motion_8_2.scale = { 80, 100 };
	motion_8_2.position = { 490, 400 };
	action_8_2.motion = motion_8_2;

	Movement movement_8_2;
	action_8_2.movement = movement_8_2;

	action_8_2.emotion = "SURPRISE";

	CutsceneEvent& event_8_2 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_8_2.actions.push_back(action_8_2);
	event_8_2.dialogue.push_back("!?");



	// TEMP ADD CUTSCENE EVENT 8.3
	Action action_8_3;

	Motion motion_8_3;
	motion_8_3.scale = { 80, 100 };
	motion_8_3.position = { 490, 400 };
	action_8_3.motion = motion_8_3;

	Movement movement_8_3;
	movement_8_3.goal_pos = { 475, 400 };
	action_8_3.movement = movement_8_3;

	CutsceneEvent& event_8_3 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_8_3.actions.push_back(action_8_3);
	event_8_3.dialogue.push_back("That voice... No... It can't be...");



	// TEMP ADD CUTSCENE EVENT 8.3
	Action action_8_4;

	Motion motion_8_4;
	motion_8_4.scale = { 80, 100 };
	motion_8_4.position = { 475, 400 };
	action_8_4.motion = motion_8_4;

	Movement movement_8_4;
	movement_8_4.goal_pos = { 500, 400 };
	action_8_4.movement = movement_8_4;

	CutsceneEvent& event_8_4 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_8_4.actions.push_back(action_8_4);
	event_8_4.dialogue.push_back("...Grandpa?");



	// TEMP ADD CUTSCENE EVENT 9
	Action action_9;

	Motion motion_9;
	motion_9.scale = { -80, 100 };
	motion_9.position = { 700, 400 };
	action_9.motion = motion_9;

	Movement movement_9;
	action_9.movement = movement_9;

	CutsceneEvent& event_9 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_9.actions.push_back(action_9);
	event_9.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 9
	Action action_9_1;

	Motion motion_9_1;
	motion_9_1.scale = { 80, 100 };
	motion_9_1.position = { 500, 400 };
	action_9_1.motion = motion_9_1;

	Movement movement_9_1;
	action_9_1.movement = movement_9_1;

	CutsceneEvent& event_9_1 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_9_1.actions.push_back(action_9_1);
	event_9_1.dialogue.push_back("It's you, isn't it...? I'd recognize your gravelly voice anywhere...");



	// TEMP ADD CUTSCENE EVENT 10
	Action action_10;

	Motion motion_10;
	motion_10.scale = { -80, 100 };
	motion_10.position = { 700, 400 };
	action_10.motion = motion_10;

	Movement movement_10;
	action_10.movement = movement_10;

	CutsceneEvent& event_10 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_10.actions.push_back(action_10);
	event_10.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 11
	Action action_11;

	Motion motion_11;
	motion_11.scale = { -80, 100 };
	motion_11.position = { 500, 400 };
	action_11.motion = motion_11;

	Movement movement_11;
	action_11.movement = movement_11;

	CutsceneEvent& event_11 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_11.actions.push_back(action_11);
	event_11.dialogue.push_back("You must've been the one to leak the dungeon layout... Only you would know how I'd manipulate these rooms... Only you would know that I'd come to this room when I'm stressed...");



	// TEMP ADD CUTSCENE EVENT 12
	Action action_12;

	Motion motion_12;
	motion_12.scale = { -80, 100 };
	motion_12.position = { 700, 400 };
	action_12.motion = motion_12;

	Movement movement_12;
	action_12.movement = movement_12;

	CutsceneEvent& event_12 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_12.actions.push_back(action_12);
	event_12.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_12_1;

	Motion motion_12_1;
	motion_12_1.scale = { 80, 100 };
	motion_12_1.position = { 500, 400 };
	action_12_1.motion = motion_12_1;

	Movement movement_12_1;
	action_12_1.movement = movement_12_1;

	CutsceneEvent& event_12_1 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_12_1.actions.push_back(action_12_1);
	event_12_1.dialogue.push_back("Why, Grandpa...? Why would you do this? Why would you attack us - no, why would you attack me?");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_12_2;

	Motion motion_12_2;
	motion_12_2.scale = { -80, 100 };
	motion_12_2.position = { 700, 400 };
	action_12_2.motion = motion_12_2;

	Movement movement_12_2;
	movement_12_2.goal_pos = { 675, 400 };
	action_12_2.movement = movement_12_2;

	CutsceneEvent& event_12_2 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_12_2.actions.push_back(action_12_2);
	event_12_2.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_12_3;

	Motion motion_12_3;
	motion_12_3.scale = { 80, 100 };
	motion_12_3.position = { 500, 400 };
	action_12_3.motion = motion_12_3;

	Movement movement_12_3;
	action_12_3.movement = movement_12_3;

	CutsceneEvent& event_12_3 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_12_3.actions.push_back(action_12_3);
	event_12_3.dialogue.push_back("Say something, Grandpa... Please...");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_12_4;

	Motion motion_12_4;
	motion_12_4.scale = { -80, 100 };
	motion_12_4.position = { 675, 400 };
	action_12_4.motion = motion_12_4;

	Movement movement_12_4;
	movement_12_4.goal_pos = { 650, 400 };
	action_12_4.movement = movement_12_4;

	CutsceneEvent& event_12_4 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_12_4.actions.push_back(action_12_4);
	event_12_4.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_12_5;

	Motion motion_12_5;
	motion_12_5.scale = { 80, 100 };
	motion_12_5.position = { 500, 400 };
	action_12_5.motion = motion_12_5;

	Movement movement_12_5;
	action_12_5.movement = movement_12_5;

	action_12_5.emotion = "ANGER";

	CutsceneEvent& event_12_5 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_12_5.actions.push_back(action_12_5);
	event_12_5.dialogue.push_back("JUST SAY SOMETHING!");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_12_6;

	Motion motion_12_6;
	motion_12_6.scale = { -80, 100 };
	motion_12_6.position = { 650, 400 };
	action_12_6.motion = motion_12_6;

	Movement movement_12_6;
	movement_12_6.goal_pos = { 625, 400 };
	action_12_6.movement = movement_12_6;

	CutsceneEvent& event_12_6 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_12_6.actions.push_back(action_12_6);
	event_12_6.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_12_7;

	Motion motion_12_7;
	motion_12_7.scale = { 80, 100 };
	motion_12_7.position = { 500, 400 };
	action_12_7.motion = motion_12_7;

	Movement movement_12_7;
	action_12_7.movement = movement_12_7;

	CutsceneEvent& event_12_7 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_12_7.actions.push_back(action_12_7);
	event_12_7.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_12_8;

	Motion motion_12_8;
	motion_12_8.scale = { 80, 100 };
	motion_12_8.position = { 500, 400 };
	action_12_8.motion = motion_12_8;

	Movement movement_12_8;
	action_12_8.movement = movement_12_8;

	CutsceneEvent& event_12_8 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_12_8.actions.push_back(action_12_8);
	event_12_8.dialogue.push_back("...If you keep moving forward, I'll have no choice but to attack you. I must protect my friends and family.");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_12_9;

	Motion motion_12_9;
	motion_12_9.scale = { -80, 100 };
	motion_12_9.position = { 625, 400 };
	action_12_9.motion = motion_12_9;

	Movement movement_12_9;
	movement_12_9.goal_pos = { 600, 400 };
	action_12_9.movement = movement_12_9;

	CutsceneEvent& event_12_9 = registry.cutscene_events.emplace_with_duplicates(imaginary_man);
	event_12_9.actions.push_back(action_12_9);
	event_12_9.dialogue.push_back("...");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_13;

	Motion motion_13;
	motion_13.scale = { 80, 100 };
	motion_13.position = { 500, 400 };
	action_13.motion = motion_13;

	Movement movement_13;
	action_13.movement = movement_13;

	CutsceneEvent& event_13 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_13.actions.push_back(action_13);
	event_13.dialogue.push_back("...So be it.");



	// TEMP ADD CUTSCENE EVENT 12.1
	Action action_13_1;

	Motion motion_13_1;
	motion_13_1.scale = { 80, 100 };
	motion_13_1.position = { 500, 400 };
	action_13_1.motion = motion_13_1;

	Movement movement_13_1;
	action_13_1.movement = movement_13_1;

	CutsceneEvent& event_13_1 = registry.cutscene_events.emplace_with_duplicates(little_guy);
	event_13_1.actions.push_back(action_13_1);
	event_13_1.dialogue.push_back("I won't hold back.");
}


void CutsceneLoader::load_dating_sim() {

}