#include "cls_b_spline_surface.h"

// Project
#include "B_spline_basis.hpp"

template <typename STORETYPE, typename COMPUTETYPE>
cls_b_spline_surface<STORETYPE, COMPUTETYPE>::cls_b_spline_surface() :
	cls_surface_base<STORETYPE, COMPUTETYPE>(),
	mKnotsU(nullptr),
	mKnotsV(nullptr)
{
	this->mNsubSegments = 4; //TODO
}

template <typename STORETYPE, typename COMPUTETYPE>
cls_b_spline_surface<STORETYPE, COMPUTETYPE>::~cls_b_spline_surface()
{
	if (mKnotsU) { delete [] mKnotsU; mKnotsU = nullptr; }
	if (mKnotsV) { delete [] mKnotsV; mKnotsV = nullptr; }
}

//FIXME types!
template <typename STORETYPE, typename COMPUTETYPE>
void cls_b_spline_surface<STORETYPE, COMPUTETYPE>::Generate(unsigned int p_orderU, unsigned int p_orderV,
	                                                        unsigned int p_nCPsU,  unsigned int p_nCPsV)
{
	this->mOrderU = p_orderU;
	this->mOrderV = p_orderV;
	this->mNcontrolPointsU = p_nCPsU;
	this->mNcontrolPointsV = p_nCPsV;
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

	// Allocate memory for knot vectors here
	this->mKnotsU = new STORETYPE[this->mOrderU + this->mNcontrolPointsU];
	this->mKnotsV = new STORETYPE[this->mOrderV + this->mNcontrolPointsV];

	// Fill knot vectors
	this->GenerateKnotVector(this->mKnotsU, this->mOrderU, this->mNcontrolPointsU, &(this->mUmin), &(this->mUmax));
	this->GenerateKnotVector(this->mKnotsV, this->mOrderV, this->mNcontrolPointsV, &(this->mVmin), &(this->mVmax));
}

template <typename STORETYPE, typename COMPUTETYPE>
void cls_b_spline_surface<STORETYPE, COMPUTETYPE>::GenerateKnotVector(STORETYPE* p_knotVector,
                                                                      unsigned int p_order,
                                                                      unsigned int p_nCPs,
                                                                      unsigned int* p_paramMin,
                                                                      unsigned int* p_paramMax)
{
	unsigned int bound1 = p_order;
	unsigned int bound2 = p_nCPs+1;
	// XXX stands for U or V
	unsigned int nKnotsXXX = p_order + p_nCPs;

	LOG(DEBUG3) << "Knot vector:" << cls_logger::endl;
	LOG(DEBUG3) << "bound1=" << bound1 << cls_logger::endl;
	LOG(DEBUG3) << "bound2=" << bound2 << cls_logger::endl;

	p_knotVector[0] = 0.f;
	LOG(DEBUG3) << "1) t_" << 0 << "=" << p_knotVector[0] << cls_logger::endl;

	for (unsigned int j=1; j<bound1; j++) {
		p_knotVector[j] = p_knotVector[j-1];
		LOG(DEBUG3) << "1) t_" << j << "=" << p_knotVector[j] << cls_logger::endl;
	}
	for (unsigned int j=bound1; j<bound2; j++) {
		p_knotVector[j] = p_knotVector[j-1] + 1.f;
		LOG(DEBUG3) << "2) t_" << j << "=" << p_knotVector[j] << cls_logger::endl;
	}
	for (unsigned int j=bound2; j<nKnotsXXX; j++) {
		p_knotVector[j] = p_knotVector[j-1];
		LOG(DEBUG3) << "3) t_" << j << "=" << p_knotVector[j] << cls_logger::endl;
	}

	//startU = (float)(bound1);
	*p_paramMin = p_knotVector[bound1-1];
	*p_paramMax = p_knotVector[bound2-1];

	LOG(DEBUG3) << "min=" << *p_paramMin << cls_logger::endl;
	LOG(DEBUG3) << "max=" << *p_paramMax << cls_logger::endl;
}

template <typename STORETYPE, typename COMPUTETYPE>
COMPUTETYPE cls_b_spline_surface<STORETYPE, COMPUTETYPE>::GetBasisValueU(COMPUTETYPE u, unsigned int i) const
{
	return B_spline_basis::GetBasis(i, this->mOrderU-1, u, mKnotsU);
}

template <typename STORETYPE, typename COMPUTETYPE>
COMPUTETYPE cls_b_spline_surface<STORETYPE, COMPUTETYPE>::GetBasisValueV(COMPUTETYPE v, unsigned int j) const
{
	return B_spline_basis::GetBasis(j, this->mOrderV-1, v, mKnotsV);
}
