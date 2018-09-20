#include "cls_surface_base.h"

// Project
#include "base/cls_logger.h"
#include "graphics/cls_model.h"

template <typename STORETYPE, typename COMPUTETYPE>
cls_surface_base<STORETYPE, COMPUTETYPE>::cls_surface_base() :
	mUmin(0),
	mUmax(0),
	mVmin(0),
	mVmax(0),
	mOrderU(0),
	mOrderV(0),
	mNcontrolPointsU(0),
	mNcontrolPointsV(0),
	mControlPoints(nullptr),
	mWeights(nullptr),
	mCacheU(nullptr),
	mCacheV(nullptr),
	mNsubSegments(10)
{
}

template <typename STORETYPE, typename COMPUTETYPE>
cls_surface_base<STORETYPE, COMPUTETYPE>::~cls_surface_base()
{
	if (mControlPoints) { delete [] mControlPoints; mControlPoints = nullptr; }
	if (mWeights) { delete [] mWeights; mWeights = nullptr; }
	if (mCacheU) { delete [] mCacheU; mCacheU = nullptr; }
	if (mCacheV) { delete [] mCacheV; mCacheV = nullptr; }
}

// +
template <typename STORETYPE, typename COMPUTETYPE>
void cls_surface_base<STORETYPE, COMPUTETYPE>::GetPoint(COMPUTETYPE p_paramU, COMPUTETYPE p_paramV, STORETYPE* o_coord) const
{
	for (unsigned int iU = 0; iU <= mOrderU; iU++) {
		mCacheU[iU] = this->GetBasisValueU(p_paramU, iU);
	}
	for (unsigned int iV = 0; iV <= mOrderV; iV++) {
		mCacheV[iV] = this->GetBasisValueV(p_paramV, iV);
	}

	for (unsigned int v_index = 0; v_index<3; v_index++) {
		float nom = 0.;
		for (unsigned int iU = 0; iU <= mOrderU; iU++) {
			for (unsigned int iV = 0; iV <= mOrderV; iV++) {
				unsigned int iCtrlPoint = iU * (mOrderV+1) + iV;
				nom += mCacheU[iU] * mCacheV[iV] * mControlPoints[iCtrlPoint].mCoord[v_index];
			}
		}
		o_coord[v_index] = nom;
	}
}

// +
template <typename STORETYPE, typename COMPUTETYPE>
void cls_surface_base<STORETYPE, COMPUTETYPE>::GetPoint(COMPUTETYPE p_paramU, COMPUTETYPE p_paramV, cls_cartesian_point<STORETYPE>* o_point) const
{
	STORETYPE v_point[3];
	this->GetPoint(p_paramU, p_paramV, v_point);
	o_point->mCoord[0] = v_point[0];
	o_point->mCoord[1] = v_point[1];
	o_point->mCoord[2] = v_point[2];
}

template <typename STORETYPE, typename COMPUTETYPE>
void cls_surface_base<STORETYPE, COMPUTETYPE>::BuildModel(cls_model* p_model)
{
	// Only control points
	unsigned int v_nCtrlPoints = mNcontrolPointsU * mNcontrolPointsV;
	//LOG(DEBUG2) << "nCtrlPoints=" << v_nCtrlPoints << cls_logger::endl;

	// Total number of points for the surface, including edges,
	// but not including control points, which are drawn separately
	unsigned int v_nPointsU = mNsubSegments*mOrderU + 1;
	unsigned int v_nPointsV = mNsubSegments*mOrderV + 1;
	unsigned int v_nPoints = v_nPointsU * v_nPointsV;

	unsigned int v_nVertices = v_nCtrlPoints + v_nPoints;

	LOG(DEBUG2) << "nPointsU=" << v_nPointsU
	            << "\tnPointsV=" << v_nPointsV
	            << "\tnPoints=" << v_nPoints
	            << "\tnVertices=" << v_nVertices
	            << cls_logger::endl;

	// Allocate memory
	float* v_vertices = new float[v_nVertices*3]; // each vertex is 3 cordinates, so *3

	// Fill
	for (unsigned int iCtrlPoint = 0; iCtrlPoint < v_nCtrlPoints; iCtrlPoint++)
	{
		v_vertices[iCtrlPoint*3+0] = mControlPoints[iCtrlPoint].mCoord[0];
		v_vertices[iCtrlPoint*3+1] = mControlPoints[iCtrlPoint].mCoord[1];
		v_vertices[iCtrlPoint*3+2] = mControlPoints[iCtrlPoint].mCoord[2];
	}

	// Fill 2
	for (unsigned int iPointU = 0; iPointU < v_nPointsU; iPointU++) {
		for (unsigned int iPointV = 0; iPointV < v_nPointsV; iPointV++) {

			COMPUTETYPE v_paramU = this->mUmin + (this->mUmax - this->mUmin)*
			                       static_cast<COMPUTETYPE>(iPointU)/static_cast<COMPUTETYPE>(v_nPointsU-1);

			COMPUTETYPE v_paramV = this->mVmin + (this->mVmax - this->mVmin)*
			                       static_cast<COMPUTETYPE>(iPointV)/static_cast<COMPUTETYPE>(v_nPointsV-1);

			unsigned int iVertex = v_nCtrlPoints + iPointU * v_nPointsV + iPointV;
			LOG(DEBUG4) << "paramU=" << v_paramU
			            << "\tparamV=" << v_paramV
			            << "\tiVertex=" << iVertex
			            << cls_logger::endl;
			this->GetPoint(v_paramU, v_paramV, &v_vertices[iVertex*3]);
		}
	}


	unsigned int v_vertShift = v_nCtrlPoints;

	unsigned int v_nCtrlGridWires = (mNcontrolPointsU - 1) * mNcontrolPointsV + 
	                                mNcontrolPointsU * (mNcontrolPointsV - 1) +
	                                (mNcontrolPointsU - 1) * (mNcontrolPointsV - 1);
	unsigned int v_nBezierWires = (v_nPointsU - 1) * v_nPointsV + 
	                              v_nPointsU * (v_nPointsV - 1) +
	                              (v_nPointsU - 1) * (v_nPointsV - 1);
	unsigned int v_nWires = v_nCtrlGridWires + v_nBezierWires;

	unsigned int* v_wires = new unsigned int[v_nWires*2];
	unsigned int iWire = 0;

	// Control grid only ======================================================
	for (unsigned int iU = 0; iU < mNcontrolPointsU; iU++) {
		for (unsigned int iV = 0; iV < mNcontrolPointsV-1; iV++) {
			v_wires[iWire*2+0] = iU * mNcontrolPointsV + iV;
			v_wires[iWire*2+1] = iU * mNcontrolPointsV + iV + 1;
			iWire++;
		}
	}
	for (unsigned int iU = 0; iU < mNcontrolPointsU-1; iU++) {
		for (unsigned int iV = 0; iV < mNcontrolPointsV; iV++) {
			v_wires[iWire*2+0] = iU * mNcontrolPointsV + iV;
			v_wires[iWire*2+1] = (iU+1) * mNcontrolPointsV + iV;
			iWire++;
		}
	}
	for (unsigned int iU = 0; iU < mNcontrolPointsU-1; iU++) {
		for (unsigned int iV = 0; iV < mNcontrolPointsV-1; iV++) {
			v_wires[iWire*2+0] = iU * mNcontrolPointsV + iV;
			v_wires[iWire*2+1] = (iU+1) * mNcontrolPointsV + iV + 1;
			iWire++;
		}
	}
	// ========================================================================

	LOG(DEBUG) << "iWire=" << iWire
	           << "\tv_nCtrlGridWires=" << v_nCtrlGridWires
	           << "\tv_nBezierWires=" << v_nBezierWires
	           << cls_logger::endl;

	// Bezier surface =========================================================
	iWire = v_nCtrlGridWires; //TODO to be on the safe side

	for (unsigned int iU = 0; iU < v_nPointsU; iU++) {
		for (unsigned int iV = 0; iV < v_nPointsV-1; iV++) {
			v_wires[iWire*2+0] = v_vertShift + iU * v_nPointsV + iV;
			v_wires[iWire*2+1] = v_vertShift + iU * v_nPointsV + iV + 1;
			iWire++;
		}
	}
	for (unsigned int iU = 0; iU < v_nPointsU-1; iU++) {
		for (unsigned int iV = 0; iV < v_nPointsV; iV++) {
			v_wires[iWire*2+0] = v_vertShift + iU * v_nPointsV + iV;
			v_wires[iWire*2+1] = v_vertShift + (iU+1) * v_nPointsV + iV;
			iWire++;
		}
	}
	for (unsigned int iU = 0; iU < v_nPointsU-1; iU++) {
		for (unsigned int iV = 0; iV < v_nPointsV-1; iV++) {
			v_wires[iWire*2+0] = v_vertShift + iU * v_nPointsV + iV;
			v_wires[iWire*2+1] = v_vertShift + (iU+1) * v_nPointsV + iV + 1;
			iWire++;
		}
	}
	// ========================================================================

	LOG(DEBUG) << "iWire=" << iWire
	           << "\tv_nCtrlGridWires=" << v_nCtrlGridWires
	           << "\tv_nBezierWires=" << v_nBezierWires
	           << cls_logger::endl;

	unsigned int v_nCtrlGridTriangs = 2 * (mNcontrolPointsU - 1) * (mNcontrolPointsV - 1);
	unsigned int v_nBezierTriangs = 2 * (v_nPointsU - 1) * (v_nPointsV - 1);

	unsigned int v_nTriangles = v_nCtrlGridTriangs + v_nBezierTriangs;
	unsigned int* v_triangles = new unsigned int[v_nTriangles*3];
	unsigned int iTriangle = 0;

	// Control grid only ======================================================
	for (unsigned int iU = 0; iU < mNcontrolPointsU - 1; iU++) {
		for (unsigned int iV = 0; iV < mNcontrolPointsV - 1; iV++) {
			v_triangles[iTriangle*3*2+0] = iU * mNcontrolPointsV + iV;
			v_triangles[iTriangle*3*2+1] = (iU+1) * mNcontrolPointsV + iV + 1;
			v_triangles[iTriangle*3*2+2] = iU * mNcontrolPointsV + iV + 1;
			v_triangles[iTriangle*3*2+3] = iU * mNcontrolPointsV + iV;
			v_triangles[iTriangle*3*2+4] = (iU+1) * mNcontrolPointsV + iV;
			v_triangles[iTriangle*3*2+5] = (iU+1) * mNcontrolPointsV + iV + 1;
			iTriangle++;
		}
	}
	// ========================================================================

	LOG(DEBUG) << "iTriangle=" << iTriangle
	           << "\tv_nCtrlGridTriangs/2=" << v_nCtrlGridTriangs/2
	           << "\tv_nBezierTriangs/2=" << v_nBezierTriangs/2
	           << cls_logger::endl;

	// Bezier surface =========================================================
	iTriangle = v_nCtrlGridTriangs/2; //TODO to be on the safe side
	for (unsigned int iU = 0; iU < v_nPointsU - 1; iU++) {
		for (unsigned int iV = 0; iV < v_nPointsV - 1; iV++) {
			v_triangles[iTriangle*3*2+0] = v_vertShift + iU * v_nPointsV + iV;
			v_triangles[iTriangle*3*2+1] = v_vertShift + (iU+1) * v_nPointsV + iV + 1;
			v_triangles[iTriangle*3*2+2] = v_vertShift + iU * v_nPointsV + iV + 1;
			v_triangles[iTriangle*3*2+3] = v_vertShift + iU * v_nPointsV + iV;
			v_triangles[iTriangle*3*2+4] = v_vertShift + (iU+1) * v_nPointsV + iV;
			v_triangles[iTriangle*3*2+5] = v_vertShift + (iU+1) * v_nPointsV + iV + 1;
			iTriangle++;
		}
	}
	// ========================================================================

	LOG(DEBUG) << "iTriangle=" << iTriangle
	           << "\tv_nCtrlGridTriangs/2=" << v_nCtrlGridTriangs/2
	           << "\tv_nBezierTriangs/2=" << v_nBezierTriangs/2
	           << cls_logger::endl;

	p_model->AppendPoints(v_nVertices, v_vertices);
	p_model->AppendWires(v_nCtrlGridWires, &v_wires[0]);
	//p_model->AppendWires(v_nBezierWires, &v_wires[v_nCtrlGridWires*2]);
	//p_model->AppendTriangles(v_nCtrlGridTriangs, &v_triangles[0]); //TODO
	p_model->AppendTriangles(v_nBezierTriangs, &v_triangles[v_nCtrlGridTriangs*3]); //TODO
	p_model->SetConstructed();

	// Free memory
	delete [] v_vertices;
	delete [] v_wires;
	delete [] v_triangles;
}
