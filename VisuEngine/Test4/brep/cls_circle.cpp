#include "cls_circle.h"

// GLM
#include <glm/gtc/constants.hpp>

// Project
#include "base/cls_logger.h"
#include "graphics/cls_model.h"

cls_circle::cls_circle()
{
	mPosition = glm::dvec3(0., 0., 0.);
	mNormal   = glm::dvec3(0., 0., 1.); // z
	mRefDir   = glm::dvec3(1., 1., 0.); // a

	// Normal and RefDir should be normalized
	mNormal = glm::normalize(mNormal);
	mRefDir = glm::normalize(mRefDir);
/*
	double az = glm::dot(mRefDir, mNormal); // az = a*z
	glm::dvec3 azz = az * mNormal; // azz = (a*z) z
	glm::dvec3 X = mRefDir - azz; // Exact X axis. // X = a - (a*z) x z // 'x' meaning cross product
*/

	mR = 20.;
}

cls_circle::~cls_circle()
{
}

void cls_circle::BuildModel(cls_model* p_model)
{
	double PI = glm::pi<double>();

	unsigned int nSections = 40;
	double curPhi = 0.;
	double dPhi = 2.*PI; // rad
	double stepPhi = dPhi / nSections;

	// Compute number of vertices, wires and triangles
	unsigned int v_nVertices = nSections+1;
	unsigned int v_nWires = nSections;

	LOG(INFO) << "Circle: " << v_nVertices << " vertices, " << v_nWires << " wires." << cls_logger::endl;
	//fprintf(stderr, "[INFO] Circle: %ld vertices, %ld wires.\n", v_nVertices, v_nWires);

	// Allocate memory
	float* v_vertices = new float[v_nVertices*3]; // each vertex is 3 cordinates, so *3
	unsigned int* v_wires = new unsigned int[v_nWires*2]; // each wire is two indices, so *2

	unsigned int v_oldNvertices = p_model->GetNumOfVertices();

	// Fill
	for (unsigned int iSection = 0; iSection < nSections; iSection++)
	{
		v_vertices[iSection*3+0] = mPosition.x + cos(curPhi) * mR;
		v_vertices[iSection*3+1] = mPosition.y + sin(curPhi) * mR;
		v_vertices[iSection*3+2] = mPosition.z + 0.;

		v_wires[iSection*2+0] = v_oldNvertices + iSection;
		v_wires[iSection*2+1] = v_oldNvertices + iSection+1;

		LOG(DEBUG) << "Wire " << iSection << "-" << iSection+1 << cls_logger::endl;
		//fprintf(stderr, "[DEBUG] wire: %d-%d\n", iSection, iSection+1);

		curPhi += stepPhi;
	}

	// Last point coinciding with the first
	v_vertices[(nSections)*3+0] = v_vertices[0*3+0];
	v_vertices[(nSections)*3+1] = v_vertices[0*3+1];
	v_vertices[(nSections)*3+2] = v_vertices[0*3+2];

	// Apply to the model
	p_model->AppendPoints(v_nVertices, v_vertices);
	p_model->AppendWires(v_nWires, v_wires);
	p_model->SetConstructed();

	// Free memory
	delete [] v_vertices;
	delete [] v_wires;

	// ------------------------------------------------
	// Center of the circle
	float* v_centerVertex = new float[3];

	v_centerVertex[0] = mPosition.x;
	v_centerVertex[1] = mPosition.y;
	v_centerVertex[2] = mPosition.z;

	// Wires
	unsigned int v_nRadialWires = (nSections+1);
	unsigned int* v_radialWires = new unsigned int[v_nRadialWires*2];
	for (unsigned int iSection = 0; iSection < v_nRadialWires; iSection++)
	{
		v_radialWires[iSection*2+0] = v_oldNvertices + nSections+1; // The one after the last vertex on the circle
		v_radialWires[iSection*2+1] = v_oldNvertices + iSection;
	}

	// Triangles
	unsigned int* v_triangles = new unsigned int[nSections*3];
	for (unsigned int iSection = 0; iSection < nSections; iSection++)
	{
		v_triangles[iSection*3+0] = v_oldNvertices + nSections+1; // The one after the last vertex on the circle
		v_triangles[iSection*3+1] = v_oldNvertices + iSection;
		v_triangles[iSection*3+2] = v_oldNvertices + iSection+1;
		LOG(DEBUG) << "Triangle " << iSection << ": " << v_oldNvertices + nSections+1 << " "
		     << v_oldNvertices + iSection << " " << v_oldNvertices + iSection+1 << cls_logger::endl;
	}

	// Apply to the model
	p_model->AppendPoints(1, v_centerVertex);
	p_model->AppendWires(v_nRadialWires, v_radialWires);
	p_model->AppendTriangles(nSections, v_triangles);
	////p_model->SetConstructed();

	// Free memory
	delete [] v_centerVertex;
	delete [] v_radialWires;
	delete [] v_triangles;
	// ------------------------------------------------
}

