#ifndef CLS_QUICK_HULL_H
#define CLS_QUICK_HULL_H

// STD
#include <list>
#include <vector>

// GLM
#include <glm/glm.hpp>

class cls_model; //TODO tmp

class cls_quick_hull
{
public:
	cls_quick_hull();
	~cls_quick_hull();

	static std::list<unsigned int> QuickHull(float* p_vertices,
	                                         unsigned int* p_indices,
	                                         unsigned int p_size,
	                                         cls_model* p_model);

	/** Input set is called S
	 *  S has the size p_size
	 *  P is the left point,
	 *  C is the farthest point on the left (looking from P to Q)
	 *  Q is the right point
	 *  Set S1 is on the left of PC
	 *  Set S2 is on the left of CQ
	 */
	static std::list<unsigned int> FindHull(float* p_vertices,
	                                        unsigned int* p_Sindices,
	                                        unsigned int p_Ssize,
	                                        unsigned int p_Pindex,
	                                        unsigned int p_Qindex,
	                                        cls_model* p_model);

	/*static std::vector<unsigned int> QuickHull2(float* p_vertices,
	                                             unsigned int* p_indices,
	                                             unsigned int p_size,
	                                             cls_model* p_model);*/

private:

	static float PointToLineDist(glm::fvec2 p_1, glm::fvec2 p_2, glm::fvec2 p_i);

	static bool OnTheLeft(glm::fvec2 p_1, glm::fvec2 p_2, glm::fvec2 p_i);

private:

	/**
	 * Step of the coordinates in the input p_vertices array
	 */
	static unsigned int DIMENSIONALITY;

};

#endif // CLS_QUICK_HULL_H
