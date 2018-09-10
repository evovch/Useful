#include "cls_spline_base.h"

// STD
#include <iomanip>

// Project
#include "base/cls_logger.h"
#include "graphics/cls_model.h"

template <typename STORETYPE, typename COMPUTETYPE>
cls_spline_base<STORETYPE, COMPUTETYPE>::cls_spline_base() :
	mTmin(0),
	mTmax(0),
	mOrder(0),
	mNcontrolPoints(0),
	mControlPoints(nullptr),
	mWeights(nullptr),
	mCache(nullptr),
	mNsubSegments(10)
{
}

template <typename STORETYPE, typename COMPUTETYPE>
cls_spline_base<STORETYPE, COMPUTETYPE>::~cls_spline_base()
{
	if (mControlPoints) { delete [] mControlPoints; mControlPoints = nullptr; }
	if (mWeights) { delete [] mWeights; mWeights = nullptr; }
	if (mCache) { delete [] mCache; mCache = nullptr; }
}

template <typename STORETYPE, typename COMPUTETYPE>
void cls_spline_base<STORETYPE, COMPUTETYPE>::GetPoint(COMPUTETYPE p_param, STORETYPE* o_coord) const
{
	COMPUTETYPE denom = 0.f;
	for (unsigned int iT=0; iT<mNcontrolPoints; iT++) {
		mCache[iT] = this->GetBasisValue(p_param, iT) * mWeights[iT];
		LOG(DEBUG4) << "mCache[" << iT << "]=" << mCache[iT] << cls_logger::endl;
		denom += mCache[iT];
	}
	for (unsigned int v_index = 0; v_index<3; v_index++) {
		COMPUTETYPE nom = 0.f;
		for (unsigned int iT=0; iT<mNcontrolPoints; iT++) {
			nom += mCache[iT] * mControlPoints[iT].mCoord[v_index];
		}
		o_coord[v_index] = nom/denom;
	}
}

template <typename STORETYPE, typename COMPUTETYPE>
void cls_spline_base<STORETYPE, COMPUTETYPE>::GetPoint(COMPUTETYPE p_param, cls_cartesian_point<STORETYPE>* o_point) const
{
	STORETYPE v_point[3];
	this->GetPoint(p_param, v_point);
	o_point->mCoord[0] = v_point[0];
	o_point->mCoord[1] = v_point[1];
	o_point->mCoord[2] = v_point[2];
}

template <typename STORETYPE, typename COMPUTETYPE>
void cls_spline_base<STORETYPE, COMPUTETYPE>::BuildModel(cls_model* p_model)
{
	// Total number of points for the curve, including edges,
	// but not including control points, which are drawn separately
	unsigned int v_nPoints = mNsubSegments * (mNcontrolPoints-1) + 1; //TODO

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

		COMPUTETYPE v_param = this->mTmin + (this->mTmax - this->mTmin)*
		                      static_cast<COMPUTETYPE>(iPoint)/static_cast<COMPUTETYPE>(v_nPoints-1);

		LOG(DEBUG2) << iPoint << ") param=" << std::fixed << std::setprecision(6) << v_param << cls_logger::endl; //"\t";

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
