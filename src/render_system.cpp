///////////////
//  imports  //
///////////////
// common
#include "world_init.hpp"

// stdlib
#include <SDL.h>
#include <algorithm>
#include <iostream>

// systems
#include "card/card_functions.hpp"
#include "render_system.hpp"
#include "phase/phase_system.hpp"
#include "tiny_ecs_registry.hpp"
#include <world_init.hpp>
#include <physics_system.hpp>
#include <phase/phase_system.hpp>
#include <level_select/level_select_system.hpp>
#include "map_system.hpp"
#include "level/level_system.hpp"
#include "menu/menu_system.hpp"
#include "cutscene/cutscene_system.hpp"



///////////////
// variables //
///////////////
vec3 cameraPosition = { 0.0, 0.0, 1.0 };
vec2 map_dimensions = { 1.25 * window_width_px, 2 * window_height_px };

// A string to hold the current scene, initialized to Intro
scene current_scene = INTRO;


///////////////
// functions //
/////////////// 
// Following drawTexturedMesh outline to draw text
void RenderSystem::draw_text(Entity entity, const mat3& projection) {
	Text& text = registry.texts.get(entity);
	Transform transform;

	// scale text as needed to aspect ratio scale
	transform.translate(text.position * aspect_ratio_scale);
	vec2 scale = vec2(text.scale, text.scale);
	transform.scale(scale * aspect_ratio_scale);

	const GLuint used_effect_enum = (GLuint)EFFECT_ASSET_ID::TEXT;
	assert(used_effect_enum != (GLuint)EFFECT_ASSET_ID::EFFECT_COUNT);
	const GLuint program = (GLuint)effects[used_effect_enum];

	assert(GEOMETRY_BUFFER_ID::TEXT_BOX != GEOMETRY_BUFFER_ID::GEOMETRY_COUNT);
	const GLuint vbo = vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::TEXT_BOX];
	const GLuint ibo = index_buffers[(GLuint)GEOMETRY_BUFFER_ID::TEXT_BOX];

	// Set vbo and ibo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	// Input data location as in the vertex buffer
	GLint in_position_loc = glGetAttribLocation(program, "in_position");
	GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");

	gl_has_errors();
	assert(in_texcoord_loc >= 0);

	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
		sizeof(TexturedVertex), (void*)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void*)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	// Loop through each text line and render them one by one
	for (int i = 0; i < NUM_TEXT_LINES; i++) {
		std::string text_message = text.text_lines[i];

		float x = text.position.x;
		float y = text.position.y + 48 * i * text.scale;
		float scale = text.scale;

		// Getting uniform locations for glUniform* calls
		GLint color_uloc = glGetUniformLocation(program, "fcolor");
		const vec4 color = vec4(text.colour, 1.f);
		glUniform4fv(color_uloc, 1, (float*)&color);
		gl_has_errors();

		GLint currProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);

		// Draw the text message
		std::string::const_iterator c;
		for (c = text_message.begin(); c != text_message.end(); c++) {
			// Setting uniform values to the currently bound program
			GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
			glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float*)&transform.mat);
			GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
			glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float*)&projection);
			gl_has_errors();

			Character ch = Characters[*c];
			float pos_x = x + ch.Bearing.x * scale;
			float pos_y = y - ch.Bearing.y * scale;
			float width = ch.Size.x * scale;
			float height = ch.Size.y * scale;



			// Update VBO for every character
			std::vector<TexturedVertex> textured_vertices(4);
			textured_vertices[0].position = { pos_x, pos_y + height, 0.f };
			textured_vertices[1].position = { pos_x + width, pos_y + height, 0.f };
			textured_vertices[2].position = { pos_x + width, pos_y, 0.f };
			textured_vertices[3].position = { pos_x, pos_y, 0.f };
			textured_vertices[0].texcoord = { 0.f, 1.f };
			textured_vertices[1].texcoord = { 1.f, 1.f };
			textured_vertices[2].texcoord = { 1.f, 0.f };
			textured_vertices[3].texcoord = { 0.f, 0.f };

			const std::vector<uint16_t> textured_indices = { 0, 3, 1, 1, 3, 2 };
			// update VBO
			bindVBOandIBO(GEOMETRY_BUFFER_ID::TEXT_BOX, textured_vertices, textured_indices);
			gl_has_errors();

			// draw glyph
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// draw text box
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
			// Advance to next glyph
			x += (ch.Advance >> 6) * scale;
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	gl_has_errors();
}

//draw all blood particles, referencing drawTexturedMesh 
void RenderSystem::draw_particles(const mat3& projection) {
	
	if (registry.blood.entities.size() == 0) {
		return;
	}
	//printf("%d \n", registry.blood.entities.size());

	std::vector<vec4> inputs;//stores xpos, ypox, size, alpha as floats
	
	for (Entity entity : registry.blood.entities) {


		Motion& motion = registry.motions.get(entity);
		Blood& blood = registry.blood.get(entity);

		/*
		Transform transform;
		transform.translate(motion.position * aspect_ratio_scale);
		transform.rotate(motion.angle);
		transform.scale(motion.scale * aspect_ratio_scale);

		transforms.push_back(transform.mat);
		*/
		inputs.push_back({ motion.position * aspect_ratio_scale,
			motion.scale.x * aspect_ratio_scale.x,
			blood.alpha
			});
	}

	const GLuint used_effect_enum = (GLuint)EFFECT_ASSET_ID::PARTICLE; 
	const GLuint program = (GLuint)effects[used_effect_enum]; 
	// Setting shaders
	glUseProgram(program);  
	gl_has_errors();    

	const GLuint vbo = vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE];
	const GLuint ibo = index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE]; 

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	GLint in_position_loc = glGetAttribLocation(program, "in_position");
	GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord"); 
	gl_has_errors();
	assert(in_texcoord_loc >= 0); 

	glEnableVertexAttribArray(in_position_loc); 
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
		sizeof(TexturedVertex), (void*)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc); 
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void*)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0); 
	gl_has_errors();


	GLuint texture_id =
		texture_gl_handles[(GLuint)TEXTURE_ASSET_ID::RED_PARTICLE]; 

	glBindTexture(GL_TEXTURE_2D, texture_id); 
	gl_has_errors();

	

	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	const vec3 color = vec3(1);
	glUniform3fv(color_uloc, 1, (float*)&color);
	gl_has_errors();

	GLuint projection_loc = glGetUniformLocation(program, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float*)&projection);
	gl_has_errors();

	// array instancing here: 
	unsigned int instanceVBO1;
	glGenBuffers(1, &instanceVBO1);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO1);

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(glm::vec4) * registry.blood.entities.size(),
		&inputs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
		(void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);

	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	(void)num_indices;

	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 6, registry.blood.size());
	gl_has_errors();

}



void RenderSystem::drawTexturedMesh(Entity entity, const mat3& projection) {
	Motion& motion = registry.motions.get(entity);

	Transform transform;
	transform.translate(motion.position * aspect_ratio_scale);
	transform.rotate(motion.angle);
	transform.scale(motion.scale * aspect_ratio_scale);

	assert(registry.renderRequests.has(entity));
	const RenderRequest& render_request = registry.renderRequests.get(entity);

	const GLuint used_effect_enum = (GLuint)render_request.used_effect;
	assert(used_effect_enum != (GLuint)EFFECT_ASSET_ID::EFFECT_COUNT);
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	assert(render_request.used_geometry != GEOMETRY_BUFFER_ID::GEOMETRY_COUNT);
	const GLuint vbo = vertex_buffers[(GLuint)render_request.used_geometry];
	const GLuint ibo = index_buffers[(GLuint)render_request.used_geometry];

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();


	// Input data location as in the vertex buffer
	if (render_request.used_effect == EFFECT_ASSET_ID::TEXTURED || render_request.used_effect == EFFECT_ASSET_ID::TEXTURED_ANIMATION ||
		render_request.used_effect == EFFECT_ASSET_ID::HEALTHBAR) {
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");

		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(TexturedVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void*)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position
		

		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();


		//apply fade if applicable
		float fade_factor = 1.f;
		if (registry.fading.has(entity)) {
			fade_factor = registry.fading.get(entity).fade_factor;
			// printf("fade factor: %f", fade_factor);
		}
		GLint fade_factor_uloc = glGetUniformLocation(program, "fade_factor");
		glUniform1f(fade_factor_uloc, fade_factor);
		gl_has_errors();
		
		if (render_request.used_effect == EFFECT_ASSET_ID::HEALTHBAR) {
			//do something :/

			HealthBar& hb = registry.healthBars.get(entity);
			GLint health_uloc = glGetUniformLocation(program, "health");
			glUniform1f(health_uloc, (float) hb.health);
		}
		


		if (render_request.used_effect == EFFECT_ASSET_ID::TEXTURED_ANIMATION ||
			render_request.used_effect == EFFECT_ASSET_ID::LSM_LITTLE_GUY) {
			drawAnimatedSprite(entity);
		}

	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::BG0 || render_request.used_effect == EFFECT_ASSET_ID::BG1 ||
		render_request.used_effect == EFFECT_ASSET_ID::BG2 || render_request.used_effect == EFFECT_ASSET_ID::BG3) {
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");

		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(TexturedVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void*)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position



		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();

		// Set texture parameters (including wrap mode)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::LSM_TEXTURED || render_request.used_effect == EFFECT_ASSET_ID::LSM_LITTLE_GUY
		|| render_request.used_effect == EFFECT_ASSET_ID::LSM_GREYED) {
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");

		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(TexturedVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void*)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position



		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();

		//apply fade if applicable
		float fade_factor = 1.f;
		if (registry.fading.has(entity)) {
			fade_factor = registry.fading.get(entity).fade_factor;
			// printf("fade factor: %f", fade_factor);
		}
		GLint fade_factor_uloc = glGetUniformLocation(program, "fade_factor");
		glUniform1f(fade_factor_uloc, fade_factor);
		gl_has_errors();

		if (render_request.used_effect == EFFECT_ASSET_ID::HEALTHBAR) {
			//do something :/

			HealthBar& hb = registry.healthBars.get(entity);
			GLint health_uloc = glGetUniformLocation(program, "health");
			glUniform1f(health_uloc, (float)hb.health);
		}


		if (render_request.used_effect == EFFECT_ASSET_ID::TEXTURED_ANIMATION ||
			render_request.used_effect == EFFECT_ASSET_ID::LSM_LITTLE_GUY) {
			drawAnimatedSprite(entity);
		}
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::GLOWING) {
		const GLuint glow_program = effects[(GLuint)EFFECT_ASSET_ID::GLOWING];

		// Set clock
		GLuint time_uloc = glGetUniformLocation(glow_program, "time");
		glUniform1f(time_uloc, (float)(glfwGetTime()));
		gl_has_errors();

		GLint in_position_loc = glGetAttribLocation(glow_program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(glow_program, "in_texcoord");
		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(TexturedVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void*)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position

		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();

	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::PEBBLE || render_request.used_effect == EFFECT_ASSET_ID::LOGO_TEXT) {
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_color_loc = glGetAttribLocation(program, "in_color");
		gl_has_errors();

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_color_loc);
		glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)sizeof(vec3));

		gl_has_errors();
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::PEBBLE || render_request.used_effect == EFFECT_ASSET_ID::LOGO_TEXT) {
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_color_loc = glGetAttribLocation(program, "in_color");
		gl_has_errors();

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_color_loc);
		glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)sizeof(vec3));

		gl_has_errors();
	}
	else {
		assert(false && "Type of render request not supported");
	}

	// Getting uniform locations for glUniform* calls
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	const vec3 color = registry.colors.has(entity) ? registry.colors.get(entity) : vec3(1);
	glUniform3fv(color_uloc, 1, (float*)&color);




	// Retrieve screen width and height
	int screenWidth = window_width_px;
	int screenHeight = window_height_px;

	// Get the location of the uniform variable in the shader
	GLuint resolutionLocation = glGetUniformLocation(program, "u_resolution");

	// Set the screen resolution in the shader
	glUseProgram(program);
	glUniform2f(resolutionLocation, screenWidth, screenHeight);


	// Uniform for camera position (we only need y position!)
	GLuint cameraYPos = glGetUniformLocation(program, "camera_y");

	// Set the y position in the shader
	glUseProgram(program);
	glUniform1f(cameraYPos, get_camera_matrix().y);

	gl_has_errors();



	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
	glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float*)&transform.mat);
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float*)&projection);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

void RenderSystem::drawAnimatedSprite(Entity entity) {

	Animation& animations = registry.animations.get(entity);
	GLuint textured_animation_program = effects[(GLuint) registry.renderRequests.get(entity).used_effect];
	glUseProgram(textured_animation_program);


	GLint frame_height_uloc = glGetUniformLocation(textured_animation_program, "frame_height");
	glUniform1f(frame_height_uloc, animations.sprite_h);
	GLint frame_width_uloc = glGetUniformLocation(textured_animation_program, "frame_width");
	glUniform1f(frame_width_uloc, animations.sprite_w);

	GLint animation_frame_uloc = glGetUniformLocation(textured_animation_program, "frame");
	glUniform1f(animation_frame_uloc, (float)animations.curr_frame);

	GLint animation_type_uloc = glGetUniformLocation(textured_animation_program, "animation_type");
	glUniform1f(animation_type_uloc, (float)animations.animation_type);


	const RenderRequest& render_request = registry.renderRequests.get(entity);


	gl_has_errors();
}



// draw the intermediate texture to the screen, with some distortion to simulate
// water
void RenderSystem::drawToScreen()
{
	// Setting shaders
	// get the water texture, sprite mesh, and program
	glUseProgram(effects[(GLuint)EFFECT_ASSET_ID::WATER]);
	gl_has_errors();
	// Clearing backbuffer
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
	glDepthRange(0, 10);
	glClearColor(1.f, 0, 0, 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_has_errors();
	// Enabling alpha channel for textures
	glDisable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// Draw the screen texture on the quad geometry
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]);
	glBindBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]); // Note, GL_ELEMENT_ARRAY_BUFFER associates
	// indices to the bound GL_ARRAY_BUFFER
	gl_has_errors();
	const GLuint water_program = effects[(GLuint)EFFECT_ASSET_ID::WATER];
	// Set clock
	GLuint time_uloc = glGetUniformLocation(water_program, "time");
	GLuint dead_timer_uloc = glGetUniformLocation(water_program, "screen_darken_factor");
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
	ScreenState& screen = registry.screenStates.get(screen_state_entity);
	glUniform1f(dead_timer_uloc, screen.screen_darken_factor);
	gl_has_errors();
	// Set the vertex position and vertex texture coordinates (both stored in the
	// same VBO)
	GLint in_position_loc = glGetAttribLocation(water_program, "in_position");
	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	gl_has_errors();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, off_screen_render_buffer_color);
	gl_has_errors();
	// Draw
	glDrawElements(
		GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,
		nullptr); // one triangle = 3 vertices; nullptr indicates that there is
	// no offset from the bound index buffer
	gl_has_errors();
}


mat3 RenderSystem::createProjectionMatrix() {
	// Fake projection matrix, scales with respect to window coordinates
	float left = 0.f;
	float top = 0.f;
	gl_has_errors();
	float right = (float)window_width_px;
	float bottom = (float)window_height_px;
	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	return { {sx, 0.f, 0.f}, {0.f, sy, 0.f}, {tx, ty, 1.f} };
}


// simple helper to make the functions clearer to read
void RenderSystem::renderEntity(Entity entity, mat3 projection_matrix) {

	if (registry.renderRequests.get(entity).used_effect == EFFECT_ASSET_ID::PARTICLE) {
		// draw_particles(entity, projection_matrix);
	} else {
		// draw the entity's sprite
		drawTexturedMesh(entity, projection_matrix);
		// if the sprite also has associated text, draw that too (on top of the sprite)
		if (registry.texts.has(entity)) {
			draw_text(entity, projection_matrix);
		}
	}
}


// Render our game world
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void RenderSystem::draw() {
	// Getting size of window
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays

	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	gl_has_errors();
	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);
	// control color of background
	glClearColor(1, 1, 1, 1.0);
	glClearDepth(10.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);		// native OpenGL does not work with a depth buffer
	// and alpha blending, one would have to sort
	// sprites back to front

	gl_has_errors();

	mat3 viewMatrix = mat3(1.0);
	viewMatrix[2] = cameraPosition;
	projection_2D = createProjectionMatrix();
	projection_view_matrix = projection_2D * viewMatrix;

	// Controls the depth at which things are rendered
	for (std::string layer : layers) {

		// blood particles rendered as instances, not contained in renderRequests container
		if (layer == "CARDS_ON_ROOM") {
			draw_particles(projection_view_matrix);
		}
		// Draw all textured meshes that have a position and size component
		for (Entity entity : registry.renderRequests.entities) {
			// Note, its not very efficient to access elements indirectly via the entity
			// albeit iterating through all Sprites in sequence. A good point to optimize
			RenderRequest& rr = registry.renderRequests.get(entity);
			// check if the entity has a motion; if not, cannot be rendered so need to skip it
			// the text check is temporary as we refactor text to remove duplicate motion data
			if (registry.motions.has(entity) || registry.texts.has(entity)) {
				// check if the render request is on the correct layer to be rendered
				if (rr.z_val == layer) {
					// if a ui-stickied component, needs to be passed a different projection matrix to make it stickied
					mat3 projection_matrix = (registry.uiComponents.has(entity)) ? projection_2D : projection_view_matrix;
					// now render the entity using the appropriate matrix
					renderEntity(entity, projection_matrix);
				}
			}
		}
	}

	// Truely render to the screen
	drawToScreen();
	// flicker-free display with a double buffer
	glfwSwapBuffers(window);

	gl_has_errors();
}


// handles moving the camera view
void RenderSystem::on_keyboard(int key, int scancode, int action, int mods) {
	// Adjust camera position based on key events
	if (current_scene == IN_FULL_MAP_VIEW && !is_paused && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		Motion last_room = registry.motions.get(registry.room.entities[registry.room.entities.size() - 1]);
		int depth = registry.levels.get(*current_level).depth;
		switch (key) {
		case GLFW_KEY_UP:
			cameraPosition.y = std::min(cameraPosition.y + 30.0, (double)(-1 * (last_room.position.y - last_room.scale.y)) * aspect_ratio_scale.y);
			break;
		case GLFW_KEY_DOWN:
			cameraPosition.y = std::max(cameraPosition.y - 30.0, (double)(0));
			break;
		case GLFW_KEY_LEFT:
			cameraPosition.x = std::min(cameraPosition.x + 10.0, (double)(map_dimensions.x / 2 - window_width_px / 2));
			break;
		case GLFW_KEY_RIGHT:
			cameraPosition.x = std::max(cameraPosition.x - 10.0, (double)(-1 * map_dimensions.x / 2 + window_width_px / 2));
			break;
			// Add more keys for camera movement as needed
		}
	}

	// cannot use keyboard commands if menu is open
	if (!menu_opened && current_scene == IN_GAME) phase_system.keyboard_handler(key, action);
}


// handles clicking various entities
void RenderSystem::on_mouse_click(GLFWwindow* window, int key, int action, int mod) {
	if ((current_scene == LEVEL_SELECT || current_scene == IN_GAME || current_scene == CUTSCENE || current_scene == IN_FULL_MAP_VIEW)) {
		menu_mouse_click_handler(window, key, action, mod);
	}
	if (current_scene == CUTSCENE) {
		cutscene_system.handle_cutscene_advancement(window, key, action, mod);
	}
	if (!menu_opened && current_scene == LEVEL_SELECT) {
		lsm_mouse_click_handler(window, key, action, mod);
	}
	if (!menu_opened && (current_scene == IN_GAME || current_scene == IN_FULL_MAP_VIEW)) {
		map_view_mouse_click_handler(window, key, action, mod);
	}

	// USER CAN ONLY INTERACT WITH CARDS DURING PLAN PHASE
	if (!menu_opened && current_scene == IN_GAME && registry.phases.components[0].type == PLAN) {
		if (key == GLFW_MOUSE_BUTTON_LEFT && !is_paused) {
			if (action == GLFW_PRESS) {
				// fetch mouse cursor position and sprite motion data
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);

				// NOTE: should change this to some component Interactable to signify that the cursor can interact with it
				auto& cards_container = registry.cards;
				for (int i = 0; i < cards_container.size(); i++) {
					// get current card entity and its motion info
					Entity card = cards_container.entities[i];
					Motion& motion = registry.motions.get(card);
					float card_xpos = motion.position.x * aspect_ratio_scale.x;
					float card_ypos = motion.position.y * aspect_ratio_scale.y;
					float card_width = abs(motion.scale.x) * aspect_ratio_scale.x;
					float card_height = abs(motion.scale.y) * aspect_ratio_scale.y;

					// find card's bounding box
					float x_left_bound = card_xpos - (card_width / 2);
					float x_right_bound = card_xpos + (card_width / 2);
					float y_up_bound = card_ypos - (card_height / 2);
					float y_down_bound = card_ypos + (card_height / 2);

					// if the card is currently a ui element then have to handle that case...
					if (!registry.uiComponents.has(card)) {
						x_left_bound += cameraPosition.x;
						x_right_bound += cameraPosition.x;
						y_up_bound += cameraPosition.y;
						y_down_bound += cameraPosition.y;
					}

					// check to see if mouse cursor is within the card's bounding box
					if ((x_left_bound <= xpos) && (xpos <= x_right_bound) && (y_up_bound <= ypos) && (ypos <= y_down_bound)) {
						// max 1 cursor focus at a time
						registry.cursorFocus.clear();
						// if it is, adds a cursor focus component to it to signify that the cursor is currently clicking it
						registry.cursorFocus.emplace(card);
						// fix z value such that card and text are always on top
						registry.renderRequests.get(card).z_val = "PRIORITY";
						// remove from 2d world
						registry.uiComponents.remove(card);
					}
				}

				// card debugging function
				//// next check if cursor clicked the deck
				//if (registry.deck.size() == 1) {
				//	Entity deck = registry.deck.entities[0];
				//	Motion& motion = registry.motions.get(deck);
				//	float deck_xpos = motion.position.x;
				//	float deck_ypos = motion.position.y;
				//	float deck_width = abs(motion.scale.x);
				//	float deck_height = abs(motion.scale.y);

				//	// find bounding box
				//	float x_left_bound = deck_xpos - (deck_width / 2);
				//	float x_right_bound = deck_xpos + (deck_width / 2);
				//	float y_up_bound = deck_ypos - (deck_height / 2);
				//	float y_down_bound = deck_ypos + (deck_height / 2);

				//	// deck is always a uiComponent so no need to handle any special case

				//	if ((x_left_bound <= xpos) && (xpos <= x_right_bound) && (y_up_bound <= ypos) && (ypos <= y_down_bound)) {
				//		// if mouse click on deck, DRAW CARD but only if there are cards avaiable to draw
				//		// make sure the hand exists
				//		assert(draw_card_from_deck(this));
				//		// draw_card_from_deck(deck, registry.hand.entities[0], this);
				//	}
				//}
			}
			else if (action == GLFW_RELEASE) {

				if (registry.cursorFocus.size() > 0) {
					// cursor focus is never a uicomponent at this point
					Entity primary_focus = registry.cursorFocus.entities[0];

					// fix z value such that card and text lose their priority-ness
					RenderRequest& card_render = registry.renderRequests.get(primary_focus);
					card_render.z_val = "CARDS";

					// temporary check that cursor focus is a card
					assert(registry.cards.has(primary_focus));

					// at this point, the card is no longer associated with its previous room (if it had one)
					// this will also remove it from being in play
					remove_card_from_room(primary_focus);

					// fetch secondary focus (if it exists)
					// if no valid secondary focus set, then ignore this next part
					if (registry.secondaryFocus.size()) {
						Entity secondary_focus = registry.secondaryFocus.entities[0];

						// if secondary focus is a card, merge cards
						if (registry.cards.has(secondary_focus)) {
							
							// if they are mergeable, merge the two cards into one card
							// this also checks if they are mergeable
							assert(merge_cards(primary_focus, secondary_focus));

						// otherwise, secondary focus must be a room - so handle card-room collision
						} else {
							
							// apply the card to the room
							Entity corresponding_room = registry.correspondingRoom.get(secondary_focus).room;
							assert(apply_card_to_room(primary_focus, corresponding_room));

							// once a card is on a room, it doesn't need to move with the ui
							registry.uiComponents.remove(primary_focus);
						}

					// at this point, if the card didn't snap, it wasn't hovering over a room
					// so readd its ui stickiness if needed
					} else {
						if (!registry.uiComponents.has(primary_focus)) {
							registry.uiComponents.emplace(primary_focus);
						}
					}
				}

				// when button is released, REMOVE ALL CURSORFOCUS COMPONENTS because a cursor can only touch ONE entity at a time
				// also handle removing secondary focuses too
				// if the cursor_focus currently has a secondary focus, remove any glowing effect it may have had
				if (registry.secondaryFocus.size() > 0) {
					RenderRequest& secondary_focus = registry.renderRequests.get(registry.secondaryFocus.entities[0]);
					secondary_focus.used_effect = EFFECT_ASSET_ID::TEXTURED;
				}
				registry.cursorFocus.clear();
				registry.secondaryFocus.clear();

				// reformat hand and cards on rooms, in case of any changes
				reformat_hand();

				// set the card's position to be that of the room's
				// if there are multiple cards on a room, space them out appropriately
				for (Entity room : registry.cards_on_room.entities) {
					std::vector<Entity> corresponding_rooms = registry.correspondingRoom.entities;
					Entity actual_room = room;
					for (int i = 0; i < corresponding_rooms.size(); i++) {
						if (registry.correspondingRoom.get(corresponding_rooms[i]).room == room) {
							actual_room = corresponding_rooms[i];
							break;
						}
					}
					reformat_cards(registry.cards_on_room.get(room).cards, registry.motions.get(actual_room).position);
				}
			}
		}
	}
}


// handles card drag and stat window ui hover
void RenderSystem::on_mouse_move(vec2 mouse_position) {
	if (current_scene == LEVEL_SELECT) {
		handle_lsm_selector_drag(mouse_position);
	}

	if (current_scene == IN_GAME && !is_paused && !menu_opened) {
		handle_card_drag(mouse_position);

		// this now gets called in world_systems's step
		//handle_char_hover(mouse_position);
	}
}


// handles mouse click + drag for moving cards across the screen 
void RenderSystem::handle_card_drag(vec2 mouse_position) {
	// USER CANNOT DRAG CARDS UNLESS ITS PLAN PHASE
	if (current_scene == IN_GAME && registry.phases.components[0].type == PLAN) {
		// check to see if there is a cursor focus or not
		auto& cursor_container = registry.cursorFocus;
		if (cursor_container.size() > 0) {
			// fetch the cursor focus - there is a max of 1 cursor focus in the container
			// so the correct cursor focus is always at index 0
			auto& cursor_entity = cursor_container.entities[0];
			auto& motion = registry.motions.get(cursor_entity);
			float xpos = mouse_position.x / aspect_ratio_scale.x;
			float ypos = mouse_position.y / aspect_ratio_scale.y;

			// printf("Mouse XPOS: %f\n", xpos);
			// printf("Mouse YPOS: %f\n", ypos);

			// find boundaries of the game window relative to the background
			float x_left_bound = 0;
			float x_right_bound = window_width_px / aspect_ratio_scale.x;
			float y_up_bound = 0;
			float y_down_bound = window_height_px / aspect_ratio_scale.y;

			// card text entity motion
			auto& text_motion = registry.texts.get(cursor_entity);

			// boundary check for the game window so the sprite cannot be placed outside of bounds
			// NOTE: make this smoother by doing min/max instead of if-check
			if ((x_left_bound < xpos) && (xpos < x_right_bound) && (y_up_bound < ypos) && (ypos < y_down_bound)) {
				motion.position.x = xpos;
				motion.position.y = ypos;

				vec2 scaler = vec2(1.f / 3.f);

				// { (2.095 * scaler.x) * curr_x_pos - (90 * scaler.x), (2.095 * scaler.y) * center_point.y - (110.5 * scaler.y) }

				text_motion.position.x = (2.095 * scaler.x) * xpos - (90 * scaler.x);
				text_motion.position.y = (2.095 * scaler.y) * ypos - (110.5 * scaler.y);

				// if the card is currently a ui element then have to handle that case...
				if (!registry.uiComponents.has(cursor_entity)) {
					motion.position.x -= cameraPosition.x / aspect_ratio_scale.x;
					motion.position.y -= cameraPosition.y / aspect_ratio_scale.y;
					text_motion.position.x -= (2.095 * scaler.x) * (cameraPosition.x / aspect_ratio_scale.x);
					text_motion.position.y -= (2.095 * scaler.y) * (cameraPosition.y / aspect_ratio_scale.y);
				}
			}
		}
	}
}


void RenderSystem::handle_char_hover(vec2 mouse_position) {


	auto& char_container = registry.characters;
	auto& stat_box_container = registry.statusBoxes;

	for (uint i = 0; i < char_container.entities.size(); i++) {
		//if character is hovered, render the statbox
		Entity entity = char_container.entities[i];

		Entity statBox = char_container.components[i].stat_box;
		
		//copy is ok here
		Motion motion = registry.motions.get(entity);


		// i dont know why this is necessary but it is
		motion.position.x += cameraPosition.x / aspect_ratio_scale.x;
		motion.position.y += cameraPosition.y / aspect_ratio_scale.y;
		motion.position *= aspect_ratio_scale;

		if (hovers(motion, mouse_position)) {

			StatusBox& stat_box = registry.statusBoxes.get(statBox);

			Motion& statBox_motion = registry.motions.get(statBox);
			statBox_motion.position = { mouse_position[0] / aspect_ratio_scale.x + STATBOX_WIDTH / 2,
										mouse_position[1] / aspect_ratio_scale.y - STATBOX_HEIGHT / 2 };


			if (!registry.renderRequests.has(statBox)) {
				registry.renderRequests.insert(
					statBox,
					{ TEXTURE_ASSET_ID::UI_WINDOW,
						EFFECT_ASSET_ID::TEXTURED,
						GEOMETRY_BUFFER_ID::SPRITE,
						"CARDS_ON_ROOM" });
				registry.uiComponents.emplace(statBox);

			}
			//text rendering

			Text text = Text{};

			std::string class_name = "";

			Stats stat_changes;

			if (registry.stat_changes.has(entity)) {
				stat_changes = registry.stat_changes.get(entity).stat_change;
			}

			if (registry.adventurerClass.has(entity)) {

				switch (registry.adventurerClass.get(entity).advClass) {
				case Class::WARRIOR:
					class_name = "WARRIOR";
					break;
				case Class::MAGE:
					class_name = "MAGE";
					break;
				case Class::RANGER:
					class_name = "RANGER";
					break;
				default:
					class_name = "!!ERROR!!";
					break;
				}

			}
			else if (registry.mobClass.has(entity)) {
				switch (registry.mobClass.get(entity).mobClass) {
				case MobClass::BARBARIAN:
					class_name = "BARBARIAN";
					break;
				case MobClass::BRUTE:
					class_name = "BRUTE";
					break;
				case MobClass::SHAMAN:
					class_name = "SHAMAN";
					break;
				default:
					class_name = "!!ERROR!!";
					break;
				}
			}

			Stats& stats = char_container.components[i].stats;

			text.text_lines[0] = class_name;
			text.text_lines[1] = "hp: " + std::to_string(stats.hp + stat_changes.hp);
			text.text_lines[2] = "atk: " + std::to_string(stats.atk + stat_changes.atk);
			text.text_lines[3] = "def: " + std::to_string(stats.def + stat_changes.def);

			text.colour = { 0.0, 0.0, 0.0 };

			text.scale = 1;
			vec2 scaler = vec2(1.f / 3.f);
			text.position = { (2 * scaler.x) * statBox_motion.position.x - ((STATBOX_WIDTH / 2) * scaler.x), 
							  (2 * scaler.y) * statBox_motion.position.y - ((STATBOX_HEIGHT / 2) * scaler.y) };


			// remove and insert new text
			if (registry.texts.has(statBox)) {
				registry.texts.remove(statBox);
			}
			registry.texts.insert(statBox, text);

		}
		else {
			if (registry.renderRequests.has(statBox)) {
				registry.renderRequests.remove(statBox);
				registry.uiComponents.remove(statBox);
				registry.texts.remove(statBox);

			}
		}
	}

	//bad stuff just ignore it
	/*
	auto& adv_container = registry.adventurerClass;
	auto& stat_box_container = registry.statusBoxes;

	for (uint i = 0; i < adv_container.components.size(); i++) {
		Entity entity_i = adv_container.entities[i];
		Motion motion_i = registry.motions.get(entity_i);
		motion_i.position.x += cameraPosition.x / aspect_ratio_scale.x;
		motion_i.position.y += cameraPosition.y / aspect_ratio_scale.y;


		if (stat_box_container.components.size() > 0) {
			for (uint j = 0; j < stat_box_container.components.size(); j++) {
				Entity entity_j = stat_box_container.entities[j];
				stat_box_container.remove(entity_j);
				registry.remove_all_components_of(entity_j);
			}
		}

		Motion curr_motion_i = motion_i;
		
		//TODO: make the bounding box positions more accurate - Harvir
		curr_motion_i.position *= aspect_ratio_scale;

		if (hovers(curr_motion_i, mouse_position)) {
			Stats stats = registry.characters.get(entity_i).stats;
			Stats stat_changes = registry.stat_changes.has(entity_i) ? registry.stat_changes.get(entity_i).stat_change : Stats();
			Class::CLASS adv_class = registry.adventurerClass.get(entity_i).advClass;

			//char class[] = registry.adventurerClass.get(entity_i).advClass

			float x_pos = mouse_position[0] / aspect_ratio_scale.x + STATBOX_WIDTH / 2;
			float y_pos = mouse_position[1] / aspect_ratio_scale.y - STATBOX_HEIGHT / 2;
			Entity statBox = createAdvStatBox(this, vec2{ x_pos, y_pos }, stats, adv_class);

			Motion& parent_motion = registry.motions.get(statBox);
			Entity textBox = createAdvStatText(statBox, parent_motion.position, stats, stat_changes, adv_class);

			break;
		}
	}
	*/
}

void RenderSystem::handle_lsm_selector_drag(vec2 mouse_position) {
	// USER CANNOT DRAG SELECTOR UNLESS IT'S LEVEL SELECT SCENE
	if (current_scene == LEVEL_SELECT && selector_clicked) {
		// check to see if there is a cursor focus or not
		auto& cursor_container = registry.cursorFocus;
		if (cursor_container.size() > 0) {

			float mouse_change = 0;

			// loop over the level select buttons:
			for (int i = 0; i < registry.lsmButtons.entities.size(); i++) {
				Entity button = registry.lsmButtons.entities[i];

				// fetch the cursor focus - there is a max of 1 cursor focus in the container
				// so the correct cursor focus is always at index 0
				auto& motion = registry.motions.get(button);
				float xpos = mouse_position.x / aspect_ratio_scale.x;
				float ypos = mouse_position.y / aspect_ratio_scale.y;

				// printf("Mouse XPOS: %f\n", xpos);
				// printf("Mouse YPOS: %f\n", ypos);


				if (i == 0 && motion.position.y <= 200) {

				}


				if (i == 0) {
					mouse_change = ypos - selector_mouse_y;
					selector_mouse_y = ypos;

					if (motion.position.y >= 220 && mouse_change > 0) {
						mouse_change = 0;
					}

					if (motion.position.y <= -200 && mouse_change < 0) {
						mouse_change = 0;
					}
				}

				motion.position.y += mouse_change;

			}

			// Move little guy as well
			Entity little_guy = registry.lsmLittleGuy.entities[0];
			Motion& little_guy_motion = registry.motions.get(little_guy);
			little_guy_motion.position.y += mouse_change;

		}
	}
}

void RenderSystem::lsm_mouse_click_handler(GLFWwindow* window, int key, int action, int mod) {
	// check if mouse button left clicked
	if (key == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			// LSM SELECTOR HANDLER
			lsm_selector_mouse_click_handler(window, key, action, mod);


			// fetch mouse cursor position
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);


			// loop over all the start buttons
			auto& lsm_start_button_container = registry.lsmStartButtons;
			for (int i = 0; i < lsm_start_button_container.size(); i++) {
				// get current button motion info
				Entity button = lsm_start_button_container.entities[i];
				Motion& motion = registry.motions.get(button);
				float button_xpos = motion.position.x * aspect_ratio_scale.x;
				float button_ypos = motion.position.y * aspect_ratio_scale.y;
				float button_width = abs(motion.scale.x) * aspect_ratio_scale.x;
				float button_height = abs(motion.scale.y) * aspect_ratio_scale.y;

				// find button's bounding box
				float x_left_bound = button_xpos - (button_width / 2);
				float x_right_bound = button_xpos + (button_width / 2);
				float y_up_bound = button_ypos - (button_height / 2);
				float y_down_bound = button_ypos + (button_height / 2);

				// if the card is currently a ui element then have to handle that case...
				if (!registry.uiComponents.has(button)) {
					x_left_bound += cameraPosition.x;
					x_right_bound += cameraPosition.x;
					y_up_bound += cameraPosition.y;
					y_down_bound += cameraPosition.y;
				}

				// check to see if mouse cursor is within the card's bounding box
				if ((x_left_bound <= xpos) && (xpos <= x_right_bound) && (y_up_bound <= ypos) && (ypos <= y_down_bound)) {
					// Start the corresponding level!
					std::cout << "Button: " << i << " pressed" << std::endl;
					level_select_system.confirm_selection(this);

				}
			}

			if (!selector_clicked) return; // The selector wasn't clicked, don't register the level select buttons.

			// loop over all level_select buttons
			auto& lsm_button_container = registry.lsmButtons;
			for (int i = 0; i < lsm_button_container.size(); i++) {
				// get current button motion info
				Entity button = lsm_button_container.entities[i];
				Motion& motion = registry.motions.get(button);
				float button_xpos = motion.position.x * aspect_ratio_scale.x;
				float button_ypos = motion.position.y * aspect_ratio_scale.y;
				float button_width = abs(motion.scale.x) * aspect_ratio_scale.x;
				float button_height = abs(motion.scale.y) * aspect_ratio_scale.y;

				// find button's bounding box
				float x_left_bound = button_xpos - (button_width / 2);
				float x_right_bound = button_xpos + (button_width / 2);
				float y_up_bound = button_ypos - (button_height / 2);
				float y_down_bound = button_ypos + (button_height / 2);

				// if the card is currently a ui element then have to handle that case...
				if (!registry.uiComponents.has(button)) {
					x_left_bound += cameraPosition.x;
					x_right_bound += cameraPosition.x;
					y_up_bound += cameraPosition.y;
					y_down_bound += cameraPosition.y;
				}

				// check to see if mouse cursor is within the bounding box
				if ((x_left_bound <= xpos) && (xpos <= x_right_bound) && (y_up_bound <= ypos) && (ypos <= y_down_bound)) {
					// Start the corresponding level's popup window!
					std::cout << "Button: " << i << " pressed" << std::endl;
					level_select_system.handle_level_selection(this, i);

				}
			}
		}
		else if (action == GLFW_RELEASE) {
			selector_clicked = false;
			selector_mouse_y = 0;
		}
	}
}

void RenderSystem::lsm_selector_mouse_click_handler(GLFWwindow* window, int key, int action, int mod) {
	// If the click was inside the selector, set selector_clicked = true;
	// set the cursor focus to the selector

	// fetch mouse cursor position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	auto& lsm_selector_container = registry.lsmSelectors;
	for (int i = 0; i < lsm_selector_container.size(); i++) {
		// get current button motion info
		Entity selector = lsm_selector_container.entities[i];
		Motion& motion = registry.motions.get(selector);
		float selector_xpos = motion.position.x * aspect_ratio_scale.x;
		float selector_ypos = motion.position.y * aspect_ratio_scale.y;
		float selector_width = abs(motion.scale.x) * aspect_ratio_scale.x;
		float selector_height = abs(motion.scale.y) * aspect_ratio_scale.y;

		// find button's bounding box
		float x_left_bound = selector_xpos - (selector_width / 2);
		float x_right_bound = selector_xpos + (selector_width / 2);
		float y_up_bound = selector_ypos - (selector_height / 2);
		float y_down_bound = selector_xpos + (selector_height / 1.5);

		// if the card is currently a ui element then have to handle that case...
		//if (!registry.uiComponents.has(selector)) {
		//	x_left_bound += cameraPosition.x;
		//	x_right_bound += cameraPosition.x;
		//	y_up_bound += cameraPosition.y;
		//	y_down_bound += cameraPosition.y;
		//}

		// check to see if mouse cursor is within the bounding box
		if ((x_left_bound <= xpos) && (xpos <= x_right_bound) && (y_up_bound <= ypos) && (ypos <= y_down_bound)) {
			selector_clicked = true;
			selector_mouse_y = ypos / aspect_ratio_scale.y;; // for tracking drag


			// max 1 cursor focus at a time
			registry.cursorFocus.clear();
			// if it is, adds a cursor focus component to it to signify that the cursor is currently clicking it
			registry.cursorFocus.emplace(selector);

			// remove from 2d world
			registry.uiComponents.remove(selector);
		}
	}
}

void RenderSystem::map_view_mouse_click_handler(GLFWwindow* window, int key, int action, int mod) {
	if (key == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			// fetch mouse cursor position
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			// loop over all level_select buttons
			auto& map_view_button_container = registry.fullMapViewButton;
			for (int i = 0; i < map_view_button_container.size(); i++) {
				// get current button motion info
				Entity button = map_view_button_container.entities[i];
				Motion& motion = registry.motions.get(button);
				float button_xpos = motion.position.x * aspect_ratio_scale.x;
				float button_ypos = motion.position.y * aspect_ratio_scale.y;
				float button_width = abs(motion.scale.x) * aspect_ratio_scale.x;
				float button_height = abs(motion.scale.y) * aspect_ratio_scale.y;

				// find button's bounding box
				float x_left_bound = button_xpos - (button_width / 2);
				float x_right_bound = button_xpos + (button_width / 2);
				float y_up_bound = button_ypos - (button_height / 2);
				float y_down_bound = button_ypos + (button_height / 2);

				// if the card is currently a ui element then have to handle that case...
				if (!registry.uiComponents.has(button)) {
					x_left_bound += cameraPosition.x;
					x_right_bound += cameraPosition.x;
					y_up_bound += cameraPosition.y;
					y_down_bound += cameraPosition.y;
				}

				// check to see if mouse cursor is within the bounding box
				if ((x_left_bound <= xpos) && (xpos <= x_right_bound) && (y_up_bound <= ypos) && (ypos <= y_down_bound)) {
					// Start the corresponding level's popup window!
					std::cout << "Button: " << i << " pressed" << std::endl;
					map_system.handle_full_map_view_button(cameraPosition);

				}
			}
		}
	}
}

void RenderSystem::menu_mouse_click_handler(GLFWwindow* window, int key, int action, int mod) {
	// check if mouse button left clicked
	if (key == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {

			// fetch mouse cursor position
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			if (menu_opened) {
				// menu is opened, click a button:
				for (Entity button : registry.menuButtons.entities) {
					Motion& motion = registry.motions.get(button);

					float button_xpos = motion.position.x * aspect_ratio_scale.x;
					float button_ypos = motion.position.y * aspect_ratio_scale.y;
					float button_width = abs(motion.scale.x) * aspect_ratio_scale.x;
					float button_height = abs(motion.scale.y) * aspect_ratio_scale.y;

					// find button's bounding box
					float x_left_bound = button_xpos - (button_width / 2);
					float x_right_bound = button_xpos + (button_width / 2);
					float y_up_bound = button_ypos - (button_height / 2);
					float y_down_bound = button_ypos + (button_height / 2);


					// check to see if mouse cursor is within the bounding box
					if ((x_left_bound <= xpos) && (xpos <= x_right_bound) && (y_up_bound <= ypos) && (ypos <= y_down_bound)) {
						menu_system.button_click_handler(button);
					}
				}

				// also check the settings buttons
				for (Entity button : registry.settingsButtons.entities) {
					Motion& motion = registry.motions.get(button);

					float button_xpos = motion.position.x * aspect_ratio_scale.x;
					float button_ypos = motion.position.y * aspect_ratio_scale.y;
					float button_width = abs(motion.scale.x) * aspect_ratio_scale.x;
					float button_height = abs(motion.scale.y) * aspect_ratio_scale.y;

					// find button's bounding box
					float x_left_bound = button_xpos - (button_width / 2);
					float x_right_bound = button_xpos + (button_width / 2);
					float y_up_bound = button_ypos - (button_height / 2);
					float y_down_bound = button_ypos + (button_height / 2);

					// check to see if mouse cursor is within the bounding box
					if ((x_left_bound <= xpos) && (xpos <= x_right_bound) && (y_up_bound <= ypos) && (ypos <= y_down_bound)) {
						menu_system.button_click_handler(button);
					}
				}


			} else {
				// Click open menu button
				for (Entity menu_button : registry.openMenuButton.entities) {
					Motion& motion = registry.motions.get(menu_button);

					float button_xpos = motion.position.x * aspect_ratio_scale.x;
					float button_ypos = motion.position.y * aspect_ratio_scale.y;
					float button_width = abs(motion.scale.x) * aspect_ratio_scale.x;
					float button_height = abs(motion.scale.y) * aspect_ratio_scale.y;

					// find button's bounding box
					float x_left_bound = button_xpos - (button_width / 2);
					float x_right_bound = button_xpos + (button_width / 2);
					float y_up_bound = button_ypos - (button_height / 2);
					float y_down_bound = button_ypos + (button_height / 2);

					// check to see if mouse cursor is within the bounding box
					if ((x_left_bound <= xpos) && (xpos <= x_right_bound) && (y_up_bound <= ypos) && (ypos <= y_down_bound)) {
						// toggle the menu
						menu_system.toggle_menu();

					}
				}

			}
		}
		else if (action == GLFW_RELEASE) {
			is_icon_pressed = false;
		}
			
	}

}

vec2 get_camera_matrix() {
	return cameraPosition;
}

void reset_camera_position() {
	cameraPosition = { 0.0, 0.0, 1.0 };
}

void update_camera_position(float xpos, float ypos) {
	cameraPosition = { xpos, ypos, 1.0 };
}