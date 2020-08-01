#pragma once
#include<vector>

struct Vec2d {
	float x;
	float y;
};

struct PhysObj {
	float rotation;
	float rotationalVel;
	float mass;

	Vec2d position;
	Vec2d center;
	Vec2d velocity;

	std::vector<float> verts;
	std::vector<float> worldVerts;
};