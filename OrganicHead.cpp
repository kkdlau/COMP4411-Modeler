#include "OrganicHead.h"
#include <math.h>
#include <algorithm>

OrganicHead::OrganicHead(float l, float h, float w, float gs, double t, OrganicShapes os) 
:length(l), height(h), width(w), gridsize(gs), threshold(t), shape(os)
{
	initializeCubes();
}


double OrganicHead::getIsoLevel(Vec3f p) {
	float radius[3]{ length / 2, height / 2, width / 2 };
	switch (shape) {
	case ELLIPSOID: {
		// the ellipsoid function : 1/x_rad x^2 + 1/y_rad y^2 + 1/z_rad z^2 = 1
		double coef[3] = { 1 / pow(radius[0], 2), 1 / pow(radius[1], 2), 1 / pow(radius[2], 2) };
		double isoLevel = 0;
		for (int i = 0; i < 3; ++i) 
			isoLevel += (coef[i] * pow(p[i] - radius[i], 2));
		return isoLevel;
	}
	case HYPERBOLOID: {
		// the hyperboloid function: 1/x_rad x^2 + 1/y_rad y^2 - 1/z_rad z^2 = 1
		double coef[3] = { 1 / (radius[0] * radius[0]), 1 / (radius[1] * radius[1]), - 1 / (radius[2] * radius[2]) };
		double isoLevel = 0;
		for (int i = 0; i < 3; ++i)
			isoLevel += (coef[i] * pow(p[i] - radius[i], 2));
		return isoLevel;
	}
	}
}

void OrganicHead::initializeCubes() {
	// GRIDCELL has p (Vec3f) for all the points, and val (double) for all the isovalues
	for (float x = 0; x < length; x += gridsize) {
		for (float y = 0; y < height; y += gridsize) {
			for (float z = 0; z < width; z += gridsize) {
				GRIDCELL g;
				g.p[0] = { x, y, z };
				g.p[1] = { x + gridsize, y, z };
				g.p[2] = { x + gridsize, y + gridsize, z };
				g.p[3] = { x, y + gridsize, z };
				g.p[4] = { x, y, z + gridsize };
				g.p[5] = { x + gridsize, y, z + gridsize };
				g.p[6] = { x + gridsize, y + gridsize, z + gridsize };
				g.p[7] = { x, y + gridsize, z + gridsize };
				for (int index = 0; index < 8; ++index) {
					g.val[index] = getIsoLevel(g.p[index]);
				}
				MarchingCube cube{ g };
				cubes.push_back(cube);
			}
		}
	}
}

void OrganicHead::draw() {
	// call polygonize for all the cubes in me
	std::for_each(cubes.begin(), cubes.end(), [&](MarchingCube cube) {
		cube.Polygonise(threshold);
		});
}