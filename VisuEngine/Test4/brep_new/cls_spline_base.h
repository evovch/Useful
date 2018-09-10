/**

	@class cls_spline_base

*/

#ifndef CLS_SPLINE_BASE_H
#define CLS_SPLINE_BASE_H

class cls_model;

/*template <typename TYPE>
class cls_cartesian_point;*/
#include "cls_cartesian_point.h"

template <typename STORETYPE, typename COMPUTETYPE>
class cls_spline_base
{
public:
	cls_spline_base();

	~cls_spline_base();

	/**
	 * Output o_coord is of size 3
	 */
	void GetPoint(COMPUTETYPE p_param, STORETYPE* o_coord) const;

	/**
	 *
	 */
	void GetPoint(COMPUTETYPE p_param, cls_cartesian_point<STORETYPE>* o_point) const;

	/**
	 *
	 */
	void BuildModel(cls_model* p_model); //TODO const?

protected: // methods

	virtual void Generate(unsigned int p_order, unsigned int p_nCPs) = 0;

	virtual COMPUTETYPE GetBasisValue(COMPUTETYPE t, unsigned int i) const = 0;

protected: // data members
	/**
	 *
	 */
	unsigned int mTmin;

	/**
	 *
	 */
	unsigned int mTmax;

	/**
	 *
	 */
	unsigned int mOrder;

	/**
     *
	 */
	unsigned int mNcontrolPoints;

	/**
	 *
	 */
	cls_cartesian_point<STORETYPE>* mControlPoints; //[mNcontrolPoints]
	
	/**
	 *
	 */
	STORETYPE* mWeights; //[mNcontrolPoints]

	/**
	 *
	 */
	COMPUTETYPE* mCache; //[mNcontrolPoints]

	/**
	 * Number of segments within one parameter range of unity size
	 * (i <= u <= i+1) (between two control points)
	 */
	unsigned int mNsubSegments;

};

template class cls_spline_base<float, float>;

#endif // CLS_SPLINE_BASE_H
