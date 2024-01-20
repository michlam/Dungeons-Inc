#pragma once


///////////////
//  imports  //
/////////////// 
#include "render_system.hpp"


///////////////
//   class   //
///////////////
class CutsceneSystem {
public:
	// Constructor
	CutsceneSystem() {
		// empty constructor
	}

	void CutsceneSystem::init(RenderSystem* renderer_arg);
	void CutsceneSystem::play(int cutscene_to_play);
	void CutsceneSystem::step(float elapsed_ms_since_last_update);

	void CutsceneSystem::handle_cutscene_advancement(GLFWwindow* window, int key, int action, int mod);

	void CutsceneSystem::reset_system();

private:
	RenderSystem* renderer;

	Entity background;

	void CutsceneSystem::spawn_goblins();

	void CutsceneSystem::render_cutscene_choices();
	void CutsceneSystem::create_cutscene_choice(std::string choice, vec2 position);

	void CutsceneSystem::handle_dialogue_advance(double xpos, double ypos);
	void CutsceneSystem::handle_choice_advance(double xpos, double ypos);

	void CutsceneSystem::add_emotion(Entity entity, std::string emotion);
	void CutsceneSystem::remove_emotions();
};

// single declaration of cutscene system
extern CutsceneSystem cutscene_system;