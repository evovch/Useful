#include "cls_b_spline.h"

// Project
#include "base/support.h"

template <typename STORETYPE, typename COMPUTETYPE>
cls_b_spline<STORETYPE, COMPUTETYPE>::cls_b_spline() :
	cls_spline_base<STORETYPE, COMPUTETYPE>()
{
}

/*template <typename STORETYPE, typename COMPUTETYPE>
cls_b_spline<STORETYPE, COMPUTETYPE>::~cls_b_spline()
{
}*/

//FIXME types!
template <typename STORETYPE, typename COMPUTETYPE>
void cls_b_spline<STORETYPE, COMPUTETYPE>::Generate(unsigned int p_order, unsigned int p_nCPs)
{
	this->mOrder = p_order;
	this->mNcontrolPoints = p_nCPs;

	this->mControlPoints = new cls_cartesian_point<float>[this->mNcontrolPoints];
	this->mWeights = new STORETYPE[this->mNcontrolPoints];
	this->mCache = new COMPUTETYPE[this->mNcontrolPoints];

	for (unsigned int iCtrlPoint = 0; iCtrlPoint < this->mNcontrolPoints; iCtrlPoint++) {
		this->mControlPoints[iCtrlPoint].mCoord[0] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		this->mControlPoints[iCtrlPoint].mCoord[1] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		this->mControlPoints[iCtrlPoint].mCoord[2] = 0.; //100. * ((float)std::rand()/(float)RAND_MAX - 0.5); //FIXME
		this->mWeights[iCtrlPoint] = 1.; // * ((float)std::rand()/(float)RAND_MAX);
		LOG(DEBUG3) << "x=" << this->mControlPoints[iCtrlPoint].mCoord[0] << "\t"
		            << "y=" << this->mControlPoints[iCtrlPoint].mCoord[1] << "\t"
		            << "z=" << this->mControlPoints[iCtrlPoint].mCoord[2] << "\t"
		            << "weight" << iCtrlPoint << " = " << this->mWeights[iCtrlPoint] << cls_logger::endl;
	}

//TODO check
	unsigned int nKnots = this->mOrder + this->mNcontrolPoints;

	this->mKnots = new float[nKnots];

	LOG(DEBUG3) << "Knot vector:" << cls_logger::endl;

	unsigned int bound1 = this->mOrder;
	unsigned int bound2 = this->mNcontrolPoints+1;

	LOG(DEBUG3) << "bound1=" << bound1 << cls_logger::endl;
	LOG(DEBUG3) << "bound2=" << bound2 << cls_logger::endl;

	this->mKnots[0] = 0.f;
	LOG(DEBUG3) << "1) t_" << 0 << "=" << this->mKnots[0] << cls_logger::endl;

	for (unsigned int j=1; j<bound1; j++) {
		this->mKnots[j] = this->mKnots[j-1];
		LOG(DEBUG3) << "1) t_" << j << "=" << this->mKnots[j] << cls_logger::endl;
	}
	for (unsigned int j=bound1; j<bound2; j++) {
		this->mKnots[j] = this->mKnots[j-1] + 1.f;
		LOG(DEBUG3) << "2) t_" << j << "=" << this->mKnots[j] << cls_logger::endl;
	}
	for (unsigned int j=bound2; j<nKnots; j++) {
		this->mKnots[j] = this->mKnots[j-1];
		LOG(DEBUG3) << "3) t_" << j << "=" << this->mKnots[j] << cls_logger::endl;
	}

	//startU = (float)(bound1);
	this->mTmin = this->mKnots[bound1-1];
	this->mTmax = this->mKnots[bound2-1];

	LOG(DEBUG3) << "Tmin=" << this->mTmin << cls_logger::endl;
	LOG(DEBUG3) << "Tmax=" << this->mTmax << cls_logger::endl;
//TODO end check
}

template <typename STORETYPE, typename COMPUTETYPE>
COMPUTETYPE cls_b_spline<STORETYPE, COMPUTETYPE>::GetBasisValue(COMPUTETYPE t, unsigned int i) const
{
	return this->GetBasis(i, this->mOrder-1, t);
}

template <typename STORETYPE, typename COMPUTETYPE>
COMPUTETYPE cls_b_spline<STORETYPE, COMPUTETYPE>::GetBasis(unsigned int i, unsigned int p, COMPUTETYPE u) const
{
	static int depth = 0;
	depth++;

	COMPUTETYPE retVal;

	/*fprintf(stderr, "u=%f\n", u);
	fprintf(stderr, "mKnots[i]=%f\n", mKnots[i]);
	fprintf(stderr, "mKnots[i+1]=%f\n", mKnots[i+1]);
	if (u >= static_cast<COMPUTETYPE>(mKnots[i])) {
		fprintf(stderr, "u >= mKnots[i] ? : true\n");
	} else {
		fprintf(stderr, "u >= mKnots[i] ? : false\n");
	}*/

	if (p == 0) {

		/*
		COMPUTETYPE curKnot = static_cast<COMPUTETYPE>(mKnots[i]);
		COMPUTETYPE nxtKnot = static_cast<COMPUTETYPE>(mKnots[i+1]);
		LOG(DEBUG4) << "u   : " << GetBinaryRepresentation(sizeof(u), (void*)&u) << "\t" << u << cls_logger::endl;
		LOG(DEBUG4) << "t[" << i << "]: " << GetBinaryRepresentation(sizeof(curKnot), (void*)&curKnot) << "\t" << curKnot << cls_logger::endl;
		LOG(DEBUG4) << "t[" << i+1 << "]: " << GetBinaryRepresentation(sizeof(nxtKnot), (void*)&nxtKnot) << "\t" << nxtKnot << cls_logger::endl;
		*/
		
		if (u >= static_cast<COMPUTETYPE>(mKnots[i]) && u < static_cast<COMPUTETYPE>(mKnots[i+1])) { //TODO <= ?
			depth--;
			retVal = static_cast<COMPUTETYPE>(1.);

			LOG(DEBUG4) << depth << "| 1 | GetBasis(" << i << "," << p << "," << u << ") = "
			            << retVal << cls_logger::endl;

			return retVal;
		} else {
			depth--;
			retVal = static_cast<COMPUTETYPE>(0.);

			LOG(DEBUG4) << depth << "| 2 | GetBasis(" << i << "," << p << "," << u << ") = "
			            << retVal << cls_logger::endl;

			return retVal;
		}
	} else {
		COMPUTETYPE Bi = this->GetBasis(i, p-1, u);
		COMPUTETYPE Biplus1 = this->GetBasis(i+1, p-1, u);

		depth--;

		COMPUTETYPE ti = static_cast<COMPUTETYPE>(mKnots[i]);
		COMPUTETYPE tipluspplus1 = static_cast<COMPUTETYPE>(mKnots[i+p+1]);

		COMPUTETYPE denom1 = (static_cast<COMPUTETYPE>(mKnots[i+p]) - ti);
		COMPUTETYPE denom2 = (tipluspplus1 - static_cast<COMPUTETYPE>(mKnots[i+1]));

		COMPUTETYPE term1 = static_cast<COMPUTETYPE>(0.);
		if (denom1 != static_cast<COMPUTETYPE>(0.)) {
			term1 = (u - ti) * Bi / denom1;
		} else {
			//LOG(WARNING) << "Denominator = 0" << cls_logger::endl;
		}

		COMPUTETYPE term2 = static_cast<COMPUTETYPE>(0.);
		if (denom2 != static_cast<COMPUTETYPE>(0.)) {
			term2 = (tipluspplus1 - u) * Biplus1 / denom2;
		} else {
			//LOG(WARNING) << "Denominator = 0" << cls_logger::endl;
		}

		retVal = term1 + term2;

		LOG(DEBUG4) << depth << "| 3 | GetBasis(" << i << "," << p << "," << u << ") = "
		            << retVal << cls_logger::endl;

		return retVal;
	}
}
