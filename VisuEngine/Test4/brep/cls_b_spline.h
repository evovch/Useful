/**

	@class cls_b_spline

*/

#ifndef CLS_B_SPLINE_H
#define CLS_B_SPLINE_H

class cls_model;

template <typename TYPE>
class cls_cartesian_point;

template <typename TYPE>
class cls_direction;

class cls_b_spline
{
public:
	cls_b_spline();
	~cls_b_spline();

	void Generate(unsigned int p_order, unsigned int p_nCPs);

	void BuildModel(cls_model* p_model);

private:

	/**
	 * Output o_coord is of size 3
	 */
	void GetPoint(float p_param, float* o_coord) const;

	void GetPoint(float p_param, cls_cartesian_point<float>* o_point) const;

	template <typename TYPE>
	TYPE GetBasis(unsigned int i, unsigned int p, TYPE u) const;

private:

	unsigned int mOrder;

	unsigned int mNcontrolPoints;

	cls_cartesian_point<float>* mControlPoints;

	float* mWeights;

	float* mKnots;

private:

	float* mCache;

	float startU;

};

#endif // CLS_B_SPLINE_H