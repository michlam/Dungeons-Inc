#pragma once

///////////////
//  imports  //
/////////////// 
// basics
#include "common.hpp"
#include "tiny_ecs.hpp"
#include "tiny_ecs_registry.hpp"
#include "components.hpp"
#include <render_system.hpp>

///////////////
//   class   //
/////////////// 
// A simple physics system that moves rigid bodies and checks for collision
class PhysicsSystem
{
public:
	PhysicsSystem(RenderSystem* rs) {
		renderer = rs;
	}


	void step(float elapsed_ms);

	bool collides(Entity motion1, Entity motion2);

	// static bool mesh_collides_y(Mesh& mesh, Entity entity, float boundary_y_pos); //no longer used
	static bool mesh_collides_ground(Mesh& mesh, Entity entity1, Entity entity2);

private:

	RenderSystem* renderer;

};


///////////////
// functions //
/////////////// 
bool hovers(const Motion& motion, vec2 point);

std::vector<vec2> get_bounding_box_points(const Motion& motion);

bool vertex_collides_box(const vec2 point, const std::vector<vec2>& box_points);

bool edge_collides_box(const vec2 point1, const vec2 point2, const std::vector<vec2>& box_points);

bool edge_intersects_edge(const vec2 p1, const vec2 q1, const vec2 p2, const vec2 q2);

bool on_segment(const vec2 p, const vec2 q, const vec2 r);

int orientation(const vec2 p, const vec2 q, const vec2 r);