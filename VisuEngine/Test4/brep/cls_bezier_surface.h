/**

	@class cls_bezier_surface

*/

#ifndef CLS_BEZIER_SURFACE_H
#define CLS_BEZIER_SURFACE_H

class cls_model;

template <typename TYPE>
class cls_cartesian_point;

class cls_bezier_surface
{
public:
	cls_bezier_surface();
	~cls_bezier_surface();

	void Generate(unsigned int p_orderU, unsigned int p_orderV);

	void BuildModel(cls_model* p_model);

private:

	/**
	 * Output o_coord is of size 3
	 */
	void GetPoint(float p_paramU, float p_paramV, float* o_coord);

private:

	unsigned int mOrderU;
	unsigned int mOrderV;

	cls_cartesian_point<float>* mControlPoints; // [mOrderU * mOrderV]

	//float* mWeights;

private:

	float* mCacheU;
	float* mCacheV;

};

#endif // CLS_BEZIER_SURFACE_H