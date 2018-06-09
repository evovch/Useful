#include "cls_planar_patch.h"

// STD
#include <ctime>

// Project
#include "base/cls_logger.h"
#include "graphics/cls_model.h"

#include "algo/cls_quick_hull.h"

cls_planar_patch::cls_planar_patch()
{
	std::srand(std::time(nullptr));
}

cls_planar_patch::~cls_planar_patch()
{
}

void cls_planar_patch::BuildModel(cls_model* p_model)
{
	unsigned int v_nVertices = 200;

	// Allocate memory
	float* v_vertices = new float[v_nVertices*3]; // each vertex is 3 cordinates, so *3

	// Fill
	for (unsigned int iVertex = 0; iVertex < v_nVertices; iVertex++)
	{
		v_vertices[iVertex*3+0] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		v_vertices[iVertex*3+1] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		v_vertices[iVertex*3+2] = 0.;
	}

	// Array of indices
	// Full input data set
	unsigned int* v_set = new unsigned int[v_nVertices];
	unsigned int v_setSize = v_nVertices;
	for (unsigned int iVertex = 0; iVertex < v_setSize; iVertex++) {
		v_set[iVertex] = iVertex;
	}

	// ============================================================================================
	// Build outer boundary

	std::list<unsigned int> v_convex =
		cls_quick_hull::QuickHull(v_vertices, v_set, v_setSize, p_model);

	delete [] v_set;

	// ============================================================================================

	// Draw =======================================================================================
	unsigned int v_nWires = v_convex.size();
	LOG(INFO) << "Built convex hull contains " << v_nWires << " points." << cls_logger::endl;

	// Vertices and points ------------------------------------------------------------------------
	float* v_hullVertices = new float[(v_nWires+1)*3]; // each vertex is 3 cordinates, so *3
	unsigned int iVertex = 0;
	for (std::list<unsigned int>::iterator iter = v_convex.begin(); iter != v_convex.end(); ++iter)
	{
		v_hullVertices[iVertex*3+0] = v_vertices[(*iter)*3+0];
		v_hullVertices[iVertex*3+1] = v_vertices[(*iter)*3+1];
		v_hullVertices[iVertex*3+2] = v_vertices[(*iter)*3+2];

		// Mean point
		v_hullVertices[v_nWires*3+0] += v_hullVertices[iVertex*3+0];
		v_hullVertices[v_nWires*3+1] += v_hullVertices[iVertex*3+1];
		v_hullVertices[v_nWires*3+2] += v_hullVertices[iVertex*3+2];

		iVertex++;
	}

	// Mean point
	v_hullVertices[v_nWires*3+0] /= v_nWires;
	v_hullVertices[v_nWires*3+1] /= v_nWires;
	v_hullVertices[v_nWires*3+2] /= v_nWires;

	p_model->AppendPoints(v_nWires+1, v_hullVertices);
	delete [] v_hullVertices;
	// --------------------------------------------------------------------------------------------

	// Hull wires ---------------------------------------------------------------------------------
	unsigned int* v_hullWires = new unsigned int[(v_nWires)*2];
	for (unsigned int i=0; i<v_nWires; i++) {
		v_hullWires[i*2+0] = i;
		v_hullWires[i*2+1] = i+1;
	}
	v_hullWires[(v_nWires-1)*2+1] = 0;

	p_model->AppendWires(v_nWires, v_hullWires);
	delete [] v_hullWires;

	// Radial wires ---------------------------------------------------------------------------------
	unsigned int* v_radialWires = new unsigned int[(v_nWires)*2];
	for (unsigned int i=0; i<v_nWires; i++) {
		v_radialWires[i*2+0] = i;
		v_radialWires[i*2+1] = v_nWires;
	}

	p_model->AppendWires(v_nWires, v_radialWires);
	delete [] v_radialWires;

	// --------------------------------------------------------------------------------------------

	// Triangles ----------------------------------------------------------------------------------
	unsigned int* v_hullTriags = new unsigned int[v_nWires*3];

	for (unsigned int i=0; i<v_nWires; i++) {
		v_hullTriags[i*3+0] = v_nWires;
		v_hullTriags[i*3+1] = i+1;
		v_hullTriags[i*3+2] = i;
	}
	v_hullTriags[(v_nWires-1)*3+1] = 0;

	p_model->AppendTriangles(v_nWires, v_hullTriags);
	delete [] v_hullTriags;
	// --------------------------------------------------------------------------------------------


	// Wires --------------------------------------------------------------------------------------
/*	unsigned int* v_wires = new unsigned int[v_nWires*2];
	unsigned int iWire = 0;

	std::list<unsigned int>::iterator iter = v_convex.begin();
	v_wires[iWire*2+0] = *iter;
	++iter;
	for (; iter != v_convex.end(); ++iter) {
		v_wires[iWire*2+1] = *iter;
		iWire++;
		v_wires[iWire*2+0] = *iter;
	}
	v_wires[iWire*2+1] = v_wires[0*2+0];

	p_model->AppendWires(v_nWires, v_wires);
	delete [] v_wires;*/
	// --------------------------------------------------------------------------------------------

	// ============================================================================================

	// Send ALL vertices, not only hull vertices
	p_model->AppendPoints(v_nVertices, v_vertices);
	//p_model->SetConstructed();

	delete [] v_vertices;

	p_model->SetConstructed();
}
