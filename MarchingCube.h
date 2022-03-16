#ifndef MARCHINGCUBE_H
#define MARCHINGCUBE_H

#include "vec.h"

typedef struct {
	Vec3f p[8];
	double val[8];
} GRIDCELL;

class MarchingCube {
public:
	GRIDCELL grid;
	MarchingCube(GRIDCELL g);
	/*
   Given a grid cell and an isolevel (threshold), calculate the triangular
   facets required to represent the isosurface through the cell.
   the resulting triangle facets are drawn immediately
	*/
	void Polygonise(double isolevel);
	Vec3f VertexInterp(double isolevel, Vec3f p1, Vec3f p2, double valp1, double valp2);

	static int edgeTable[];
	static int triTable[][16];
};
#endif