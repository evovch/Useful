#include "cls_model.h"

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

cls_model::cls_model()
{
/*	fprintf(stderr, "%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n",
		            mMatrix[0][0], mMatrix[0][1], mMatrix[0][2], mMatrix[0][3],
		            mMatrix[1][0], mMatrix[1][1], mMatrix[1][2], mMatrix[1][3],
		            mMatrix[2][0], mMatrix[2][1], mMatrix[2][2], mMatrix[2][3],
		            mMatrix[3][0], mMatrix[3][1], mMatrix[3][2], mMatrix[3][3]);*/

	// mMatrix seems to be automatically initialized with E (diagonal unity matrix)

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
	//TODO temporary solution
	//static bool firstTime = true;

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

	float startX, startY, startZ;

//	if (firstTime) {
		startX = 0.; //100. * (float)std::rand()/(float)RAND_MAX;
		startY = 0.; //100. * (float)std::rand()/(float)RAND_MAX;
		startZ = 0.; //100. * (float)std::rand()/(float)RAND_MAX;
//		firstTime = false;
//	} else {
//		startX = 100. * (float)std::rand()/(float)RAND_MAX;
//		startY = 100. * (float)std::rand()/(float)RAND_MAX;
//		startZ = 100. * (float)std::rand()/(float)RAND_MAX;
//	}

	//// mNumOfVertices*3
	float v_coords[4*3] = {startX,       startY,       startZ,
		                   startX+1000., startY+0.,    startZ+0.,
		                   startX+0.,    startY+1000., startZ+0.,
		                   startX+0.,    startY+0.,    startZ+1000.};

	/*fprintf(stderr, "cls_model::GenerateAxisSystem: startX = %f\n", startX);
	fprintf(stderr, "                               startY = %f\n", startY);
	fprintf(stderr, "                               startZ = %f\n", startZ);*/

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

int cls_model::ImportSTL(const char* p_filename)
{

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

void cls_model::SendToGPUvAndC(GLuint p_VAO, GLuint p_VBO, unsigned int p_overrideNvertices) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::SendToGPUvAndC(): model is not yet constructed.\n");
		return;
	}

	/*fprintf(stderr, "[DEBUG] SendToGPUvAndC:\n");
	for (unsigned int i=0; i<mNumOfVertices; i++) {
		fprintf(stderr, "[DEBUG] x=%f\ty=%f\tz=%f\tr=%f\tg=%f\tb=%f\n",
		                 mVertexAndColorData[i].v[0],
		                 mVertexAndColorData[i].v[1],
		                 mVertexAndColorData[i].v[2],
		                 mVertexAndColorData[i].c[0],
		                 mVertexAndColorData[i].c[1],
		                 mVertexAndColorData[i].c[2]);
	}*/

	glBindVertexArray(p_VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, p_VBO);
		if (p_overrideNvertices == 0) {
			glBufferData(GL_ARRAY_BUFFER, mNumOfVertices*sizeof(stc_VandC), mVertexAndColorData, GL_STATIC_DRAW);
		} else {
			glBufferData(GL_ARRAY_BUFFER, p_overrideNvertices*sizeof(stc_VandC), mVertexAndColorData, GL_STATIC_DRAW);
		}
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, v));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, c));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}
	glBindVertexArray(0);
}

void cls_model::SendToGPUtriangles(GLuint p_IBO, unsigned int p_overrideNtriangles) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::SendToGPUtriangles(): model is not yet constructed.\n");
		return;
	}

	/*fprintf(stderr, "[DEBUG] SendToGPUtriangles:\n");
	for (unsigned int i=0; i<mNumOfTriangles*3; i++) {
		fprintf(stderr, "[DEBUG] mTriangleIndices[%d]=%d\n", i, mTriangleIndices[i]);
	}*/

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
		fprintf(stderr, "[ERROR] cls_model::SendToGPUwires(): model is not yet constructed.\n");
		return;
	}

	/*fprintf(stderr, "[DEBUG] SendToGPUwires:\n");
	for (unsigned int i=0; i<mNumOfWires*2; i++) {
		fprintf(stderr, "[DEBUG] mWireIndices[%d]=%d\n", i, mWireIndices[i]);
	}*/

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
		fprintf(stderr, "[ERROR] cls_model::SendToGPUpoints(): model is not yet constructed.\n");
		return;
	}

	/*fprintf(stderr, "[DEBUG] SendToGPUpoints:\n");
	for (unsigned int i=0; i<mNumOfPoints; i++) {
		fprintf(stderr, "[DEBUG] mPointsIndices[%d]=%d\n", i, mPointsIndices[i]);
	}*/

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
		fprintf(stderr, "[ERROR] cls_model::SendToGPUFull(): model is not yet constructed.\n");
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
		fprintf(stderr, "[ERROR] cls_model::SendToGPUvAndC(): model is not yet constructed.\n");
		return;
	}

	//fprintf(stderr, "[DEBUG] AppendToGPUvAndC: p_offset=%ld\n", p_offset);
	for (unsigned int i=0; i<mNumOfVertices; i++) {

		glm::vec4 curVertex(mVertexAndColorData[i].v[0], mVertexAndColorData[i].v[1], mVertexAndColorData[i].v[2], 1.);
		glm::vec4 transformedVertex = mMatrix * curVertex;
		mVertexAndColorData[i].v[0] = transformedVertex[0];
		mVertexAndColorData[i].v[1] = transformedVertex[1];
		mVertexAndColorData[i].v[2] = transformedVertex[2];

		/*fprintf(stderr, "[DEBUG] x=%f\ty=%f\tz=%f\tr=%f\tg=%f\tb=%f\n",
		                 mVertexAndColorData[i].v[0],
		                 mVertexAndColorData[i].v[1],
		                 mVertexAndColorData[i].v[2],
		                 mVertexAndColorData[i].c[0],
		                 mVertexAndColorData[i].c[1],
		                 mVertexAndColorData[i].c[2]);*/
	}

	glBindVertexArray(p_VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, p_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, p_offset, mNumOfVertices*sizeof(stc_VandC), mVertexAndColorData);
		/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, v));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, c));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);*/ //TODO check that this is not required
	}
	glBindVertexArray(0);
}

void cls_model::AppendToGPUtriangles(GLuint p_IBO, GLintptr p_offset, unsigned int p_vertOffset) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::SendToGPUtriangles(): model is not yet constructed.\n");
		return;
	}

	//fprintf(stderr, "[DEBUG] AppendToGPUtriangles: p_offset=%ld p_vertOffset=%d\n", p_offset, p_vertOffset);
	for (unsigned int i=0; i<mNumOfTriangles*3; i++) {
		mTriangleIndices[i] += p_vertOffset; //TODO this line should not be commented!!!
		//fprintf(stderr, "[DEBUG] new mTriangleIndices[%d]=%d\n", i, mTriangleIndices[i]);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, p_offset, mNumOfTriangles*3*sizeof(unsigned int), mTriangleIndices);
}

void cls_model::AppendToGPUwires(GLuint p_IBO, GLintptr p_offset, unsigned int p_vertOffset) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::SendToGPUwires(): model is not yet constructed.\n");
		return;
	}

	//fprintf(stderr, "[DEBUG] AppendToGPUwires: p_offset=%ld p_vertOffset=%d\n", p_offset, p_vertOffset);
	for (unsigned int i=0; i<mNumOfWires*2; i++) {
		mWireIndices[i] += p_vertOffset; //TODO this line should not be commented!!!
		//fprintf(stderr, "[DEBUG] new mWireIndices[%d]=%d\n", i, mWireIndices[i]);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, p_offset, mNumOfWires*2*sizeof(unsigned int), mWireIndices);
}

void cls_model::AppendToGPUpoints(GLuint p_IBO, GLintptr p_offset, unsigned int p_vertOffset) const
{
	if (!mConstructed) {
		fprintf(stderr, "[ERROR] cls_model::SendToGPUpoints(): model is not yet constructed.\n");
		return;
	}

	//fprintf(stderr, "[DEBUG] AppendToGPUpoints: p_offset=%ld p_vertOffset=%d\n", p_offset, p_vertOffset);
	for (unsigned int i=0; i<mNumOfPoints; i++) {
		mPointsIndices[i] += p_vertOffset; //TODO this line should not be commented!!!
		//fprintf(stderr, "[DEBUG] new mPointsIndices[%d]=%d\n", i, mPointsIndices[i]);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, p_offset, mNumOfPoints*sizeof(unsigned int), mPointsIndices);
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

void cls_model::Shift(float p_x, float p_y, float p_z)
{
	mMatrix = glm::translate(mMatrix, glm::vec3(p_x, p_y, p_z));

	/*fprintf(stderr, "%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n",
		            mMatrix[0][0], mMatrix[0][1], mMatrix[0][2], mMatrix[0][3],
		            mMatrix[1][0], mMatrix[1][1], mMatrix[1][2], mMatrix[1][3],
		            mMatrix[2][0], mMatrix[2][1], mMatrix[2][2], mMatrix[2][3],
		            mMatrix[3][0], mMatrix[3][1], mMatrix[3][2], mMatrix[3][3]);*/
}
