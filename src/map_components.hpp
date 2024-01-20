#pragma once

///////////////
//  imports  //
///////////////
// common
#include "common.hpp"

///////////////
//  structs  //
///////////////
// A list of the entities that are inside a room
struct RoomEntities {
	std::set<Entity> roomEntities;
};

struct Room {
	std::vector<Entity> connections;
};

struct RenderedRoom {
	std::vector<Entity> connections;
};

struct Map {

};

struct CorrespondingRoom {
	Entity room;
};

struct CurrentRoom {
	Room currentRoom;
};