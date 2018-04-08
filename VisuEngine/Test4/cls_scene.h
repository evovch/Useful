/**

	@class cls_scene

**/

#ifndef CLS_SCENE_H
#define CLS_SCENE_H

// STD
#include <vector>

class cls_model;
class cls_renderer;

class cls_scene
{
public:
	cls_scene();
	~cls_scene();

	void SendToGPU(cls_renderer* p_rend);

	/**
	 * This method should be called after SendToGPU()
	 */
	void Draw(cls_renderer* p_rend) const;

	void AddModel(cls_model* p_model);

	unsigned int GetOffset(unsigned int p_modelIndex) const { return mOffsets[p_modelIndex]; }

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

};

#endif // CLS_SCENE_H
