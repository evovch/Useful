/**

	@class cls_bezier_spline

*/

#ifndef CLS_BEZIER_SPLINE_H
#define CLS_BEZIER_SPLINE_H

// STD
#include <vector>

class cls_model;

template <typename TYPE>
class cls_cartesian_point;

template <typename TYPE>
class cls_direction;

class cls_bezier_spline
{
public:
	cls_bezier_spline();
	~cls_bezier_spline();

	void Generate(unsigned int p_order);

	void BuildModel(cls_model* p_model);
	//void BuildModel1(cls_model* p_model);

	template <typename TYPE>
	void Intersect(cls_cartesian_point<TYPE>* p_start,
	               cls_direction<TYPE>* p_dir,
                   /*unsigned int* o_nSolutions,*/
	               std::vector< cls_cartesian_point<float> >& o_inters) const;

	/*template <typename TYPE>*/
	void TestInter(cls_model* p_model) const;

private:

	/**
	 * Output o_coord is of size 3
	 */
	void GetPoint(float p_param, float* o_coord) const;

	void GetPoint(float p_param, cls_cartesian_point<float>* o_point) const;

private:

	unsigned int mOrder;

	cls_cartesian_point<float>* mControlPoints;

	float* mWeights;

private:

	float* mCache;

};

#endif // CLS_BEZIER_SPLINE_H