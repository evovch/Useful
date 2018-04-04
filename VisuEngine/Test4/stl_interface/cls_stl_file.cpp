#include "cls_stl_file.h"

// STD
#include <fstream> // for ofstream
using std::endl;

// Project
#include "cls_model.h"

cls_stl_file::cls_stl_file() :
	mName(""),
	mFirstFacet(nullptr)
{
}

cls_stl_file::~cls_stl_file()
{
}

void cls_stl_file::Export(std::string p_filename) const
{
	//TODO tune output stream such that we do not loose any precision!!!
	std::ofstream f(p_filename);
	f << "solid " << mName << endl;
	struct facet_t* curfacet = mFirstFacet;
	do {
		f << "facet normal " << curfacet->mNormal->mX << " " << curfacet->mNormal->mY << " " << curfacet->mNormal->mZ << endl;
		f << " outer loop" << endl;
		f << "  vertex " << curfacet->mLoop->mV1->mX << " " << curfacet->mLoop->mV1->mY << " " << curfacet->mLoop->mV1->mZ << endl;
		f << "  vertex " << curfacet->mLoop->mV2->mX << " " << curfacet->mLoop->mV2->mY << " " << curfacet->mLoop->mV2->mZ << endl;
		f << "  vertex " << curfacet->mLoop->mV3->mX << " " << curfacet->mLoop->mV3->mY << " " << curfacet->mLoop->mV3->mZ << endl;
		f << " endloop" << endl;
		f << "endfacet" << endl;
	} while ((curfacet = curfacet->GetNext()) != nullptr);
}

void cls_stl_file::BuildModel(cls_model* p_model) const
{
	// Count facets
	unsigned long int v_nFacets = 0;
	struct facet_t* curfacet = mFirstFacet;
	do {
		v_nFacets++;
	} while ((curfacet = curfacet->GetNext()) != nullptr);

	// Compute number of vertices, wires and triangles
	unsigned long int v_nVertices = v_nFacets*3; // each facet is 3 vertices, so *3
	unsigned long int v_nWires = v_nFacets*3; // each facet is 3 wires, so *3
	unsigned long int v_nTriangs = v_nFacets; // one facet = one triangle
	fprintf(stderr, "[INFO] Found %ld facets, thus %ld vertices, %ld wires.\n",
		    v_nFacets, v_nVertices, v_nWires);

	// Allocate memory
	float* v_vertices = new float[v_nVertices*3]; // each vertex is 3 cordinates, so *3
	unsigned int* v_wires = new unsigned int[v_nWires*2]; // each wire is two indices, so *2
	unsigned int* v_triangs = new unsigned int[v_nTriangs*3]; // each triangle is 3 indices, so *3

	// Fill
	unsigned int v_oldNvertices = p_model->GetNumOfVertices();

	unsigned long int iFacet = 0;
	curfacet = mFirstFacet;
	do {
		v_vertices[iFacet*9+0] = curfacet->mLoop->mV1->mX;
		v_vertices[iFacet*9+1] = curfacet->mLoop->mV1->mY;
		v_vertices[iFacet*9+2] = curfacet->mLoop->mV1->mZ;
		v_vertices[iFacet*9+3] = curfacet->mLoop->mV2->mX;
		v_vertices[iFacet*9+4] = curfacet->mLoop->mV2->mY;
		v_vertices[iFacet*9+5] = curfacet->mLoop->mV2->mZ;
		v_vertices[iFacet*9+6] = curfacet->mLoop->mV3->mX;
		v_vertices[iFacet*9+7] = curfacet->mLoop->mV3->mY;
		v_vertices[iFacet*9+8] = curfacet->mLoop->mV3->mZ;

		v_wires[iFacet*6+0] = v_oldNvertices + iFacet*3+0;
		v_wires[iFacet*6+1] = v_oldNvertices + iFacet*3+1;
		v_wires[iFacet*6+2] = v_oldNvertices + iFacet*3+1;
		v_wires[iFacet*6+3] = v_oldNvertices + iFacet*3+2;
		v_wires[iFacet*6+4] = v_oldNvertices + iFacet*3+2;
		v_wires[iFacet*6+5] = v_oldNvertices + iFacet*3+0;

		v_triangs[iFacet*3+0] = v_oldNvertices + iFacet*3+0;
		v_triangs[iFacet*3+1] = v_oldNvertices + iFacet*3+1;
		v_triangs[iFacet*3+2] = v_oldNvertices + iFacet*3+2;

		iFacet++;
	} while ((curfacet = curfacet->GetNext()) != nullptr);

	// Apply to the model
	p_model->AppendPoints(v_nVertices, v_vertices);
	p_model->AppendWires(v_nWires, v_wires);
	p_model->AppendTriangles(v_nTriangs, v_triangs);
	p_model->SetConstructed();

	// Free memory
	delete [] v_vertices;
	delete [] v_wires;
	delete [] v_triangs;
}
