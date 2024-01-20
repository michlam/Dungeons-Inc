#pragma once


///////////////
//  imports  //
/////////////// 
#include "render_system.hpp"
#include "utils.hpp"


///////////////
//   class   //
///////////////
class CutsceneLoader {
public:
	// Constructor
	CutsceneLoader() {
		// empty constructor
	}

	void CutsceneLoader::init(RenderSystem* renderer_arg);
	void CutsceneLoader::load_cutscene(int cutscene_to_play);

private:
	RenderSystem* renderer;

	void CutsceneLoader::load_cutscene_0();
	void CutsceneLoader::load_cutscene_2();
	void CutsceneLoader::load_cutscene_4();
	void CutsceneLoader::load_cutscene_5();
	void CutsceneLoader::load_cutscene_6();
	void CutsceneLoader::load_cutscene_7();
	void CutsceneLoader::load_dating_sim();

	/*void CutsceneLoader::create_little_guy();
	void CutsceneLoader::create_knight();
	void CutsceneLoader::create_archer();
	void CutsceneLoader::create_wizard();
	void CutsceneLoader::create_goblin();

	void CutsceneLoader::create_static_scene(int scene_to_play);*/
};

// single declaration of cutscene system
extern CutsceneLoader cutscene_loader;