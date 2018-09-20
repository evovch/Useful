#include "cls_b_spline_curve.h"

// Project
#include "base/support.h"
#include "B_spline_basis.hpp"

template <typename STORETYPE, typename COMPUTETYPE>
cls_b_spline_curve<STORETYPE, COMPUTETYPE>::cls_b_spline_curve() :
	cls_spline_base<STORETYPE, COMPUTETYPE>(),
	mKnotsT(nullptr)
{
}

template <typename STORETYPE, typename COMPUTETYPE>
cls_b_spline_curve<STORETYPE, COMPUTETYPE>::~cls_b_spline_curve()
{
	if (mKnotsT) { delete [] mKnotsT; mKnotsT = nullptr; }
}

//FIXME types!
template <typename STORETYPE, typename COMPUTETYPE>
void cls_b_spline_curve<STORETYPE, COMPUTETYPE>::Generate(unsigned int p_order, unsigned int p_nCPs)
{
	this->mOrder = p_order;
	this->mNcontrolPoints = p_nCPs;

	this->mControlPoints = new cls_cartesian_point<float>[this->mNcontrolPoints];
	this->mWeights = new STORETYPE[this->mNcontrolPoints];
	this->mCache = new COMPUTETYPE[this->mNcontrolPoints];

	for (unsigned int iCtrlPoint = 0; iCtrlPoint < this->mNcontrolPoints; iCtrlPoint++) {
		this->mControlPoints[iCtrlPoint].mCoord[0] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		this->mControlPoints[iCtrlPoint].mCoord[1] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		this->mControlPoints[iCtrlPoint].mCoord[2] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5); //FIXME
		this->mWeights[iCtrlPoint] = 1.; // * ((float)std::rand()/(float)RAND_MAX);
		LOG(DEBUG3) << "x=" << this->mControlPoints[iCtrlPoint].mCoord[0] << "\t"
		            << "y=" << this->mControlPoints[iCtrlPoint].mCoord[1] << "\t"
		            << "z=" << this->mControlPoints[iCtrlPoint].mCoord[2] << "\t"
		            << "weight" << iCtrlPoint << " = " << this->mWeights[iCtrlPoint] << cls_logger::endl;
	}

//TODO check
	unsigned int nKnotsT = this->mOrder + this->mNcontrolPoints;

	this->mKnotsT = new float[nKnotsT];

	LOG(DEBUG3) << "Knot vector:" << cls_logger::endl;

	unsigned int bound1 = this->mOrder;
	unsigned int bound2 = this->mNcontrolPoints+1;

	LOG(DEBUG3) << "bound1=" << bound1 << cls_logger::endl;
	LOG(DEBUG3) << "bound2=" << bound2 << cls_logger::endl;

	this->mKnotsT[0] = 0.f;
	LOG(DEBUG3) << "1) t_" << 0 << "=" << this->mKnotsT[0] << cls_logger::endl;

	for (unsigned int j=1; j<bound1; j++) {
		this->mKnotsT[j] = this->mKnotsT[j-1];
		LOG(DEBUG3) << "1) t_" << j << "=" << this->mKnotsT[j] << cls_logger::endl;
	}
	for (unsigned int j=bound1; j<bound2; j++) {
		this->mKnotsT[j] = this->mKnotsT[j-1] + 1.f;
		LOG(DEBUG3) << "2) t_" << j << "=" << this->mKnotsT[j] << cls_logger::endl;
	}
	for (unsigned int j=bound2; j<nKnotsT; j++) {
		this->mKnotsT[j] = this->mKnotsT[j-1];
		LOG(DEBUG3) << "3) t_" << j << "=" << this->mKnotsT[j] << cls_logger::endl;
	}

	//startU = (float)(bound1);
	this->mTmin = this->mKnotsT[bound1-1];
	this->mTmax = this->mKnotsT[bound2-1];

	LOG(DEBUG3) << "Tmin=" << this->mTmin << cls_logger::endl;
	LOG(DEBUG3) << "Tmax=" << this->mTmax << cls_logger::endl;
//TODO end check
}

template <typename STORETYPE, typename COMPUTETYPE>
COMPUTETYPE cls_b_spline_curve<STORETYPE, COMPUTETYPE>::GetBasisValue(COMPUTETYPE t, unsigned int i) const
{
	return B_spline_basis::GetBasis(i, this->mOrder-1, t, mKnotsT);
}
