#include "cls_b_spline.h"

// STD
#include <ctime>

// Project
#include "base/cls_logger.h"
#include "graphics/cls_model.h"
#include "brep/cls_cartesian_point.h"
#include "brep/cls_direction.h"

cls_b_spline::cls_b_spline() :
	mOrder(0),
	mNcontrolPoints(0),
	mControlPoints(nullptr),
	mWeights(nullptr),
	mKnots(nullptr),
	mCache(nullptr),
	startU(0.f)
{
	std::srand(std::time(nullptr));
}

cls_b_spline::~cls_b_spline()
{
	if (mControlPoints) { delete [] mControlPoints; mControlPoints = nullptr; }
	if (mWeights) { delete [] mWeights; mWeights = nullptr; }
	if (mKnots) { delete [] mKnots; mKnots = nullptr; }
	if (mCache) { delete [] mCache; mCache = nullptr; }
}

void cls_b_spline::Generate(unsigned int p_order, unsigned int p_nCPs)
{
	mOrder = p_order;
	mNcontrolPoints = p_nCPs;

	mControlPoints = new cls_cartesian_point<float>[mNcontrolPoints];
	mWeights = new float[mNcontrolPoints];

	for (unsigned int iCtrlPoint = 0; iCtrlPoint < mNcontrolPoints; iCtrlPoint++) {
		mControlPoints[iCtrlPoint].mCoord[0] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		mControlPoints[iCtrlPoint].mCoord[1] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		mControlPoints[iCtrlPoint].mCoord[2] = 0.; //100. * ((float)std::rand()/(float)RAND_MAX - 0.5); //FIXME
		mWeights[iCtrlPoint] = 1.; // * ((float)std::rand()/(float)RAND_MAX);
		LOG(DEBUG3) << "weight" << iCtrlPoint << " = " << mWeights[iCtrlPoint] << cls_logger::endl;
	}

	mCache = new float[mNcontrolPoints];

//TODO check
	unsigned int nKnots = mOrder+mNcontrolPoints;

	mKnots = new float[nKnots];

	LOG(DEBUG3) << "Knot vector:" << cls_logger::endl;

	unsigned int bound1 = mOrder;
	unsigned int bound2 = mNcontrolPoints+1;

	startU = (float)(bound1);

	mKnots[0] = 0.f;
	LOG(DEBUG3) << "1) t_" << 0 << "=" << mKnots[0] << cls_logger::endl;

	for (unsigned int j=1; j<bound1; j++) {
		mKnots[j] = mKnots[j-1];
		LOG(DEBUG3) << "1) t_" << j << "=" << mKnots[j] << cls_logger::endl;
	}
	for (unsigned int j=bound1; j<bound2; j++) {
		mKnots[j] = mKnots[j-1] + 1.f;
		LOG(DEBUG3) << "2) t_" << j << "=" << mKnots[j] << cls_logger::endl;
	}
	for (unsigned int j=bound2; j<nKnots; j++) {
		mKnots[j] = mKnots[j-1];
		LOG(DEBUG3) << "3) t_" << j << "=" << mKnots[j] << cls_logger::endl;
	}
//TODO end check

}

void cls_b_spline::BuildModel(cls_model* p_model)
{
	// Number of segments within one parameter range of unity
	// (i <= u <= i+1) (between two control points)
	unsigned int v_nSubSegments = 10;

	// Total number of points for the curve, including edges,
	// but not including control points, which are drawn separately
	unsigned int v_nPoints = v_nSubSegments * (mNcontrolPoints-1) + 1;

	// Allocate memory for both 'curve' points and control points
	float* v_vertices = new float[(mNcontrolPoints + v_nPoints)*3]; // each vertex is 3 cordinates, so *3

	// Fill control points
	for (unsigned int iCtrlPoint = 0; iCtrlPoint < mNcontrolPoints; iCtrlPoint++)
	{
		v_vertices[iCtrlPoint*3+0] = mControlPoints[iCtrlPoint].mCoord[0];
		v_vertices[iCtrlPoint*3+1] = mControlPoints[iCtrlPoint].mCoord[1];
		v_vertices[iCtrlPoint*3+2] = mControlPoints[iCtrlPoint].mCoord[2];
	}

	unsigned int v_vertShift = mNcontrolPoints;

	// Fill 'curve' points
	for (unsigned int iPoint = 0; iPoint < v_nPoints; iPoint++) {
		unsigned int iVertex = v_vertShift + iPoint;
		float v_param = startU + (float)(mNcontrolPoints-1)*(float)(iPoint)/(float)(v_nPoints-1);
		LOG(DEBUG2) << iPoint << ") param=" << v_param << cls_logger::endl;

		this->GetPoint(v_param, &v_vertices[iVertex*3]);

		LOG(DEBUG2) << "x=" << v_vertices[iVertex*3+0] << "\t"
		            << "y=" << v_vertices[iVertex*3+1] << "\t"
		            << "z=" << v_vertices[iVertex*3+2] << cls_logger::endl;
	}

	unsigned int v_nWires = v_nPoints - 1;
	unsigned int* v_wires = new unsigned int[v_nWires*2];

	for (unsigned int iWire = 0; iWire < v_nWires; iWire++) {
		v_wires[iWire*2+0] = v_vertShift + iWire+0;
		v_wires[iWire*2+1] = v_vertShift + iWire+1;
		LOG(DEBUG3) << "wire " << iWire << ": " << v_wires[iWire*2+0] << ", " << v_wires[iWire*2+1] << cls_logger::endl;
	}

	p_model->AppendPoints(mNcontrolPoints, &v_vertices[0], 0.8, 0.2, 0.2);
	p_model->AppendPoints(v_nPoints, &v_vertices[mNcontrolPoints*3]);
	p_model->AppendWires(v_nWires, v_wires);
	p_model->SetConstructed();

	// Free memory
	delete [] v_vertices;
}

void cls_b_spline::GetPoint(float p_param, float* o_coord) const
{
	float denom = 0.f;
	for (unsigned int i=0; i<mNcontrolPoints; i++) {
		mCache[i] = mWeights[i] * this->GetBasis<float>(i, mOrder, p_param);
		denom += mCache[i];
	}
	//LOG(DEBUG3) << "denom=" << denom << cls_logger::endl;
	for (unsigned int v_index = 0; v_index<3; v_index++) {
		float nom = 0.f;
		for (unsigned int i=0; i<mNcontrolPoints; i++) {
			nom += mCache[i] * mControlPoints[i].mCoord[v_index];
		}
		o_coord[v_index] = nom/denom;
	}
}

void cls_b_spline::GetPoint(float p_param, cls_cartesian_point<float>* o_point) const
{
	float v_point[3];
	this->GetPoint(p_param, v_point);
	o_point->mCoord[0] = v_point[0];
	o_point->mCoord[1] = v_point[1];
	o_point->mCoord[2] = v_point[2];
}

template <typename TYPE>
TYPE cls_b_spline::GetBasis(unsigned int i, unsigned int p, TYPE u) const
{
	static int depth = 0;
	depth++;

	TYPE retVal;

	/*fprintf(stderr, "u=%f\n", u);
	fprintf(stderr, "mKnots[i]=%f\n", mKnots[i]);
	fprintf(stderr, "mKnots[i+1]=%f\n", mKnots[i+1]);
	if (u >= static_cast<TYPE>(mKnots[i])) {
		fprintf(stderr, "u >= mKnots[i] ? : true\n");
	} else {
		fprintf(stderr, "u >= mKnots[i] ? : false\n");
	}*/

	if (p == 0) {
		if (u >= static_cast<TYPE>(mKnots[i]) && u < static_cast<TYPE>(mKnots[i+1])) { //TODO <= ?
			depth--;
			retVal = static_cast<TYPE>(1.);

			LOG(DEBUG3) << depth << "| 1 | GetBasis(" << i << "," << p << "," << u << ") = "
			            << retVal << cls_logger::endl;

			return retVal;
		} else {
			depth--;
			retVal = static_cast<TYPE>(0.);

			LOG(DEBUG3) << depth << "| 2 | GetBasis(" << i << "," << p << "," << u << ") = "
			            << retVal << cls_logger::endl;

			return retVal;
		}
	} else {
		TYPE Bi = this->GetBasis(i, p-1, u);
		TYPE Biplus1 = this->GetBasis(i+1, p-1, u);

		depth--;

		TYPE ti = static_cast<TYPE>(mKnots[i]);
		TYPE tipluspplus1 = static_cast<TYPE>(mKnots[i+p+1]);

		TYPE denom1 = (static_cast<TYPE>(mKnots[i+p]) - ti);
		TYPE denom2 = (tipluspplus1 - static_cast<TYPE>(mKnots[i+1]));

		TYPE term1 = static_cast<TYPE>(0.);
		if (denom1 != static_cast<TYPE>(0.)) {
			term1 = (u - ti) * Bi / denom1;
		}

		TYPE term2 = static_cast<TYPE>(0.);
		if (denom2 != static_cast<TYPE>(0.)) {
			term2 = (tipluspplus1 - u) * Biplus1 / denom2;
		}

		retVal = term1 + term2;

		LOG(DEBUG3) << depth << "| 3 | GetBasis(" << i << "," << p << "," << u << ") = "
		            << retVal << cls_logger::endl;

		return retVal;
	}
}
