#include "cls_spline_base.h"

// Project
#include "base/cls_logger.h"
#include "graphics/cls_model.h"
#include "brep/cls_cartesian_point.h"

template <typename STORETYPE, typename COMPUTETYPE>
void cls_spline_base<STORETYPE, COMPUTETYPE>::Evaluate(COMPUTETYPE p_u, cls_cartesian_point<STORETYPE>& o_point) const
{
	if (mCache == nullptr) {
		LOG(FATAL) << "Dynamic memory for cache not allocated. Aborting." << cls_logger::endl;
	}

	COMPUTETYPE denom = 0.f;
	for (unsigned int iT = 0; iT <= mOrder; iT++) {
		mCache[iT] = this->GetBasisFval() * mWeights[iT];
		denom += mCache[iT];
	}
	for (unsigned int v_index = 0; v_index<3; v_index++) {
		COMPUTETYPE nom = 0.f;
		for (unsigned int i=0; i<=mOrder; i++) {
			nom += mCache[i] * mControlPoints[i].mCoord[v_index];
		}
		o_point[v_index] = nom/denom;
	}
}

template <typename STORETYPE, typename COMPUTETYPE>
void cls_spline_base<STORETYPE, COMPUTETYPE>::BuildModel(cls_model* p_model) const
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

		COMPUTETYPE v_param = static_cast<COMPUTETYPE>(mNcontrolPoints-1)*
		                      static_cast<COMPUTETYPE>(iPoint)/static_cast<COMPUTETYPE>(v_nPoints-1);

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
