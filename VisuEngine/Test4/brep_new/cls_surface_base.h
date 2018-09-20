/**

	@class cls_surface_base

*/

#ifndef CLS_SURFACE_BASE_H
#define CLS_SURFACE_BASE_H

class cls_model;

/*template <typename TYPE>
class cls_cartesian_point;*/
#include "cls_cartesian_point.h"

template <typename STORETYPE, typename COMPUTETYPE>
class cls_surface_base
{
public:
	cls_surface_base();

	~cls_surface_base();

	/**
	 * Output o_coord is of size 3
	 */
	void GetPoint(COMPUTETYPE p_paramU, COMPUTETYPE p_paramV, STORETYPE* o_coord) const;

	/**
	 *
	 */
	void GetPoint(COMPUTETYPE p_paramU, COMPUTETYPE p_paramV, cls_cartesian_point<STORETYPE>* o_point) const;

	/**
	 *
	 */
	void BuildModel(cls_model* p_model); //TODO const?

protected: // methods

	virtual void Generate(unsigned int p_orderU, unsigned int p_orderV,
	                      unsigned int p_nCPsU,  unsigned int p_nCPsV) = 0;

	virtual COMPUTETYPE GetBasisValueU(COMPUTETYPE u, unsigned int i) const = 0;

	virtual COMPUTETYPE GetBasisValueV(COMPUTETYPE v, unsigned int j) const = 0;

protected: // data members
	/**
	 *
	 */
	unsigned int mUmin;

	/**
	 *
	 */
	unsigned int mUmax;

	/**
	 *
	 */
	unsigned int mVmin;

	/**
	 *
	 */
	unsigned int mVmax;

	/**
	 *
	 */
	unsigned int mOrderU;

	/**
	 *
	 */
	unsigned int mOrderV;

	/**
     *
	 */
	unsigned int mNcontrolPointsU;

	/**
     *
	 */
	unsigned int mNcontrolPointsV;

	/**
	 *
	 */
	cls_cartesian_point<STORETYPE>* mControlPoints; //[mNcontrolPointsU*mNcontrolPointsV]
	
	/**
	 *
	 */
	STORETYPE* mWeights; //[mNcontrolPointsU*mNcontrolPointsV]

	/**
	 *
	 */
	COMPUTETYPE* mCacheU; //[mNcontrolPointsU]

	/**
	 *
	 */
	COMPUTETYPE* mCacheV; //[mNcontrolPointsV]

	/**
	 * Number of segments within one parameter range of unity size
	 * (i <= u <= i+1) (between two control points)
	 */
	unsigned int mNsubSegments;

};

template class cls_surface_base<float, float>;

#endif // CLS_SURFACE_BASE_H
