#pragma once
#include"PhysStructs.h"
#include<math.h>
#include<iostream>


/*
	Sets the center of a physics object to the average of all its points.
*/
void setObjCenter(PhysObj *obj) {
	float xSum=0, ySum=0;
	int size = obj->verts.size();
	for (int i = 0; i < size; i+=2) {
		xSum += obj->verts[i];
		ySum += obj->verts[i + 1];
	}
	obj->center = Vec2d{ xSum / (float)size, ySum / (float)size };
}

/*
	Calculates and returns dot product of two vectors
*/
float dotProd(Vec2d vec1, Vec2d vec2) {
	return vec1.x*vec2.x + vec1.y*vec2.y;
}

/*
	Update the world position vertices of an object. 
*/
void updateWorldVerts(PhysObj *obj) {
	obj->worldVerts.clear();

	float angle = (int)(obj->rotation)*3.14159265 / 180;//convert from rads to degrees

	for (int i = 0; i < obj->verts.size(); i+=2) {//count by two since they are stored individually x and then y
		
		//---------rotate verts-----------
		float x = obj->verts[i] * cos(angle) - obj->verts[i + 1] * sin(angle);
		float y = obj->verts[i + 1] * cos(angle) + obj->verts[i] * sin(angle);
		//--------Translate verts--------
		x += obj->position.x;
		y += obj->position.y;
		obj->worldVerts.push_back(x);
		obj->worldVerts.push_back(y);
	}
}



