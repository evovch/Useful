/**

	@class cls_model

*/

#ifndef CLS_MODEL_H
#define CLS_MODEL_H

// STD
#include <string>

// GLM
#include <glm/glm.hpp>

// Project
#include "support.h"

class cls_scene;

class cls_model
{
public:
	cls_model();
	~cls_model();

	void GenerateBox(void);
	void GenerateAxisSystem(void);

	void AppendPoints(unsigned int p_nPoints, float* p_array);
	void AppendWires(unsigned int p_nWires, unsigned int* p_array);
	void AppendTriangles(unsigned int p_nTriangles, unsigned int* p_array);

	void PrepareUniqueColors(void);

	void HighlightTriangle(unsigned int p_index, GLuint p_VAO, GLuint p_VBO) const;

	void Dump(void) const;

	// Send the display-model to the GPU using given OpenGL object
	void SendToGPUvAndC(GLuint p_VAO, GLuint p_VBO, unsigned int p_overrideNvertices = 0) const;
	void SendToGPUtriangles(GLuint p_IBO, unsigned int p_overrideNtriangles = 0) const;
	void SendToGPUwires(GLuint p_IBO, unsigned int p_overrideNwires = 0) const;
	void SendToGPUpoints(GLuint p_IBO, unsigned int p_overrideNpoints = 0) const;

	// Simply call SendToGPUvAndC(), SendToGPUtriangles(), SendToGPUwires(), SendToGPUpoints() one after another
	void SendToGPUFull(GLuint p_VAO, GLuint p_VBO, GLuint p_IBOtr, GLuint p_IBOwire, GLuint p_IBOpoints) const;

	// Offset in bytes
	void AppendToGPUvAndC(GLuint p_VAO, GLuint p_VBO, GLintptr p_offset) const;
	void AppendToGPUtriangles(GLuint p_IBO, GLintptr p_offset, unsigned int p_vertOffset) const;
	void AppendToGPUwires(GLuint p_IBO, GLintptr p_offset, unsigned int p_vertOffset) const;
	void AppendToGPUpoints(GLuint p_IBO, GLintptr p_offset, unsigned int p_vertOffset) const;
	void SetConstructed(bool p_value = true) { mConstructed = p_value; }

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

	// Operations on the 'model-in-the-scene' matrix
	void Shift(float p_x, float p_y, float p_z);

	void SetScene(cls_scene* p_scene, unsigned int p_index) { mScene = p_scene; mIndexInScene = p_index; }

private:
	void Reset(void);

private:
	unsigned int mNumOfVertices;
	unsigned int mNumOfTriangles;
	unsigned int mNumOfWires;
	unsigned int mNumOfPoints;

	stc_VandC* mVertexAndColorData;		// [mNumOfVertices]
	unsigned int* mTriangleIndices;		// [mNumOfTriangles*3]
	unsigned int* mWireIndices;			// [mNumOfWires*2]
	unsigned int* mPointsIndices;		// [mNumOfPoints]

	/**
	 * @brief A flag which indicates if the model is constructed or not yet ready to be used
	 */
	bool mConstructed;

	/**
	 * @brief 'Model-in-the-scene' matrix
	 */
	glm::mat4 mMatrix;

	stc_VandC* mVandCdataUniqueColors;	// [mNumOfVertices]

	/**
	 * The scene which contains this model, if any.
	 * The model can be insie one scene.
	 */
	cls_scene* mScene;

	unsigned int mIndexInScene;

};

#endif // CLS_MODEL_H
