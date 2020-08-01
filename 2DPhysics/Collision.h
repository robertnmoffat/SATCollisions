#pragma once
#include"PhysFuncs.h"
#include<vector>
#include<algorithm>


namespace Collision {
	/*
		create a vector to the current vertex, project it onto the normal, and then check if it is the new minimum or maximum
	*/
	void minMaxVertices(float *min, float *max, Vec2d *norm, PhysObj *obj, Vec2d *position, int vertPos1, int vertPos2, bool first, float bMagnitudeSquared) {
		//dot product each with norm, divided by magnitude of norm. compare with the left and right extremes, if further replace it

		Vec2d vec = Vec2d{ obj->worldVerts[vertPos1] - (*position).x, obj->worldVerts[vertPos2] - (*position).y };//convert point to a vector from center point
		float abDot = dotProd(vec, *norm);//dot product with norm to get its position on the norm line
		float projection = abDot / bMagnitudeSquared;


		//check if ob1min is less than ob2max and that ob1max is greater than ob2min
		if (first) {
			*min = projection;
			*max = projection;
		}
		else if (projection < *min) {
			*min = projection;
		}
		else if (projection > *max) {
			*max = projection;
		}
	}

	/*
		Project vertices on to the normal vector
		find the min/max vertex extremes for each object on the normal vector
		check if there is overlap within the extremes
	*/
	bool projectionOverlap(PhysObj *ob1, PhysObj *ob2, int vertposx1, int vertposx2, int vertposy1, int vertposy2) {
		Vec2d vec = Vec2d{ ob1->worldVerts[vertposx2] - ob1->worldVerts[vertposx1], ob1->worldVerts[vertposy2] - ob1->worldVerts[vertposy1] };
		Vec2d norm = Vec2d{ -vec.y, vec.x };

		float normMag = sqrtf(norm.x*norm.x+norm.y*norm.y);

		float ob1min, ob1max, ob2min, ob2max;

		for (int j = 0; j < ob2->worldVerts.size(); j += 2) {//Loop through each vertex
			minMaxVertices(&ob2min, &ob2max, &norm, ob2, &(ob1->position), j, j + 1, j == 0, normMag);
		}
		for (int j = 0; j < ob1->worldVerts.size(); j += 2) {//Loop through each vertex
			minMaxVertices(&ob1min, &ob1max, &norm, ob1, &(ob1->position), j, j + 1, j == 0, normMag);
		}

		//if any are not colliding return false. Ends checks early when they are not colliding, which is most of the time
		return (ob1max > ob2min&&ob1min < ob2max);
	}


	/*
		Calculate SAT(separates axis theorem) collision detection on two physics objecs
		calculate for each side of first polygon, then each side of second polygon
	*/
	bool SATCollision(PhysObj ob1, PhysObj ob2) {
		//second x minus first x, second y minus first y
		for (int i = 0; i < ob1.worldVerts.size(); i += 2) {//loop through each side of polygon
			if (!projectionOverlap(&ob1, &ob2, i, (i + 2) % ob1.worldVerts.size(), (i + 1) % ob1.worldVerts.size(), (i + 3) % ob1.worldVerts.size()))
				return false;
		}

		for (int i = 0; i < ob2.worldVerts.size(); i += 2) {//loop through each side of polygon
			if (!projectionOverlap(&ob2, &ob1, i, (i + 2) % ob2.worldVerts.size(), (i + 1) % ob2.worldVerts.size(), (i + 3) % ob2.worldVerts.size()))
				return false;
		}


		return true;
	}

}