///////////////
//  imports  //
/////////////// 
#include "physics_system.hpp"
#include "world_init.hpp"
#include <phase/phase_system.hpp>

///////////////
// functions //
/////////////// 
// Returns the local bounding coordinates scaled by the current size of the entity
vec2 get_bounding_box(const Motion& motion) {
	// abs is to avoid negative scale due to the facing direction.
	return { abs(motion.scale.x), abs(motion.scale.y) };
}


// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You can
// surely implement a more accurate detection
bool PhysicsSystem::collides(Entity entity1, Entity entity2) {
	vec2 view_matrix = get_camera_matrix();

	// quick check to verify that the TWO ENTITIES ARE NOT THE SAME ENTITY REEEE
	if ((int)entity1 == (int)entity2) {
		return false;
	}

	Motion new_motion_1 = registry.motions.get(entity1);
	Motion new_motion_2 = registry.motions.get(entity2);

	if (registry.uiComponents.has(entity1)) {
		new_motion_1.position = new_motion_1.position - view_matrix / aspect_ratio_scale;
		// new_motion_1.position.y = new_motion_1.position.y - view_matrix.y / aspect_ratio_scale.y;
	}

	if (registry.uiComponents.has(entity2)) {
		new_motion_2.position = new_motion_2.position - view_matrix / aspect_ratio_scale;
		// new_motion_2.position.y = new_motion_2.position.y - view_matrix.y / aspect_ratio_scale.y;
	}

	vec2 dp = new_motion_1.position - new_motion_2.position;
	float dist_squared = dot(dp, dp);

	const vec2 other_bonding_box = get_bounding_box(new_motion_1) / 3.f;
	const float other_r_squared = dot(other_bonding_box, other_bonding_box);
	const vec2 my_bonding_box = get_bounding_box(new_motion_2) / 3.f;
	const float my_r_squared = dot(my_bonding_box, my_bonding_box);
	const float r_squared = max(other_r_squared, my_r_squared);

	if (dist_squared < r_squared) {
		return true;
	}

	return false;
}


// This is a check that puts a circle around the bounding boxes and sees
// if the mouse position is inside the other's bounding-box-circle. 
bool hovers(const Motion& motion, vec2 point) {
	vec2 dp = (point - motion.position);
	float dist_squared = dot(dp, dp);

	const vec2 bounding_box = get_bounding_box(motion) / 2.f;
	const float r_squared = dot(bounding_box, bounding_box);

	if (dist_squared < r_squared)
		return true;

	return false;
}


void PhysicsSystem::step(float elapsed_ms) {
	//update movement
	float speed = 150.f;
	auto& movement_container = registry.movements;

	if (!is_paused) {
		for (int i = movement_container.components.size() - 1; i >= 0; --i) {
			Entity entity = movement_container.entities[i];

			//update animation type

			vec2 goal_pos = movement_container.components[i].goal_pos;
			Motion& motion = registry.motions.get(entity);
			Animation& animation = registry.animations.get(entity);

			if (length(motion.position - goal_pos) < 5.f) {
				animation.animation_type = 0;

				motion.velocity = vec2{ 0.f, 0.f };
				
				//HARVIR - add next item in movement queue
				if (registry.movement_instructions.has(registry.movements.entities[i])) {

					MovementInstructions& movement_instructions = registry.movement_instructions.get(registry.movements.entities[i]);
					movement_instructions.curr_goal += 1;
					if (movement_instructions.curr_goal < movement_instructions.goal_positions.size()) {
						if (movement_instructions.curr_goal == movement_instructions.goal_positions.size() - 1) {
							registry.movements.components[i].goal_pos = movement_instructions.goal_positions[movement_instructions.curr_goal] + movement_instructions.char_offset;
						}
						else {
							registry.movements.components[i].goal_pos = movement_instructions.goal_positions[movement_instructions.curr_goal];
						}
					}
					else {
						registry.movement_instructions.remove(entity);
						movement_container.remove(entity);
					}

				}
			}
			else if (goal_pos != vec2({ -1.f, -1.f })) {
				
				animation.animation_type = 1;

				float prev_dir = motion.scale.x;

				motion.velocity = normalize(goal_pos - motion.position) * speed;

				if (prev_dir * motion.velocity.x < 0) {
					motion.scale.x *= -1;
				}

				// if moving entity is a character, reposition weapon too
				if (registry.characters.has(entity)) {
					Entity& weapon = registry.characters.get(entity).weapon;

					if (registry.weapons.has(weapon)) {
						Motion& weapon_motion = registry.motions.get(weapon);

						if (motion.scale.x > 0) {
							weapon_motion.angle = M_PI / 8;
						}
						else {
							weapon_motion.angle = -M_PI / 8;
						}
						if (registry.adventurerClass.get(entity).advClass == Class::RANGER) {
							weapon_motion.scale.x = motion.scale.x;
						}
						else {
							weapon_motion.scale.x = motion.scale.x / 3 * 2;

						}

						float weapon_offset_x = motion.scale.x / 3;
						float weapon_offset_y = motion.scale.y / 4;

						weapon_motion.position = vec2{ motion.position.x + weapon_offset_x,  motion.position.y + weapon_offset_y };

						//weapon_motion.position = motion.position;
					}
				}
			}
		}

		// move entities with a nonzero velocity based on how much time has past
		// this is to (partially) avoid having entities move at different speed based on the machine
		auto& motion_container = registry.motions;
		for (uint i = 0; i < motion_container.size(); i++) {
			Motion& motion = motion_container.components[i];
			Entity entity = motion_container.entities[i];
			float step_seconds = elapsed_ms / 1000.f;

			motion.position += (motion.velocity * step_seconds);

			// handle cutscene effect movements
			if (registry.cutscene_effects.has(entity)) {
				CutsceneEffect effect = registry.cutscene_effects.get(entity);
				if (registry.motions.has(effect.entity)) {
					Motion effect_entity_motion = registry.motions.get(effect.entity);

					motion.position += (effect_entity_motion.velocity * step_seconds);
				}
			}
		}

		// Check for collisions between all moving entities
		for (uint i = 0; i < motion_container.components.size(); i++) {
			Motion& motion_i = motion_container.components[i];
			Entity entity_i = motion_container.entities[i];

			// note starting j at i+1 to compare all (i,j) pairs only once (and to not compare with itself)
			for (uint j = i + 1; j < motion_container.components.size(); j++) {
				Motion& motion_j = motion_container.components[j];
				Entity entity_j = motion_container.entities[j];

				if (collides(entity_i, entity_j)) {
					Entity entity_j = motion_container.entities[j];
					// Create a collisions event
					// We are abusing the ECS system a bit in that we potentially insert muliple collisions for the same entity
					registry.collisions.emplace_with_duplicates(entity_i, entity_j);
					registry.collisions.emplace_with_duplicates(entity_j, entity_i);
				}
			}
		}
	}
}

/*
bool PhysicsSystem::mesh_collides_y(Mesh& mesh, Entity entity, float boundary_y_pos) {

	Motion& motion = registry.motions.get(entity);

	Transform transform;
	transform.translate(motion.position);
	transform.rotate(motion.angle);
	transform.scale(motion.scale);

	assert(registry.renderRequests.has(entity));
	const RenderRequest& render_request = registry.renderRequests.get(entity);


	//old
	for (const ColoredVertex& vertex : mesh.vertices) {
		// Transform the vertex position
		vec3 transformed_pos = transform.mat * vec3(vertex.position.x, vertex.position.y, 1.0);

		//check if y coord passes boundary
		if (transformed_pos.y > boundary_y_pos) {
			return true;
		}
	}
	return false;
}
*/


bool PhysicsSystem::mesh_collides_ground(Mesh& mesh, Entity entity1, Entity entity2) {

	Motion& motion1 = registry.motions.get(entity1);

	Transform transform;
	transform.translate(motion1.position);
	transform.rotate(motion1.angle);
	transform.scale(motion1.scale);

	assert(registry.renderRequests.has(entity1));

	Motion& motion2 = registry.motions.get(entity2);
	assert(registry.renderRequests.has(entity2));

	std::vector<vec2> box_points = get_bounding_box_points(motion2);



	// iterate through triangles (3 vertices per triangle)
	for (int i = 0; i < mesh.vertex_indices.size(); i += 3) {

		
		//vertex-edge check
		for (int j = 0; j < 3; j++) {
			// Get index of vertex in the triangle
			int vertexIndex = mesh.vertex_indices[i + j];
			const ColoredVertex& vertex = mesh.vertices[vertexIndex];

			vec3 transformed_pos = transform.mat * vec3(vertex.position.x, vertex.position.y, 1.0);

			// Check if the transformed vertex is inside the box
			if (vertex_collides_box(vec2(transformed_pos.x, transformed_pos.y), box_points)) {
				return true; 
			}
		}
		
		

		// edge-edge-edge check (for each edge in triangle)
		for (int j = 0; j < 3; j++) {

			//printf("%d", j);
			int vertex_index1 = mesh.vertex_indices[i + j];
			int vertex_index2 = mesh.vertex_indices[i + (j + 1) % 3];

			const ColoredVertex& vertex1 = mesh.vertices[vertex_index1];
			const ColoredVertex& vertex2 = mesh.vertices[vertex_index2];

			vec3 transformed_pos1 = transform.mat * vec3(vertex1.position.x, vertex1.position.y, 1.0);
			vec3 transformed_pos2 = transform.mat * vec3(vertex2.position.x, vertex2.position.y, 1.0);

			// Check for intersection with the box along the edge
			if (edge_collides_box(vec2(transformed_pos1.x, transformed_pos1.y),
				vec2(transformed_pos2.x, transformed_pos2.y), box_points)) {
				return true; 
			}
		}
		
		
	}

	return false; // No collision detected
}

std::vector<vec2> get_bounding_box_points(const Motion& motion) {
	
	std::vector<vec2> box_points;

	float half_width = motion.scale.x / 2.f;
    float half_height = motion.scale.y / 2.f;

    // Calculate bounding box corners
    vec2 top_left = {motion.position.x - half_width, motion.position.y - half_height };
    vec2 top_right = { motion.position.x + half_width, motion.position.y - half_height };
	vec2 bottom_left = { motion.position.x - half_width, motion.position.y + half_height };
    vec2 bottom_right = { motion.position.x + half_width, motion.position.y + half_height };

    // Add corners to the vector
	box_points.push_back(top_left);
	box_points.push_back(top_right);
	box_points.push_back(bottom_right);
	box_points.push_back(bottom_left);

    return box_points;
}


//check if a vertex is inside a box
bool vertex_collides_box(const vec2 point, const std::vector<vec2>& box_points) {


	//initialize min and max values
	float x_min = box_points[0].x;
	float x_max = box_points[0].x;

	float y_min = box_points[0].y;
	float y_max = box_points[0].y;

	for (int i = 1; i < box_points.size(); i++) {
		if (box_points[i].x < x_min) {
			x_min = box_points[i].x;
		}
		if (box_points[i].x > x_max) {
			x_max = box_points[i].x;
		}
		if (box_points[i].y < y_min) {
			y_min = box_points[i].y;
		}
		if (box_points[i].y > y_max) {
			y_max = box_points[i].y;
		}
	}

	// check if point lies inside of box bounds
	if (point.x > x_min && point.x < x_max && point.y > y_min && point.y < y_max) {
		return true;
	}

	return false;
}



// check if an edge intersects with the box
bool edge_collides_box(const vec2 point1, const vec2 point2, const std::vector<vec2>& box_points) {

	// Check if the edge intersects any of the box edges
	for (int i = 0; i < box_points.size(); ++i) {
		int next = (i + 1) % box_points.size();
		if (edge_intersects_edge(point1, point2, box_points[i], box_points[next])) {
			return true;
		}
	}

	return false;
}


// line intersesction check code adapted from
// https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// 
// check if two line segments intersect
bool edge_intersects_edge(const vec2 p1, const vec2 q1, const vec2 p2, const vec2 q2) {
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// general case
	if (o1 != o2 && o3 != o4) {
		return true; 
	}

	// Special cases for colinear points
	if (o1 == 0 && on_segment(p1, p2, q1)) return true;
	if (o2 == 0 && on_segment(p1, q2, q1)) return true;
	if (o3 == 0 && on_segment(p2, p1, q2)) return true;
	if (o4 == 0 && on_segment(p2, q1, q2)) return true;

	return false;
}

//  check if a point lies on a line segment
bool on_segment(const vec2 p, const vec2 q, const vec2 r) {

	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;

	return false;
}

// determine the orientation of three points
int orientation(const vec2 p, const vec2 q, const vec2 r) {
	float val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0.f) return 0;  // collinear 

	return (val > 0.f) ? 1 : 2; // clock or counterclock wise 
}