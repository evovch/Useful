/**

	@class cls_scene

**/

#ifndef CLS_SCENE_H
#define CLS_SCENE_H

#include <list>

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

	void AddModel(cls_model* p_model) { mModels.push_back(p_model); }

private:

	std::list<cls_model*> mModels;

	/**
	 * These numbers are filled during cls_scene::SendToGPU()
	 */
	unsigned int mTotalNvertices;
	unsigned int mTotalNtriangles;
	unsigned int mTotalNwires;
	unsigned int mTotalNpoints;

};

#endif // CLS_SCENE_H
