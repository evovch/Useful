#include "cls_model.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>

#include "rapidxml/rapidxml.hpp"
using rapidxml::xml_node;


#include "myTypes.h"
#include "cls_axis2_placement_3d.h"

cls_model::cls_model()
{
	this->GenerateAxisSystem();
	//this->Dump();
}

cls_model::~cls_model()
{
}

void cls_model::GenerateBox(void)
{
	mNumOfVertices = 8;
	mNumOfTriangles = 12;
	mNumOfWires = 12;
	mNumOfPoints = 8;

	//// ------------------------------------------------------------------------------------------

	//// Vertices and colors
	mVertexAndColorData = new stc_VandC[8]; //// mNumOfVertices

	//// Only colors
	for (unsigned int i=0; i<8; i++) { //// mNumOfVertices
		mVertexAndColorData[i].c[0] = (float)std::rand()/(float)RAND_MAX;
		mVertexAndColorData[i].c[1] = (float)std::rand()/(float)RAND_MAX;
		mVertexAndColorData[i].c[2] = (float)std::rand()/(float)RAND_MAX;
	}

	//// mNumOfVertices*3
	float v_coords[8*3] = {-0.5,-0.5,-0.5, -0.5,-0.5, 0.5,
	                       -0.5, 0.5,-0.5, -0.5, 0.5, 0.5,
	                        0.5,-0.5,-0.5,  0.5,-0.5, 0.5,
	                        0.5, 0.5,-0.5,  0.5, 0.5, 0.5};

	//// Vertices
	for (unsigned int i=0; i<8; i++) { //// mNumOfVertices
		mVertexAndColorData[i].v[0] = v_coords[i*3+0];
		mVertexAndColorData[i].v[1] = v_coords[i*3+1];
		mVertexAndColorData[i].v[2] = v_coords[i*3+2];
	}

	//// ------------------------------------------------------------------------------------------

	//// Triangles
	mTriangleIndices = new unsigned int[12*3]; //// mNumOfTriangles*3

	//// mNumOfTriangles*3
	unsigned int v_triangIndices[12*3] = {0,1,2, 1,3,2, 6,5,4, 7,5,6,
	                                      7,1,5, 7,3,1, 0,6,4, 0,2,6,
	                                      2,7,6, 2,3,7, 5,0,4, 5,1,0};

	for (unsigned int i=0; i<12*3; i++) { //// mNumOfTriangles*3
		mTriangleIndices[i] = v_triangIndices[i];
	}

	//// ------------------------------------------------------------------------------------------

	//// Wires
	mWireIndices = new unsigned int[12*2]; //// mNumOfWires*2

	//// mNumOfWires*2
	unsigned int v_wiresIndices[12*2] = {0,1, 0,2, 1,3, 2,3, 4,5, 4,6, 5,7, 6,7, 0,4, 1,5, 3,7, 2,6};

	for (unsigned int i=0; i<12*2; i++) { //// mNumOfWires*2
		mWireIndices[i] = v_wiresIndices[i];
	}

	//// ------------------------------------------------------------------------------------------

	//// Points
	mPointsIndices = new unsigned int[8]; //// mNumOfPoints

	for (unsigned int i=0; i<8; i++) { //// mNumOfPoints
		mPointsIndices[i] = i;
	}

	//// ------------------------------------------------------------------------------------------

	mConstructed = true;
}

void cls_model::GenerateAxisSystem(void)
{
	mNumOfVertices = 4;
	mNumOfTriangles = 0;
	mNumOfWires = 3;
	mNumOfPoints = 4;

	//// ------------------------------------------------------------------------------------------

	//// Vertices and colors
	mVertexAndColorData = new stc_VandC[4]; //// mNumOfVertices

	mVertexAndColorData[0].c[0] = 1.;
	mVertexAndColorData[0].c[1] = 1.;
	mVertexAndColorData[0].c[2] = 1.;
	mVertexAndColorData[1].c[0] = 1.;
	mVertexAndColorData[1].c[1] = 0.;
	mVertexAndColorData[1].c[2] = 0.;
	mVertexAndColorData[2].c[0] = 0.;
	mVertexAndColorData[2].c[1] = 1.;
	mVertexAndColorData[2].c[2] = 0.;
	mVertexAndColorData[3].c[0] = 0.;
	mVertexAndColorData[3].c[1] = 1.;
	mVertexAndColorData[3].c[2] = 1.;

	//// mNumOfVertices*3
	float v_coords[4*3] = {0.,0.,0., 1000.,0.,0., 0.,1000.,0., 0.,0.,1000.};

	//// Vertices
	for (unsigned int i=0; i<4; i++) { //// mNumOfVertices
		mVertexAndColorData[i].v[0] = v_coords[i*3+0];
		mVertexAndColorData[i].v[1] = v_coords[i*3+1];
		mVertexAndColorData[i].v[2] = v_coords[i*3+2];
	}

	//// ------------------------------------------------------------------------------------------

	//// Wires
	mWireIndices = new unsigned int[3*2]; //// mNumOfWires*2

	//// mNumOfWires*2
	unsigned int v_wiresIndices[3*2] = {1,0, 2,0, 3,0};

	for (unsigned int i=0; i<3*2; i++) { //// mNumOfWires*2
		mWireIndices[i] = v_wiresIndices[i];
	}

	//// ------------------------------------------------------------------------------------------

	//// Points
	mPointsIndices = new unsigned int[4]; //// mNumOfPoints

	for (unsigned int i=0; i<4; i++) { //// mNumOfPoints
		mPointsIndices[i] = i;
	}

	//// ------------------------------------------------------------------------------------------

	mConstructed = true;
}

void cls_model::GenerateLocalAxisSystem(cls_axis2_placement_3d* p_axisSys)
{
	// Append generated local axis system to the existing model

	// --------------------------------------------------------------------------------------------

	// Expand mVertexAndColorData by 4 elements
	stc_VandC* tmpVandC = new stc_VandC[mNumOfVertices+4]; //// mNumOfVertices + 4
	std::copy(mVertexAndColorData, mVertexAndColorData + mNumOfVertices, tmpVandC);
	delete [] mVertexAndColorData;
	mVertexAndColorData = tmpVandC;

	mVertexAndColorData[mNumOfVertices+0].v[0] = p_axisSys->mLocation.fX;
	mVertexAndColorData[mNumOfVertices+0].v[1] = p_axisSys->mLocation.fY;
	mVertexAndColorData[mNumOfVertices+0].v[2] = p_axisSys->mLocation.fZ;
	mVertexAndColorData[mNumOfVertices+1].v[0] = p_axisSys->mLocation.fX + p_axisSys->mXaxis.fPx;
	mVertexAndColorData[mNumOfVertices+1].v[1] = p_axisSys->mLocation.fY + p_axisSys->mXaxis.fPy;
	mVertexAndColorData[mNumOfVertices+1].v[2] = p_axisSys->mLocation.fZ + p_axisSys->mXaxis.fPz;
	mVertexAndColorData[mNumOfVertices+2].v[0] = p_axisSys->mLocation.fX + p_axisSys->mYaxis.fPx;
	mVertexAndColorData[mNumOfVertices+2].v[1] = p_axisSys->mLocation.fY + p_axisSys->mYaxis.fPy;
	mVertexAndColorData[mNumOfVertices+2].v[2] = p_axisSys->mLocation.fZ + p_axisSys->mYaxis.fPz;
	mVertexAndColorData[mNumOfVertices+3].v[0] = p_axisSys->mLocation.fX + p_axisSys->mAxis.fPx;
	mVertexAndColorData[mNumOfVertices+3].v[1] = p_axisSys->mLocation.fY + p_axisSys->mAxis.fPy;
	mVertexAndColorData[mNumOfVertices+3].v[2] = p_axisSys->mLocation.fZ + p_axisSys->mAxis.fPz;

	mVertexAndColorData[mNumOfVertices+0].c[0] = 1.;
	mVertexAndColorData[mNumOfVertices+0].c[1] = 1.;
	mVertexAndColorData[mNumOfVertices+0].c[2] = 1.;
	mVertexAndColorData[mNumOfVertices+1].c[0] = 1.;
	mVertexAndColorData[mNumOfVertices+1].c[1] = 0.;
	mVertexAndColorData[mNumOfVertices+1].c[2] = 0.;
	mVertexAndColorData[mNumOfVertices+2].c[0] = 0.;
	mVertexAndColorData[mNumOfVertices+2].c[1] = 1.;
	mVertexAndColorData[mNumOfVertices+2].c[2] = 0.;
	mVertexAndColorData[mNumOfVertices+3].c[0] = 0.;
	mVertexAndColorData[mNumOfVertices+3].c[1] = 1.;
	mVertexAndColorData[mNumOfVertices+3].c[2] = 1.;

	mNumOfVertices += 4;

	// --------------------------------------------------------------------------------------------

	// Triangles not changed
	////mNumOfTriangles += 0;

	// --------------------------------------------------------------------------------------------

	// Expand mWireIndices by 3*2 elements
	unsigned int* tmpWireIndices = new unsigned int[(mNumOfWires+3)*2]; //// (mNumOfWires+3)*2
	std::copy(mWireIndices, mWireIndices + mNumOfWires*2, tmpWireIndices);
	delete [] mWireIndices;
	mWireIndices = tmpWireIndices;

	// Add new indices
	unsigned int v_additionalWiresIndices[3*2] = {5,4, 6,4, 7,4};
	for (unsigned int i=mNumOfWires*2+0; i<mNumOfWires*2+3*2; i++) {
		mWireIndices[i] = v_additionalWiresIndices[i-mNumOfWires*2];
	}
	// Increase the number
	mNumOfWires += 3;

	// --------------------------------------------------------------------------------------------

	// Expand mPointsIndices by 4 elements
	unsigned int* tmpPointsIndices = new unsigned int[mNumOfPoints+4]; //// mNumOfPoints + 4
	std::copy(mPointsIndices, mPointsIndices + mNumOfPoints, tmpPointsIndices);
	delete [] mPointsIndices;
	mPointsIndices = tmpPointsIndices;

	// Add new indices
	for (unsigned int i=mNumOfPoints+0; i<mNumOfPoints+4; i++) {
		mPointsIndices[i] = i;
	}
	// Increase the number
	mNumOfPoints += 4;

	// --------------------------------------------------------------------------------------------
}

void cls_model::AppendPoints(unsigned int p_nPoints, float* p_array)
{
	fprintf(stderr, "mNumOfVertices=%d\n", mNumOfVertices);
	fprintf(stderr, "p_nPoints=%d\n", p_nPoints);

	// --------------------------------------------------------------------------------------------

	// Expand mVertexAndColorData by p_nPoints elements
	stc_VandC* tmpVandC = new stc_VandC[mNumOfVertices+p_nPoints]; //// mNumOfVertices + p_nPoints
	std::copy(mVertexAndColorData, mVertexAndColorData + mNumOfVertices, tmpVandC);
	delete [] mVertexAndColorData;
	mVertexAndColorData = tmpVandC;

	for (unsigned int i=0; i<p_nPoints; i++) {
		mVertexAndColorData[mNumOfVertices+i].v[0] = p_array[i*3+0];
		mVertexAndColorData[mNumOfVertices+i].v[1] = p_array[i*3+1];
		mVertexAndColorData[mNumOfVertices+i].v[2] = p_array[i*3+2];
		mVertexAndColorData[mNumOfVertices+i].c[0] = 0.7;
		mVertexAndColorData[mNumOfVertices+i].c[1] = 0.7;
		mVertexAndColorData[mNumOfVertices+i].c[2] = 0.7;
	}

	mNumOfVertices += p_nPoints;

	fprintf(stderr, "mNumOfVertices=%d\n", mNumOfVertices);

	// --------------------------------------------------------------------------------------------

	fprintf(stderr, "mNumOfPoints=%d\n", mNumOfPoints);

	// Expand mPointsIndices by p_nPoints elements
	unsigned int* tmpPointsIndices = new unsigned int[mNumOfPoints+p_nPoints]; //// mNumOfPoints + p_nPoints
	std::copy(mPointsIndices, mPointsIndices + mNumOfPoints, tmpPointsIndices);
	delete [] mPointsIndices;
	mPointsIndices = tmpPointsIndices;

	// Add new indices
	for (unsigned int i=mNumOfPoints+0; i<mNumOfPoints+p_nPoints; i++) {
		mPointsIndices[i] = i;
	}
	// Increase the number
	mNumOfPoints += p_nPoints;

	fprintf(stderr, "mNumOfPoints=%d\n", mNumOfPoints);

	// --------------------------------------------------------------------------------------------
}

void cls_model::AppendWires(unsigned int p_nWires, unsigned int* p_array)
{
	fprintf(stderr, "mNumOfWires=%d\n", mNumOfWires);
	fprintf(stderr, "p_nWires=%d\n", p_nWires);

	// --------------------------------------------------------------------------------------------

	// Expand mWireIndices by p_nWires*2 elements
	unsigned int* tmpWires = new unsigned int[mNumOfWires*2 + p_nWires*2]; //// mNumOfWires*2 + p_nWires*2
	std::copy(mWireIndices, mWireIndices + mNumOfWires*2, tmpWires);
	delete [] mWireIndices;
	mWireIndices = tmpWires;

	for (unsigned int i=0; i<p_nWires*2; i++) {
		mWireIndices[mNumOfWires*2+i] = p_array[i];
	}

	mNumOfWires += p_nWires;

	fprintf(stderr, "mNumOfWires=%d\n", mNumOfWires);
}

void cls_model::AppendTriangles(unsigned int p_nTriangles, unsigned int* p_array)
{
	fprintf(stderr, "mNumOfTriangles=%d\n", mNumOfTriangles);
	fprintf(stderr, "p_nTriangles=%d\n", p_nTriangles);

	// --------------------------------------------------------------------------------------------

	// Expand mTriangleIndices by p_nTriangles*3 elements
	unsigned int* tmpTriangles = new unsigned int[mNumOfTriangles*3 + p_nTriangles*3]; //// mNumOfTriangles*3 + p_nTriangles*3
	std::copy(mTriangleIndices, mTriangleIndices + mNumOfTriangles*3, tmpTriangles);
	delete [] mTriangleIndices;
	mTriangleIndices = tmpTriangles;

	for (unsigned int i=0; i<p_nTriangles*3; i++) {
		mTriangleIndices[mNumOfTriangles*3+i] = p_array[i];
	}

	mNumOfTriangles += p_nTriangles;

	fprintf(stderr, "mNumOfTriangles=%d\n", mNumOfTriangles);
}

void cls_model::Dump(void) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::Dump(): model is not yet constructed.\n");
		return;
	}

	fprintf(stdout, "----------------------------------------------------------------------\n");

	fprintf(stdout, "mNumOfVertices=%d\n", mNumOfVertices);
	fprintf(stdout, "mNumOfTriangles=%d\n", mNumOfTriangles);
	fprintf(stdout, "mNumOfWires=%d\n", mNumOfWires);
	fprintf(stdout, "mNumOfPoints=%d\n", mNumOfPoints);

	fprintf(stdout, "Vertices and colors ==================================================\n");

	for (unsigned int i=0; i<mNumOfVertices; i++) {
		fprintf(stdout, "x=%f\ty=%f\tz=%f\tr=%f\tg=%f\tb=%f\n",
		        mVertexAndColorData[i].v[0],
		        mVertexAndColorData[i].v[1],
		        mVertexAndColorData[i].v[2],
		        mVertexAndColorData[i].c[0],
		        mVertexAndColorData[i].c[1],
		        mVertexAndColorData[i].c[2]);
	}

	fprintf(stdout, "Triangles ============================================================\n");

	for (unsigned int i=0; i<mNumOfTriangles; i++) {
		fprintf(stdout, "%d, %d, %d\n", mTriangleIndices[i*3+0],
		                                mTriangleIndices[i*3+1],
		                                mTriangleIndices[i*3+2]);
	}

	fprintf(stdout, "Wires ================================================================\n");

	for (unsigned int i=0; i<mNumOfWires; i++) {
		fprintf(stdout, "%d, %d\n", mWireIndices[i*2+0],
		                            mWireIndices[i*2+1]);
	}

	fprintf(stdout, "Points ===============================================================\n");

	for (unsigned int i=0; i<mNumOfPoints; i++) {
		fprintf(stdout, "%d\n", mPointsIndices[i]);
	}

	fprintf(stdout, "----------------------------------------------------------------------\n");
}

int cls_model::ImportGDML(std::string p_filename)
{
	//TODO check that input file exists

	rapidxml::xml_document<> doc;
	xml_node<>* root_node;

	std::ifstream theFile(p_filename.c_str());
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);

	root_node = doc.first_node("gdml");
	if (!root_node) {
		fprintf(stderr, "<gdml> tag was not found. Aborting.\n");
		return 1;
	}

	// Points =====================================================================================

	xml_node<>* define_node = root_node->first_node("define");
	if (!define_node) {
		fprintf(stderr, "<define> tag was not found. Aborting.\n");
		return 2;
	}

	unsigned int posCounter = 0;
	for (xml_node<>* position_node = define_node->first_node("position");
		             position_node;
		             position_node = position_node->next_sibling("position"))
	{
		/*fprintf(stderr, "%s: %s, %s, %s, %s\n", position_node->first_attribute("name")->value(),
		    position_node->first_attribute("x")->value(),
		    position_node->first_attribute("y")->value(),
		    position_node->first_attribute("z")->value(),
		    position_node->first_attribute("unit")->value());*/
		posCounter++;
	}

	unsigned int nPoints = posCounter;
	float* pointsArray = new float[nPoints*3];
	posCounter = 0;
	for (xml_node<>* position_node = define_node->first_node("position");
		             position_node;
		             position_node = position_node->next_sibling("position"))
	{
		//char vertex_name[256];
		// skip first 'v' sign by shifting the address by 1 byte
		unsigned int vertexID = atoi(position_node->first_attribute("name")->value() + 1);
		/*fprintf(stderr, "%d: %s (%d): %s, %s, %s, %s\n",
		    posCounter,
		    position_node->first_attribute("name")->value(),
		    vertexID,
		    position_node->first_attribute("x")->value(),
		    position_node->first_attribute("y")->value(),
		    position_node->first_attribute("z")->value(),
		    position_node->first_attribute("unit")->value());*/

		if (posCounter != vertexID) {
			fprintf(stderr, "posCounter != vertexID\n");
		}

		float v_scalefactor = UnitToScaleFactor(position_node->first_attribute("unit")->value());

		pointsArray[posCounter*3+0] = v_scalefactor * atof(position_node->first_attribute("x")->value());
		pointsArray[posCounter*3+1] = v_scalefactor * atof(position_node->first_attribute("y")->value());
		pointsArray[posCounter*3+2] = v_scalefactor * atof(position_node->first_attribute("z")->value());

		posCounter++;
	}

	unsigned int oldNumOfPoints = mNumOfVertices;
	//unsigned int oldNumOfTriangles = mNumOfTriangles;

	this->AppendPoints(nPoints, pointsArray);

	delete [] pointsArray;

	// Triangles ==================================================================================

	xml_node<>* solids_node = root_node->first_node("solids");
	if (!solids_node) {
		fprintf(stderr, "<solids> tag was not found. Aborting.\n");
		return 2;
	}

	// Count <tesselated> tags ----------------------------------------------------------
	unsigned int v_nTesselated = 0;
	for (xml_node<>* v_tess_node = solids_node->first_node("tessellated");
		             v_tess_node;
		             v_tess_node = v_tess_node->next_sibling("tessellated"))
	{
		v_nTesselated++;
	}
	fprintf(stderr, "Total found %d tesselated solids.\n", v_nTesselated);

	// Allocate memory for the number of <triangular> for each <tesselated> -------------
	unsigned int* v_numberTriangs = new unsigned int[v_nTesselated];
	unsigned int** v_triangIndices = new unsigned int*[v_nTesselated];
	unsigned int** v_wiresIndices = new unsigned int*[v_nTesselated];

	// Count <triangular> tags for each <tesselated> ------------------------------------
	unsigned int v_tessCounter = 0;
	for (xml_node<>* v_tess_node = solids_node->first_node("tessellated");
		             v_tess_node;
		             v_tess_node = v_tess_node->next_sibling("tessellated"))
	{
		unsigned int triangCounter = 0;
		for (xml_node<>* v_triang_node = v_tess_node->first_node("triangular");
			             v_triang_node;
			             v_triang_node = v_triang_node->next_sibling("triangular"))
		{
			triangCounter++;
		}

		v_numberTriangs[v_tessCounter] = triangCounter;
		fprintf(stderr, "<%s> name=%s contains %d triangles\n", v_tess_node->name(),
			    v_tess_node->first_attribute("name")->value(), triangCounter);

		v_tessCounter++;
	}

	// Allocate memory
	for (v_tessCounter = 0; v_tessCounter < v_nTesselated; v_tessCounter++) {
		unsigned int curNumberOfTriangles = v_numberTriangs[v_tessCounter];
		fprintf(stderr, "Allocating v_tessCounter=%d curNumberOfTriangles=%d\n", v_tessCounter, curNumberOfTriangles);
		v_triangIndices[v_tessCounter] = new unsigned int[curNumberOfTriangles*3];
		v_wiresIndices[v_tessCounter] = new unsigned int[curNumberOfTriangles*6];
	}

	v_tessCounter = 0;
	for (xml_node<>* v_tess_node = solids_node->first_node("tessellated");
		             v_tess_node;
		             v_tess_node = v_tess_node->next_sibling("tessellated"))
	{
		unsigned int triangCounter = 0;
		for (xml_node<>* v_triang_node = v_tess_node->first_node("triangular");
			             v_triang_node;
			             v_triang_node = v_triang_node->next_sibling("triangular"))
		{
			unsigned int v1ID = atoi(v_triang_node->first_attribute("vertex1")->value() + 1);
			unsigned int v2ID = atoi(v_triang_node->first_attribute("vertex2")->value() + 1);
			unsigned int v3ID = atoi(v_triang_node->first_attribute("vertex3")->value() + 1);

			/*fprintf(stderr, "%s(%d)\t%s(%d)\t%s(%d)\n",
			        v_triang_node->first_attribute("vertex1")->value(), v1ID,
			        v_triang_node->first_attribute("vertex2")->value(), v2ID,
			        v_triang_node->first_attribute("vertex3")->value(), v3ID);*/

			v_triangIndices[v_tessCounter][triangCounter*3+0] = oldNumOfPoints + v1ID;
			v_triangIndices[v_tessCounter][triangCounter*3+1] = oldNumOfPoints + v2ID;
			v_triangIndices[v_tessCounter][triangCounter*3+2] = oldNumOfPoints + v3ID;

			v_wiresIndices[v_tessCounter][triangCounter*6+0] = oldNumOfPoints + v1ID;
			v_wiresIndices[v_tessCounter][triangCounter*6+1] = oldNumOfPoints + v2ID;
			v_wiresIndices[v_tessCounter][triangCounter*6+2] = oldNumOfPoints + v2ID;
			v_wiresIndices[v_tessCounter][triangCounter*6+3] = oldNumOfPoints + v3ID;
			v_wiresIndices[v_tessCounter][triangCounter*6+4] = oldNumOfPoints + v3ID;
			v_wiresIndices[v_tessCounter][triangCounter*6+5] = oldNumOfPoints + v1ID;

			triangCounter++;
		}

		v_tessCounter++;
	}

	for (v_tessCounter = 0; v_tessCounter < v_nTesselated; v_tessCounter++) {
		unsigned int curNumberOfTriangles = v_numberTriangs[v_tessCounter];
		this->AppendTriangles(curNumberOfTriangles, v_triangIndices[v_tessCounter]);
		this->AppendWires(curNumberOfTriangles*3, v_wiresIndices[v_tessCounter]);
	}

	// Free dynamic memory
	for (v_tessCounter = 0; v_tessCounter < v_nTesselated; v_tessCounter++) {
		delete [] v_triangIndices[v_tessCounter];
		delete [] v_wiresIndices[v_tessCounter];
	}
	delete [] v_triangIndices;
	delete [] v_wiresIndices;
	delete [] v_numberTriangs;

	return 0;
}

void cls_model::SendToGPUvAndC(GLuint p_VAO, GLuint p_VBO) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::SendToGPUvAndC(): model is not yet constructed.\n");
		return;
	}

	glBindVertexArray(p_VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, p_VBO);
		glBufferData(GL_ARRAY_BUFFER, mNumOfVertices*sizeof(stc_VandC), mVertexAndColorData, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, v));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, c));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}
	glBindVertexArray(0);
}

void cls_model::SendToGPUtriangles(GLuint p_IBO) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::SendToGPUtriangles(): model is not yet constructed.\n");
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfTriangles*3*sizeof(unsigned int), mTriangleIndices, GL_STATIC_DRAW);
}

void cls_model::SendToGPUwires(GLuint p_IBO) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::SendToGPUwires(): model is not yet constructed.\n");
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfWires*2*sizeof(unsigned int), mWireIndices, GL_STATIC_DRAW);
}

void cls_model::SendToGPUpoints(GLuint p_IBO) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::SendToGPUpoints(): model is not yet constructed.\n");
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfPoints*sizeof(unsigned int), mPointsIndices, GL_STATIC_DRAW);
}

void cls_model::SendToGPUFull(GLuint p_VAO, GLuint p_VBO, GLuint p_IBOtr, GLuint p_IBOwire, GLuint p_IBOpoints) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::SendToGPUFull(): model is not yet constructed.\n");
		return;
	}

	this->SendToGPUvAndC(p_VAO, p_VBO);
	this->SendToGPUtriangles(p_IBOtr);
	this->SendToGPUwires(p_IBOwire);
	this->SendToGPUpoints(p_IBOpoints);
}

void cls_model::DrawTriangles(GLuint p_program, GLuint p_vao, GLuint p_ibo) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::DrawTriangles(): model is not yet constructed.\n");
		return;
	}

	glUseProgram(p_program);
	glBindVertexArray(p_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ibo);
	glDrawElements(GL_TRIANGLES, mNumOfTriangles*3, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	glUseProgram(0);
}

void cls_model::DrawWires(GLuint p_program, GLuint p_vao, GLuint p_ibo) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::DrawWires(): model is not yet constructed.\n");
		return;
	}

	glUseProgram(p_program);
	glBindVertexArray(p_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ibo);
	glDrawElements(GL_LINES, mNumOfWires*2, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	glUseProgram(0);
}

void cls_model::DrawPoints(GLuint p_program, GLuint p_vao, GLuint p_ibo) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::DrawPoints(): model is not yet constructed.\n");
		return;
	}

	glUseProgram(p_program);
	glBindVertexArray(p_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ibo);
	glDrawElements(GL_POINTS, mNumOfPoints, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	glUseProgram(0);
}
