/**

	@class cls_model

*/

#ifndef CLS_MODEL_H
#define CLS_MODEL_H

#include "support.h"

class cls_model
{
public:
	cls_model();
	~cls_model();

	void GenerateBox(void);

	// Send the display-model to the GPU using given OpenGL object
	void SendToGPUvAndC(GLuint p_VAO, GLuint p_VBO) const;
	void SendToGPUtriangles(GLuint p_IBO) const;
	void SendToGPUwires(GLuint p_IBO) const;
	void SendToGPUpoints(GLuint p_IBO) const;
	// Simply call SendToGPUvAndC(), SendToGPUtriangles(), SendToGPUwires(), SendToGPUpoints() one after another
	void SendToGPUFull(GLuint p_VAO, GLuint p_VBO, GLuint p_IBOtr, GLuint p_IBOwire, GLuint p_IBOpoints) const;

	void DrawTriangles(GLuint p_program, GLuint p_vao, GLuint p_ibo) const;
	void DrawWires(GLuint p_program, GLuint p_vao, GLuint p_ibo) const;
	void DrawPoints(GLuint p_program, GLuint p_vao, GLuint p_ibo) const;

	// Getters
	unsigned int GetNumOfVertices() const { return mNumOfVertices; }
	unsigned int GetNumOfTriangles() const { return mNumOfTriangles; }
	unsigned int GetNumOfWires() const { return mNumOfWires; }
	unsigned int GetNumOfPoints() const { return mNumOfPoints; }

	stc_VandC* GetVandCdata() const { return mVertexAndColorData; }
	unsigned int* GetTriangleIndices() const { return mTriangleIndices; }
	unsigned int* GetWireIndices() const { return mWireIndices; }
	unsigned int* GetPointsIndices() const { return mPointsIndices; }

private:

	unsigned int mNumOfVertices;

	unsigned int mNumOfTriangles;
	unsigned int mNumOfWires;
	unsigned int mNumOfPoints;

	stc_VandC* mVertexAndColorData;		// [mNumOfVertices]
	unsigned int* mTriangleIndices;		// [mNumOfTriangles*3]
	unsigned int* mWireIndices;			// [mNumOfWires*2]
	unsigned int* mPointsIndices;		// [mNumOfPoints]

	bool mConstructed;

};

#endif // CLS_MODEL_H
