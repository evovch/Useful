/**

	@class cls_scene

**/

#ifndef CLS_SCENE_H
#define CLS_SCENE_H

// STD
#include <vector>

// OpenGL
#include <GL/glew.h>

class cls_model;
class cls_renderer;
class cls_offscreen_renderer;

class cls_scene
{
public:
	cls_scene();
	~cls_scene();

	void SendToGPU(cls_renderer* p_rend, bool p_uniqueColor = false);

	void SendToGPUvAndC(cls_renderer* p_rend, bool p_uniqueColor = false);

	void HighlightTriangle(unsigned int p_index, GLuint p_VAO, GLuint p_VBO) const;

	/**
	 * This method should be called after SendToGPU()
	 */
	void Draw(cls_renderer* p_rend) const;

	void Draw(cls_offscreen_renderer* p_rend) const;

	void AddModel(cls_model* p_model);

	unsigned int GetOffset(unsigned int p_modelIndex) const { return mOffsets.at(p_modelIndex); }

	void Dump(void) const;

private:

	std::vector<cls_model*> mModels;

	/**
	 * These numbers are filled during cls_scene::SendToGPU()
	 */
	unsigned int mTotalNvertices;
	unsigned int mTotalNtriangles;
	unsigned int mTotalNwires;
	unsigned int mTotalNpoints;

	/**
	 *
	 */
	std::vector<unsigned int> mOffsets; // Only for vertices by now. Should also implement for triangles, wires and points...

	//TODO Move the matrix from the model class here, into the list


};

#endif // CLS_SCENE_H
