#include "cls_model.h"

// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "base/cls_logger.h"
#include "cls_scene.h"

cls_model::cls_model() :
	mNumOfVertices(0),
	mNumOfTriangles(0),
	mNumOfWires(0),
	mNumOfPoints(0),
	mVertexAndColorData(nullptr),
	mTriangleIndices(nullptr),
	mWireIndices(nullptr),
	mPointsIndices(nullptr),
	mConstructed(false),
	//mMatrix
	mVandCdataUniqueColors(nullptr),
	mScene(nullptr),
	mIndexInScene(0)
{
	LOG(DEBUG3) << "mMatrix at cls_model construction:" << cls_logger::endl
		        << "\t\t" << mMatrix[0][0] << " " << mMatrix[0][1] << " " << mMatrix[0][2] << " " << mMatrix[0][3] << cls_logger::endl
		        << "\t\t" << mMatrix[1][0] << " " << mMatrix[1][1] << " " << mMatrix[1][2] << " " << mMatrix[1][3] << cls_logger::endl
		        << "\t\t" << mMatrix[2][0] << " " << mMatrix[2][1] << " " << mMatrix[2][2] << " " << mMatrix[2][3] << cls_logger::endl
		        << "\t\t" << mMatrix[3][0] << " " << mMatrix[3][1] << " " << mMatrix[3][2] << " " << mMatrix[3][3] << cls_logger::endl;
	// mMatrix seems to be automatically initialized with E (diagonal unity matrix)

	//this->GenerateAxisSystem();
	//this->Dump();
}

cls_model::~cls_model()
{
	this->Reset();
}

void cls_model::Reset(void)
{
	if (mVertexAndColorData != nullptr) { delete [] mVertexAndColorData; mVertexAndColorData = nullptr; }
	if (mTriangleIndices != nullptr) { delete [] mTriangleIndices; mTriangleIndices = nullptr; }
	if (mWireIndices != nullptr) { delete [] mWireIndices; mWireIndices = nullptr; }
	if (mPointsIndices != nullptr) { delete [] mPointsIndices; mPointsIndices = nullptr; }
	mNumOfVertices = 0;
	mNumOfTriangles = 0;
	mNumOfWires = 0;
	mNumOfPoints = 0;
	mConstructed = false;
	mMatrix = glm::mat4();
	if (mVandCdataUniqueColors != nullptr) { delete [] mVandCdataUniqueColors; mVandCdataUniqueColors = nullptr; }
}

void cls_model::GenerateBox(void)
{
	this->Reset();

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
	this->Reset();

	//TODO temporary solution
	//static bool firstTime = true;

	mNumOfVertices = 4;
	mNumOfTriangles = 0;
	mNumOfWires = 3;
	mNumOfPoints = 4;

	//// ------------------------------------------------------------------------------------------

	//// Vertices and colors
	mVertexAndColorData = new stc_VandC[4]; //// mNumOfVertices

	mVertexAndColorData[0].c[0] = 1.f;
	mVertexAndColorData[0].c[1] = 1.f;
	mVertexAndColorData[0].c[2] = 1.f;
	mVertexAndColorData[1].c[0] = 1.f;
	mVertexAndColorData[1].c[1] = 0.f;
	mVertexAndColorData[1].c[2] = 0.f;
	mVertexAndColorData[2].c[0] = 0.f;
	mVertexAndColorData[2].c[1] = 1.f;
	mVertexAndColorData[2].c[2] = 0.f;
	mVertexAndColorData[3].c[0] = 0.f;
	mVertexAndColorData[3].c[1] = 1.f;
	mVertexAndColorData[3].c[2] = 1.f;

	float startX, startY, startZ;

//	if (firstTime) {
		startX = 0.f; //100. * (float)std::rand()/(float)RAND_MAX;
		startY = 0.f; //100. * (float)std::rand()/(float)RAND_MAX;
		startZ = 0.f; //100. * (float)std::rand()/(float)RAND_MAX;
//		firstTime = false;
//	} else {
//		startX = 100. * (float)std::rand()/(float)RAND_MAX;
//		startY = 100. * (float)std::rand()/(float)RAND_MAX;
//		startZ = 100. * (float)std::rand()/(float)RAND_MAX;
//	}

	float v_size = 100.f;

	//// mNumOfVertices*3
	float v_coords[4*3] = {startX,        startY,        startZ,
		                   startX+v_size, startY+0.f,    startZ+0.f,
		                   startX+0.f,    startY+v_size, startZ+0.f,
		                   startX+0.f,    startY+0.f,    startZ+v_size};

	LOG(DEBUG) << "Generate axis system: start point: (" << startX << ", " << startY << ", " << startZ << ")" << cls_logger::endl;

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

void cls_model::AppendPoints(unsigned int p_nPoints, float* p_array)
{
	LOG(DEBUG) << "before: mNumOfVertices=" << mNumOfVertices << cls_logger::endl;
	LOG(DEBUG) << "parameter: p_nPoints=" << p_nPoints << cls_logger::endl;

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

	LOG(DEBUG) << "after: mNumOfVertices=" << mNumOfVertices << cls_logger::endl;

	// --------------------------------------------------------------------------------------------

	LOG(DEBUG) << "before: mNumOfPoints=" << mNumOfPoints << cls_logger::endl;
	LOG(DEBUG) << "parameter: p_nPoints=" << p_nPoints << cls_logger::endl;

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

	LOG(DEBUG) << "after: mNumOfPoints=" << mNumOfPoints << cls_logger::endl;

	// --------------------------------------------------------------------------------------------
}

void cls_model::AppendWires(unsigned int p_nWires, unsigned int* p_array)
{
	LOG(DEBUG) << "before: mNumOfWires=" << mNumOfWires << cls_logger::endl;
	LOG(DEBUG) << "parameter: p_nWires=" << p_nWires << cls_logger::endl;

	// --------------------------------------------------------------------------------------------

	// Expand mWireIndices by p_nWires*2 elements
	unsigned int* tmpWires = new unsigned int[mNumOfWires*2 + p_nWires*2]; //// mNumOfWires*2 + p_nWires*2
	std::copy(mWireIndices, mWireIndices + mNumOfWires*2, tmpWires);
	delete [] mWireIndices;
	mWireIndices = tmpWires;

	for (unsigned int i=0; i<p_nWires*2; i++) {
		mWireIndices[mNumOfWires*2+i] = p_array[i]; //TODO fix
	}

	mNumOfWires += p_nWires;

	LOG(DEBUG) << "after: mNumOfWires=" << mNumOfWires << cls_logger::endl;
}

void cls_model::AppendTriangles(unsigned int p_nTriangles, unsigned int* p_array)
{

	LOG(DEBUG) << "before: mNumOfTriangles=" << mNumOfTriangles << cls_logger::endl;
	LOG(DEBUG) << "parameter: p_nTriangles=" << p_nTriangles << cls_logger::endl;

	// --------------------------------------------------------------------------------------------

	// Expand mTriangleIndices by p_nTriangles*3 elements
	unsigned int* tmpTriangles = new unsigned int[mNumOfTriangles*3 + p_nTriangles*3]; //// mNumOfTriangles*3 + p_nTriangles*3
	std::copy(mTriangleIndices, mTriangleIndices + mNumOfTriangles*3, tmpTriangles);
	delete [] mTriangleIndices; //TODO potential problem. If there were no triangles before....
	mTriangleIndices = tmpTriangles;

	for (unsigned int i=0; i<p_nTriangles*3; i++) {
		mTriangleIndices[mNumOfTriangles*3+i] = p_array[i]; //TODO fix
	}

	mNumOfTriangles += p_nTriangles;

	LOG(DEBUG) << "after: mNumOfTriangles=" << mNumOfTriangles << cls_logger::endl;
}

void cls_model::PrepareUniqueColors(void)
{
	if (mVandCdataUniqueColors) delete [] mVandCdataUniqueColors;

	mVandCdataUniqueColors = new stc_VandC[mNumOfVertices];
	std::copy(mVertexAndColorData, mVertexAndColorData + mNumOfVertices, mVandCdataUniqueColors);

	for (unsigned int i=0; i<mNumOfTriangles; i++) {
		IntToColor(i, &mVandCdataUniqueColors[mTriangleIndices[i*3+0]]); //TODO check
	}
}

void cls_model::HighlightTriangle(unsigned int p_index, GLuint p_VAO, GLuint p_VBO) const
{
	//TODO test this method

	unsigned int v_vertexIndex0 = mTriangleIndices[p_index*3+0];
/*	unsigned int v_vertexIndex1 = mTriangleIndices[p_index*3+1];
	unsigned int v_vertexIndex2 = mTriangleIndices[p_index*3+2];*/

	glm::vec4 curVertex0(mVertexAndColorData[v_vertexIndex0].v[0],
		                 mVertexAndColorData[v_vertexIndex0].v[1],
		                 mVertexAndColorData[v_vertexIndex0].v[2], 1.);
/*	glm::vec4 curVertex1(mVertexAndColorData[v_vertexIndex1].v[0],
		                 mVertexAndColorData[v_vertexIndex1].v[1],
		                 mVertexAndColorData[v_vertexIndex1].v[2], 1.);
	glm::vec4 curVertex2(mVertexAndColorData[v_vertexIndex2].v[0],
		                 mVertexAndColorData[v_vertexIndex2].v[1],
		                 mVertexAndColorData[v_vertexIndex2].v[2], 1.);*/
	glm::vec4 transformedVertex0 = mMatrix * curVertex0;
/*	glm::vec4 transformedVertex1 = mMatrix * curVertex1;
	glm::vec4 transformedVertex2 = mMatrix * curVertex2;*/

	stc_VandC* v_newVertexAndColorData = new stc_VandC[3];
	v_newVertexAndColorData[0].v[0] = transformedVertex0[0];
	v_newVertexAndColorData[0].v[1] = transformedVertex0[1];
	v_newVertexAndColorData[0].v[2] = transformedVertex0[2];
	v_newVertexAndColorData[0].c[0] = 0.0;
	v_newVertexAndColorData[0].c[1] = 1.0;
	v_newVertexAndColorData[0].c[2] = 0.0;
/*	v_newVertexAndColorData[1].v[0] = transformedVertex1[0];
	v_newVertexAndColorData[1].v[1] = transformedVertex1[1];
	v_newVertexAndColorData[1].v[2] = transformedVertex1[2];
	v_newVertexAndColorData[1].c[0] = 0.0;
	v_newVertexAndColorData[1].c[1] = 1.0;
	v_newVertexAndColorData[1].c[2] = 0.0;
	v_newVertexAndColorData[2].v[0] = transformedVertex2[0];
	v_newVertexAndColorData[2].v[1] = transformedVertex2[1];
	v_newVertexAndColorData[2].v[2] = transformedVertex2[2];
	v_newVertexAndColorData[2].c[0] = 0.0;
	v_newVertexAndColorData[2].c[1] = 1.0;
	v_newVertexAndColorData[2].c[2] = 0.0;*/

	unsigned int v_offset = v_vertexIndex0;
	unsigned int v_size = 1*sizeof(stc_VandC);
	unsigned int v_offsetInScene = mScene->GetOffset(mIndexInScene);

	LOG(DEBUG2) << "highlight triangle_index=" << p_index
	           << ", vertex_index0=" << v_vertexIndex0
	           << ", offset=" << v_offset
	           << ", v_offsetInScene=" << v_offsetInScene
	           << ", v_size=" << v_size << cls_logger::endl;

	unsigned int v_fullOffsetInBytes = (v_offsetInScene + v_offset)*sizeof(stc_VandC);

	glBindVertexArray(p_VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, p_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, v_fullOffsetInBytes, v_size, v_newVertexAndColorData);
		/*...*/ //TODO check that this is not required // see cls_model::AppendToGPUvAndC()
	}
	glBindVertexArray(0);

	delete [] v_newVertexAndColorData;
}

void cls_model::Dump(void) const
{
	if (!mConstructed) {
		LOG(ERROR) << "cls_model::Dump(): model is not yet constructed." << cls_logger::endl;
		return;
	}

	LOG(INFO) << "----------------------------------------------------------------------" << cls_logger::endl;

	LOG(INFO) << "mNumOfVertices=" << mNumOfVertices << cls_logger::endl;
	LOG(INFO) << "mNumOfTriangles=" << mNumOfTriangles << cls_logger::endl;
	LOG(INFO) << "mNumOfWires=" << mNumOfWires << cls_logger::endl;
	LOG(INFO) << "mNumOfPoints=" << mNumOfPoints << cls_logger::endl;

	LOG(INFO) << "Vertices and colors ==================================================" << cls_logger::endl;

	for (unsigned int i=0; i<mNumOfVertices; i++) {

		LOG(INFO) << "x=" << mVertexAndColorData[i].v[0] << "\t"
		          << "y=" << mVertexAndColorData[i].v[1] << "\t"
		          << "z=" << mVertexAndColorData[i].v[2] << "\t"
		          << "r=" << mVertexAndColorData[i].c[0] << "\t"
		          << "g=" << mVertexAndColorData[i].c[1] << "\t"
		          << "b=" << mVertexAndColorData[i].c[2] << cls_logger::endl;

	}

	LOG(INFO) << "Triangles ============================================================" << cls_logger::endl;

	for (unsigned int i=0; i<mNumOfTriangles; i++) {
		LOG(INFO) << "(" << mTriangleIndices[i*3+0] << "," << mTriangleIndices[i*3+1] << "," << mTriangleIndices[i*3+2] << ") ";
		if ((i+1)%4 == 0) { LOG(INFO) << cls_logger::endl; }
	}
	LOG(INFO) << cls_logger::endl;

	LOG(INFO) << "Wires ================================================================" << cls_logger::endl;

	for (unsigned int i=0; i<mNumOfWires; i++) {
		LOG(INFO) << "(" << mWireIndices[i*2+0] << "," << mWireIndices[i*2+1] << ") ";
		if ((i+1)%8 == 0) { LOG(INFO) << cls_logger::endl; }
	}
	LOG(INFO) << cls_logger::endl;

	LOG(INFO) << "Points ===============================================================" << cls_logger::endl;

	for (unsigned int i=0; i<mNumOfPoints; i++) {
		LOG(INFO) << mPointsIndices[i] << ", ";
		if ((i+1)%16 == 0) { LOG(INFO) << cls_logger::endl; }
	}
	LOG(INFO) << cls_logger::endl;

	//TODO dump mMatrix?
	//TODO dump mConstructed?
	//TODO dump mVandCdataUniqueColors?

	LOG(INFO) << "----------------------------------------------------------------------" << cls_logger::endl;
}

void cls_model::SendToGPUvAndC(GLuint p_VAO, GLuint p_VBO, unsigned int p_overrideNvertices) const
{
	if (!mConstructed) {
		LOG(ERROR) << "cls_model::SendToGPUvAndC(): model is not yet constructed." << cls_logger::endl;
		return;
	}

	stc_VandC* v_newVertexAndColorData;

	//TODO implement correct if statement
	//TODO do this only if the matrix is not trivial.
	//TODO Otherwise keep the 'new' pointer pointing to the 'old' one
	{
		v_newVertexAndColorData = new stc_VandC[mNumOfVertices];
		std::copy(mVertexAndColorData, mVertexAndColorData + mNumOfVertices, v_newVertexAndColorData);

		LOG(DEBUG) << "SendToGPUvAndC: (activate DEBUG4 level to see the data)" << cls_logger::endl;
		for (unsigned int i=0; i<mNumOfVertices; i++) {

			glm::vec4 curVertex(mVertexAndColorData[i].v[0], mVertexAndColorData[i].v[1], mVertexAndColorData[i].v[2], 1.);
			glm::vec4 transformedVertex = mMatrix * curVertex;
			v_newVertexAndColorData[i].v[0] = transformedVertex[0];
			v_newVertexAndColorData[i].v[1] = transformedVertex[1];
			v_newVertexAndColorData[i].v[2] = transformedVertex[2];

			LOG(DEBUG4) << "x=" << v_newVertexAndColorData[i].v[0] << "\t"
			            << "y=" << v_newVertexAndColorData[i].v[1] << "\t"
			            << "z=" << v_newVertexAndColorData[i].v[2] << "\t"
			            << "r=" << v_newVertexAndColorData[i].c[0] << "\t"
			            << "g=" << v_newVertexAndColorData[i].c[1] << "\t"
			            << "b=" << v_newVertexAndColorData[i].c[2] << cls_logger::endl;
		}

	}

	glBindVertexArray(p_VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, p_VBO);
		if (p_overrideNvertices == 0) {
			glBufferData(GL_ARRAY_BUFFER, mNumOfVertices*sizeof(stc_VandC), v_newVertexAndColorData, GL_STATIC_DRAW);
		} else {
			// Allocate memory - give full size
			glBufferData(GL_ARRAY_BUFFER, p_overrideNvertices*sizeof(stc_VandC), NULL, GL_STATIC_DRAW);
			// Send the data - give only the size of the data sent (offset = 0)
			glBufferSubData(GL_ARRAY_BUFFER, 0, mNumOfVertices*sizeof(stc_VandC), v_newVertexAndColorData);
		}
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, v));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, c));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}
	glBindVertexArray(0);

	//TODO check that we do not delete the original data!
	delete [] v_newVertexAndColorData;
}

void cls_model::SendToGPUtriangles(GLuint p_IBO, unsigned int p_overrideNtriangles) const
{
	if (!mConstructed) {
		LOG(ERROR) << "cls_model::SendToGPUtriangles(): model is not yet constructed." << cls_logger::endl;
		return;
	}

	LOG(DEBUG) << "SendToGPUtriangles: (activate DEBUG4 level to see the data)" << cls_logger::endl;
	for (unsigned int i=0; i<mNumOfTriangles*3; i++) {
		LOG(DEBUG4) << "mTriangleIndices[" << i << "]=" << mTriangleIndices[i] << cls_logger::endl;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	if (p_overrideNtriangles == 0) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfTriangles*3*sizeof(unsigned int), mTriangleIndices, GL_STATIC_DRAW);
	} else {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_overrideNtriangles*3*sizeof(unsigned int), mTriangleIndices, GL_STATIC_DRAW);
	}
}

void cls_model::SendToGPUwires(GLuint p_IBO, unsigned int p_overrideNwires) const
{
	if (!mConstructed) {
		LOG(ERROR) << "cls_model::SendToGPUwires(): model is not yet constructed." << cls_logger::endl;
		return;
	}

	LOG(DEBUG) << "SendToGPUwires: (activate DEBUG4 level to see the data)" << cls_logger::endl;
	for (unsigned int i=0; i<mNumOfWires*2; i++) {
		LOG(DEBUG4) << "mWireIndices[" << i << "]=" << mWireIndices[i] << cls_logger::endl;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	if (p_overrideNwires == 0) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfWires*2*sizeof(unsigned int), mWireIndices, GL_STATIC_DRAW);
	} else {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_overrideNwires*2*sizeof(unsigned int), mWireIndices, GL_STATIC_DRAW);
	}
}

void cls_model::SendToGPUpoints(GLuint p_IBO, unsigned int p_overrideNpoints) const
{
	if (!mConstructed) {
		LOG(ERROR) << "cls_model::SendToGPUpoints(): model is not yet constructed." << cls_logger::endl;
		return;
	}

	LOG(DEBUG) << "SendToGPUpoints: (activate DEBUG4 level to see the data)" << cls_logger::endl;
	for (unsigned int i=0; i<mNumOfPoints; i++) {
		LOG(DEBUG4) << "mPointsIndices[" << i << "]=" << mPointsIndices[i] << cls_logger::endl;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	if (p_overrideNpoints == 0) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfPoints*sizeof(unsigned int), mPointsIndices, GL_STATIC_DRAW);
	} else {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_overrideNpoints*sizeof(unsigned int), mPointsIndices, GL_STATIC_DRAW);
	}
}

void cls_model::SendToGPUFull(GLuint p_VAO, GLuint p_VBO, GLuint p_IBOtr, GLuint p_IBOwire, GLuint p_IBOpoints) const
{
	if (!mConstructed) {
		LOG(ERROR) << "cls_model::SendToGPUFull(): model is not yet constructed." << cls_logger::endl;
		return;
	}

	this->SendToGPUvAndC(p_VAO, p_VBO);
	this->SendToGPUtriangles(p_IBOtr);
	this->SendToGPUwires(p_IBOwire);
	this->SendToGPUpoints(p_IBOpoints);
}

void cls_model::AppendToGPUvAndC(GLuint p_VAO, GLuint p_VBO, GLintptr p_offset) const
{
	if (!mConstructed) {
		LOG(ERROR) << "cls_model::AppendToGPUvAndC(): model is not yet constructed." << cls_logger::endl;
		return;
	}

	stc_VandC* v_newVertexAndColorData;

	//TODO implement correct if statement
	//TODO do this only if the matrix is not trivial.
	//TODO Otherwise keep the 'new' pointer pointing to the 'old' one
	{
		v_newVertexAndColorData = new stc_VandC[mNumOfVertices];
		std::copy(mVertexAndColorData, mVertexAndColorData + mNumOfVertices, v_newVertexAndColorData);

		LOG(DEBUG) << "AppendToGPUvAndC: p_offset=" << p_offset
		           << " (activate DEBUG4 level to see the data)" << cls_logger::endl;
		for (unsigned int i=0; i<mNumOfVertices; i++) {

			glm::vec4 curVertex(mVertexAndColorData[i].v[0], mVertexAndColorData[i].v[1], mVertexAndColorData[i].v[2], 1.);
			glm::vec4 transformedVertex = mMatrix * curVertex;
			v_newVertexAndColorData[i].v[0] = transformedVertex[0];
			v_newVertexAndColorData[i].v[1] = transformedVertex[1];
			v_newVertexAndColorData[i].v[2] = transformedVertex[2];

			LOG(DEBUG4) << "x=" << v_newVertexAndColorData[i].v[0] << "\t"
			            << "y=" << v_newVertexAndColorData[i].v[1] << "\t"
			            << "z=" << v_newVertexAndColorData[i].v[2] << "\t"
			            << "r=" << v_newVertexAndColorData[i].c[0] << "\t"
			            << "g=" << v_newVertexAndColorData[i].c[1] << "\t"
			            << "b=" << v_newVertexAndColorData[i].c[2] << cls_logger::endl;
		}

	}

	glBindVertexArray(p_VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, p_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, p_offset, mNumOfVertices*sizeof(stc_VandC), v_newVertexAndColorData);
		/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, v));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, c));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);*/ //TODO check that this is not required
	}
	glBindVertexArray(0);

	//TODO check that we do not delete the original data!
	delete [] v_newVertexAndColorData;
}

void cls_model::AppendToGPUtriangles(GLuint p_IBO, GLintptr p_offset, unsigned int p_vertOffset) const
{
	if (!mConstructed) {
		LOG(ERROR) << "cls_model::AppendToGPUtriangles(): model is not yet constructed." << cls_logger::endl;
		return;
	}

	unsigned int* v_newTriangleIndices = new unsigned int[mNumOfTriangles*3];

	LOG(DEBUG) << "AppendToGPUtriangles: p_offset=" << p_offset << " p_vertOffset=" << p_vertOffset
	           << " (activate DEBUG4 level to see the data)" << cls_logger::endl;

	for (unsigned int i=0; i<mNumOfTriangles*3; i++) {
		v_newTriangleIndices[i] = mTriangleIndices[i] + p_vertOffset; //TODO this line should not be commented!!!
		LOG(DEBUG4) << "new mTriangleIndices[" << i << "]=" << mTriangleIndices[i] << cls_logger::endl;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, p_offset, mNumOfTriangles*3*sizeof(unsigned int), v_newTriangleIndices);

	delete [] v_newTriangleIndices;
}

void cls_model::AppendToGPUwires(GLuint p_IBO, GLintptr p_offset, unsigned int p_vertOffset) const
{
	if (!mConstructed) {
		LOG(ERROR) << "cls_model::AppendToGPUwires(): model is not yet constructed." << cls_logger::endl;
		return;
	}

	unsigned int* v_newWireIndices = new unsigned int[mNumOfWires*2];

	LOG(DEBUG) << "AppendToGPUwires: p_offset=" << p_offset << " p_vertOffset=" << p_vertOffset
	           << " (activate DEBUG4 level to see the data)" << cls_logger::endl;
	for (unsigned int i=0; i<mNumOfWires*2; i++) {
		v_newWireIndices[i] = mWireIndices[i] + p_vertOffset; //TODO this line should not be commented!!!
		LOG(DEBUG4) << "new mWireIndices[" << i << "]=" << mWireIndices[i] << cls_logger::endl;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, p_offset, mNumOfWires*2*sizeof(unsigned int), v_newWireIndices);

	delete [] v_newWireIndices;
}

void cls_model::AppendToGPUpoints(GLuint p_IBO, GLintptr p_offset, unsigned int p_vertOffset) const
{
	if (!mConstructed) {
		LOG(ERROR) << "cls_model::AppendToGPUpoints(): model is not yet constructed." << cls_logger::endl;
		return;
	}

	unsigned int* v_newPointsIndices = new unsigned int[mNumOfPoints];

	LOG(DEBUG) << "AppendToGPUpoints: p_offset=" << p_offset << " p_vertOffset=" << p_vertOffset
	           << " (activate DEBUG4 level to see the data)" << cls_logger::endl;
	for (unsigned int i=0; i<mNumOfPoints; i++) {
		v_newPointsIndices[i] = mPointsIndices[i] + p_vertOffset; //TODO this line should not be commented!!!
		LOG(DEBUG4) << "new mPointsIndices[" << i << "]=" << mPointsIndices[i] << cls_logger::endl;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, p_offset, mNumOfPoints*sizeof(unsigned int), v_newPointsIndices);

	delete [] v_newPointsIndices;
}

void cls_model::DrawTriangles(GLuint p_program, GLuint p_vao, GLuint p_ibo) const
{
	if (!mConstructed) {
		LOG(ERROR) << "cls_model::DrawTriangles(): model is not yet constructed." << cls_logger::endl;
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
		LOG(ERROR) << "cls_model::DrawWires(): model is not yet constructed." << cls_logger::endl;
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
		LOG(ERROR) << "cls_model::DrawPoints(): model is not yet constructed." << cls_logger::endl;
		return;
	}

	glUseProgram(p_program);
	glBindVertexArray(p_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ibo);
	glDrawElements(GL_POINTS, mNumOfPoints, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	glUseProgram(0);
}

void cls_model::Shift(float p_x, float p_y, float p_z)
{
	mMatrix = glm::translate(mMatrix, glm::vec3(p_x, p_y, p_z));

	LOG(DEBUG3) << "mMatrix after Shift():" << cls_logger::endl
		        << "\t\t" << mMatrix[0][0] << " " << mMatrix[0][1] << " " << mMatrix[0][2] << " " << mMatrix[0][3] << cls_logger::endl
		        << "\t\t" << mMatrix[1][0] << " " << mMatrix[1][1] << " " << mMatrix[1][2] << " " << mMatrix[1][3] << cls_logger::endl
		        << "\t\t" << mMatrix[2][0] << " " << mMatrix[2][1] << " " << mMatrix[2][2] << " " << mMatrix[2][3] << cls_logger::endl
		        << "\t\t" << mMatrix[3][0] << " " << mMatrix[3][1] << " " << mMatrix[3][2] << " " << mMatrix[3][3] << cls_logger::endl;
}
