#include "cls_model.h"

#include <cstdlib>
#include <cstdio>

cls_model::cls_model()
{
	this->GenerateBox();
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

void cls_model::SendToGPUvAndC(GLuint p_VAO, GLuint p_VBO) const
{
	if (!mConstructed) {
		fprintf(stderr, "cls_model::SendToGPUvAndC(): model is not yet constructed.\n");
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
		fprintf(stderr, "cls_model::SendToGPUtriangles(): model is not yet constructed.\n");
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfTriangles*3*sizeof(unsigned int), mTriangleIndices, GL_STATIC_DRAW);
}

void cls_model::SendToGPUwires(GLuint p_IBO) const
{
	if (!mConstructed) {
		fprintf(stderr, "cls_model::SendToGPUwires(): model is not yet constructed.\n");
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfWires*2*sizeof(unsigned int), mWireIndices, GL_STATIC_DRAW);
}

void cls_model::SendToGPUpoints(GLuint p_IBO) const
{
	if (!mConstructed) {
		fprintf(stderr, "cls_model::SendToGPUpoints(): model is not yet constructed.\n");
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfPoints*sizeof(unsigned int), mPointsIndices, GL_STATIC_DRAW);
}

void cls_model::SendToGPUFull(GLuint p_VAO, GLuint p_VBO, GLuint p_IBOtr, GLuint p_IBOwire, GLuint p_IBOpoints) const
{
	if (!mConstructed) {
		fprintf(stderr, "cls_model::SendToGPUFull(): model is not yet constructed.\n");
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
		fprintf(stderr, "cls_model::DrawTriangles(): model is not yet constructed.\n");
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
		fprintf(stderr, "cls_model::DrawWires(): model is not yet constructed.\n");
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
		fprintf(stderr, "cls_model::DrawPoints(): model is not yet constructed.\n");
		return;
	}

	glUseProgram(p_program);
	glBindVertexArray(p_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ibo);
	glDrawElements(GL_POINTS, mNumOfPoints, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	glUseProgram(0);
}
