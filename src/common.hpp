#pragma once

///////////////
//  imports  //
/////////////// 
#define GLFW_EXPOSE_NATIVE_WIN32

// stlib
#include <fstream> // stdout, stderr..
#include <string>
#include <tuple>
#include <vector>
#include <windows.h>

// glfw (OpenGL)
#define NOMINMAX
#include <gl3w.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// The glm library provides vector and matrix operations as in GLSL
#include <glm/vec2.hpp>				// vec2
#include <glm/ext/vector_int2.hpp>  // ivec2
#include <glm/vec3.hpp>             // vec3
#include <glm/mat3x3.hpp>           // mat3
using namespace glm;

// text rendering / freetype
#include <ft2build.h>
#include FT_FREETYPE_H  

// other
#include "tiny_ecs.hpp"


// Additional imports
#include <iostream>

///////////////
// constants //
///////////////
// Simple utility functions to avoid mistyping directory name
// audio_path("audio.ogg") -> data/audio/audio.ogg
// Get defintion of PROJECT_SOURCE_DIR from:
#include "../ext/project_path.hpp"
inline std::string data_path() { return std::string(PROJECT_SOURCE_DIR) + "data"; };
inline std::string shader_path(const std::string& name) { return std::string(PROJECT_SOURCE_DIR) + "/shaders/" + name; };
inline std::string textures_path(const std::string& name) { return data_path() + "/textures/" + std::string(name); };
inline std::string audio_path(const std::string& name) { return data_path() + "/audio/" + std::string(name); };
inline std::string mesh_path(const std::string& name) { return data_path() + "/meshes/" + std::string(name); };
inline std::string font_path(const std::string& name) { return data_path() + "/fonts/" + std::string(name); };

// screen dimensions of the user's primary monitor
const DPI_AWARENESS_CONTEXT dpi = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
const int screen_width = GetSystemMetrics(SM_CXSCREEN);
const int screen_height = GetSystemMetrics(SM_CYSCREEN);

const vec2 default_screen_resolution = { 1920.f, 1080.f };
const vec2 default_window_size = { 1200.f, 800.f };
const vec2 current_screen_resolution = { static_cast<float>(screen_width), static_cast<float>(screen_height) };

const vec2 aspect_ratio_scale = current_screen_resolution / default_window_size;

// for fullscreen
const int window_width_px = current_screen_resolution.x;
const int window_height_px = current_screen_resolution.y;

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


///////////////
//  structs  //
/////////////// 
// The 'Transform' component handles transformations passed to the Vertex shader
// (similar to the gl Immediate mode equivalent, e.g., glTranslate()...)
// We recomment making all components non-copyable by derving from ComponentNonCopyable
struct Transform {
	mat3 mat = { { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f}, { 0.f, 0.f, 1.f} }; // start with the identity
	void scale(vec2 scale);
	void rotate(float radians);
	void translate(vec2 offset);
};

bool gl_has_errors();
