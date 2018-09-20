#include "cls_bezier_surface.h"

// Project
#include "base/support.h"

template <typename STORETYPE, typename COMPUTETYPE>
cls_bezier_surface<STORETYPE, COMPUTETYPE>::cls_bezier_surface() :
	cls_surface_base<STORETYPE, COMPUTETYPE>()
{
	// Override default values of a generic surface
	this->mUmin = 0;
	this->mUmax = 1;
	this->mVmin = 0;
	this->mVmax = 1;
	//TODO initialize all members which need that
}

/*template <typename STORETYPE, typename COMPUTETYPE>
cls_bezier_surface<STORETYPE, COMPUTETYPE>::~cls_bezier_surface()
{
}*/

//FIXME types!
// +
template <typename STORETYPE, typename COMPUTETYPE>
void cls_bezier_surface<STORETYPE, COMPUTETYPE>::Generate(unsigned int p_orderU, unsigned int p_orderV,
	                                                      unsigned int /*p_nCPsU*/,  unsigned int /*p_nCPsV*/)
{
	this->mOrderU = p_orderU;
	this->mOrderV = p_orderV;
	this->mNcontrolPointsU = this->mOrderU + 1;
	this->mNcontrolPointsV = this->mOrderV + 1;
	unsigned int v_nCtrlPoints = this->mNcontrolPointsU * this->mNcontrolPointsV;

	this->mControlPoints = new cls_cartesian_point<float>[v_nCtrlPoints];
	this->mWeights = new STORETYPE[v_nCtrlPoints];
	this->mCacheU = new COMPUTETYPE[this->mNcontrolPointsU];
	this->mCacheV = new COMPUTETYPE[this->mNcontrolPointsV];

	for (unsigned int iU = 0; iU < this->mNcontrolPointsU; iU++) {
		for (unsigned int iV = 0; iV < this->mNcontrolPointsV; iV++) {
			unsigned int iCtrlPoint = iU * this->mNcontrolPointsV + iV;

			this->mControlPoints[iCtrlPoint].mCoord[0] = 100. * ((float)iU/(float)(this->mNcontrolPointsU-1) - 0.5);
			this->mControlPoints[iCtrlPoint].mCoord[1] = 100. * ((float)iV/(float)(this->mNcontrolPointsV-1) - 0.5);
			this->mControlPoints[iCtrlPoint].mCoord[2] = 50. * ((float)std::rand()/(float)RAND_MAX);
			this->mWeights[iCtrlPoint] = static_cast<STORETYPE>(1.); // * ((float)std::rand()/(float)RAND_MAX);
			LOG(DEBUG4) << "x=" << this->mControlPoints[iCtrlPoint].mCoord[0] << "\t"
			            << "y=" << this->mControlPoints[iCtrlPoint].mCoord[1] << "\t"
			            << "z=" << this->mControlPoints[iCtrlPoint].mCoord[2] << "\t"
			            << "weight" << iCtrlPoint << " = " << this->mWeights[iCtrlPoint] << cls_logger::endl;
		}
	}
}

// +
template <typename STORETYPE, typename COMPUTETYPE>
COMPUTETYPE cls_bezier_surface<STORETYPE, COMPUTETYPE>::GetBasisValueU(COMPUTETYPE u, unsigned int i) const
{
	return BernsteinCoef(u, this->mOrderU, i);
}

// +
template <typename STORETYPE, typename COMPUTETYPE>
COMPUTETYPE cls_bezier_surface<STORETYPE, COMPUTETYPE>::GetBasisValueV(COMPUTETYPE v, unsigned int j) const
{
	return BernsteinCoef(v, this->mOrderV, j);
}
