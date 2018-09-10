/**

	@class cls_spline_base

*/

#ifndef CLS_SPLINE_BASE_H
#define CLS_SPLINE_BASE_H

class cls_model;

template <typename TYPE>
class cls_cartesian_point;

template <typename STORETYPE, typename COMPUTETYPE>
class cls_spline_base
{
public:
	cls_spline_base() :
		mNsubSegments(10),
		mCache(nullptr)
	{}

	~cls_spline_base();

	void Evaluate(COMPUTETYPE p_u, cls_cartesian_point<STORETYPE>& o_point) const;

	virtual COMPUTETYPE GetBasisFval(COMPUTETYPE p_param, unsigned int iCP, unsigned int dj) const = 0;

	void BuildModel(cls_model* p_model) const;

private:

	unsigned int mNcontrolPoints; // n+1

	cls_cartesian_point<STORETYPE>* mControlPoints;

	STORETYPE* mWeights;

	unsigned int mTmin;

	unsigned int mTmax;

	unsigned int mOrder; // d

	STORETYPE* mKnots;

	/**
	 * Number of segments within one parameter range of unity size
	 * (i <= u <= i+1) (between two control points)
	 */
	unsigned int mNsubSegments;

	float* mCache;

};

#endif // CLS_SPLINE_BASE_H
