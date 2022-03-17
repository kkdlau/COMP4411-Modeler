#ifndef ORGANICHEAD_H
#define ORGANICHEAD_H

#include "MarchingCube.h"
#include "modelerglobals.h"
#include <vector>

class OrganicHead {
public:
	static enum OrganicShapes {
		ELLIPSOID = 0,
		HYPERBOLOID
	};
	std::vector<MarchingCube> cubes;
	double threshold = 1; // x^2 + y^2 + z^2 = 1 (the 1 here)
	float length, height, width, gridsize;
	OrganicShapes shape;

	/*
	Initialize Grid based on length (x), height (y), constant z
	*/
	OrganicHead(float l, float h, float w, float gs, double t, OrganicShapes os);

	/*
	Return the isolevel for a point based on the shape
	*/
	double getIsoLevel(Vec3f p);
	/*
	Initialize the grids 
	*/
	void initializeCubes();

	void draw();
};

#endif