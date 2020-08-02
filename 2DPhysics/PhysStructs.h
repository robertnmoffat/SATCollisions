#pragma once
#include<vector>

/*
	Simple struct to represent a 2 dimensional vector
*/
struct Vec2d {
	float x;
	float y;
};

/*
	Struct representing a physical object to be drawn and collided
*/
struct PhysObj {
	float rotation;

	Vec2d position;
	Vec2d center;

	std::vector<float> verts;
	std::vector<float> worldVerts;
};