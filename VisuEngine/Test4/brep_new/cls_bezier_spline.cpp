#include "cls_bezier_spline.h"

// Project
#include "base/cls_logger.h"
#include "base/support.h"

template <typename STORETYPE, typename COMPUTETYPE>
cls_bezier_spline<STORETYPE, COMPUTETYPE>::cls_bezier_spline() :
	cls_spline_base<STORETYPE, COMPUTETYPE>()
{
	// Override default values of a generic spline
	this->mTmin = 0;
	this->mTmax = 1;
	//TODO initialize all members which need that
}

/*template <typename STORETYPE, typename COMPUTETYPE>
cls_bezier_spline<STORETYPE, COMPUTETYPE>::~cls_bezier_spline()
{
}*/

//FIXME types!
template <typename STORETYPE, typename COMPUTETYPE>
void cls_bezier_spline<STORETYPE, COMPUTETYPE>::Generate(unsigned int p_order, unsigned int p_nCPs)
{
	this->mOrder = p_order;

	// p_nCPs is ignored cause or Bezier curves number of control points must be order+1
	this->mNcontrolPoints = this->mOrder + 1;

	this->mControlPoints = new cls_cartesian_point<float>[this->mNcontrolPoints];
	this->mWeights = new STORETYPE[this->mNcontrolPoints];
	this->mCache = new COMPUTETYPE[this->mNcontrolPoints];

	for (unsigned int iCtrlPoint = 0; iCtrlPoint < this->mNcontrolPoints; iCtrlPoint++) {
		this->mControlPoints[iCtrlPoint].mCoord[0] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		this->mControlPoints[iCtrlPoint].mCoord[1] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		this->mControlPoints[iCtrlPoint].mCoord[2] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5); //FIXME
		this->mWeights[iCtrlPoint] = static_cast<STORETYPE>(1.); // * ((float)std::rand()/(float)RAND_MAX);
		LOG(DEBUG4) << "x=" << this->mControlPoints[iCtrlPoint].mCoord[0] << "\t"
		            << "y=" << this->mControlPoints[iCtrlPoint].mCoord[1] << "\t"
		            << "z=" << this->mControlPoints[iCtrlPoint].mCoord[2] << "\t"
		            << "weight" << iCtrlPoint << " = " << this->mWeights[iCtrlPoint] << cls_logger::endl;
	}
}

template <typename STORETYPE, typename COMPUTETYPE>
COMPUTETYPE cls_bezier_spline<STORETYPE, COMPUTETYPE>::GetBasisValue(COMPUTETYPE t, unsigned int i) const
{
	return BernsteinCoef(t, this->mOrder, i);
}
