#include "cls_bezier_surface.h"

// STD
#include <ctime>

// Project
#include "base/cls_logger.h"
#include "graphics/cls_model.h"
#include "cls_cartesian_point.h"

cls_bezier_surface::cls_bezier_surface() :
	mOrderU(0),
	mOrderV(0),
	mControlPoints(nullptr),
	mCacheU(nullptr),
	mCacheV(nullptr)
{
	std::srand(std::time(nullptr));
}

cls_bezier_surface::~cls_bezier_surface()
{
	if (mControlPoints) { delete [] mControlPoints; mControlPoints = nullptr; }
	if (mCacheU) { delete [] mCacheU; mCacheU = nullptr; }
	if (mCacheV) { delete [] mCacheV; mCacheV = nullptr; }
}

void cls_bezier_surface::Generate(unsigned int p_orderU, unsigned int p_orderV)
{
	mOrderU = p_orderU;
	mOrderV = p_orderV;
	unsigned int v_nCtrlPointsU = mOrderU + 1;
	unsigned int v_nCtrlPointsV = mOrderV + 1;
	unsigned int v_nCtrlPoints = v_nCtrlPointsU * v_nCtrlPointsV;

	mControlPoints = new cls_cartesian_point<float>[v_nCtrlPoints];
	mCacheU = new float[v_nCtrlPointsU];
	mCacheV = new float[v_nCtrlPointsV];

	for (unsigned int iU = 0; iU < v_nCtrlPointsU; iU++) {
		for (unsigned int iV = 0; iV < v_nCtrlPointsV; iV++) {
			unsigned int iCtrlPoint = iU * v_nCtrlPointsV + iV;

			mControlPoints[iCtrlPoint].mCoord[0] = 100. * ((float)iU/(float)(v_nCtrlPointsU-1) - 0.5);
			mControlPoints[iCtrlPoint].mCoord[1] = 100. * ((float)iV/(float)(v_nCtrlPointsV-1) - 0.5);
			mControlPoints[iCtrlPoint].mCoord[2] = 50. * ((float)std::rand()/(float)RAND_MAX);
		}
	}

}

void cls_bezier_surface::BuildModel(cls_model* p_model)
{
	unsigned int v_nCtrlPointsU = mOrderU + 1;
	unsigned int v_nCtrlPointsV = mOrderV + 1;
	unsigned int v_nCtrlPoints = v_nCtrlPointsU * v_nCtrlPointsV;

	LOG(DEBUG2) << "nCtrlPoints_U=" << v_nCtrlPointsU
	            << "\tnCtrlPoints_V=" << v_nCtrlPointsV
	            << "\tnCtrlPoints=" << v_nCtrlPoints
	            << cls_logger::endl;

	unsigned int v_nPointsU = 11*mOrderU;
	unsigned int v_nPointsV = 11*mOrderV;
	unsigned int v_nVertices = (v_nCtrlPointsU + v_nPointsU) * (v_nCtrlPointsV + v_nPointsV);

	LOG(DEBUG2) << "nPointsU=" << v_nPointsU
	            << "\tnPointsV=" << v_nPointsU
	            << "\tv_nVertices=" << v_nVertices
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
			float v_paramU = (float)(iPointU)/(float)(v_nPointsU-1);
			float v_paramV = (float)(iPointV)/(float)(v_nPointsV-1);
			unsigned int iVertex = v_nCtrlPoints + iPointU * v_nPointsV + iPointV;
			LOG(DEBUG4) << "paramU=" << v_paramU
			            << "\tparamV=" << v_paramV
			            << "\tiVertex=" << iVertex
			            << cls_logger::endl;
			this->GetPoint(v_paramU, v_paramV, &v_vertices[iVertex*3]);
		}
	}


	unsigned int v_vertShift = v_nCtrlPoints;

	unsigned int v_nCtrlGridWires = (v_nCtrlPointsU - 1) * v_nCtrlPointsV + 
	                                v_nCtrlPointsU * (v_nCtrlPointsV - 1) +
	                                (v_nCtrlPointsU - 1) * (v_nCtrlPointsV - 1);
	unsigned int v_nBezierWires = (v_nPointsU - 1) * v_nPointsV + 
	                              v_nPointsU * (v_nPointsV - 1) +
	                              (v_nPointsU - 1) * (v_nPointsV - 1);
	unsigned int v_nWires = v_nCtrlGridWires + v_nBezierWires;

	unsigned int* v_wires = new unsigned int[v_nWires*2];
	unsigned int iWire = 0;

	// Control grid only ======================================================
	for (unsigned int iU = 0; iU < v_nCtrlPointsU; iU++) {
		for (unsigned int iV = 0; iV < v_nCtrlPointsV-1; iV++) {
			v_wires[iWire*2+0] = iU * v_nCtrlPointsV + iV;
			v_wires[iWire*2+1] = iU * v_nCtrlPointsV + iV + 1;
			iWire++;
		}
	}
	for (unsigned int iU = 0; iU < v_nCtrlPointsU-1; iU++) {
		for (unsigned int iV = 0; iV < v_nCtrlPointsV; iV++) {
			v_wires[iWire*2+0] = iU * v_nCtrlPointsV + iV;
			v_wires[iWire*2+1] = (iU+1) * v_nCtrlPointsV + iV;
			iWire++;
		}
	}
	for (unsigned int iU = 0; iU < v_nCtrlPointsU-1; iU++) {
		for (unsigned int iV = 0; iV < v_nCtrlPointsV-1; iV++) {
			v_wires[iWire*2+0] = iU * v_nCtrlPointsV + iV;
			v_wires[iWire*2+1] = (iU+1) * v_nCtrlPointsV + iV + 1;
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

	unsigned int v_nCtrlGridTriangs = 2 * (v_nCtrlPointsU - 1) * (v_nCtrlPointsV - 1);
	unsigned int v_nBezierTriangs = 2 * (v_nPointsU - 1) * (v_nPointsV - 1);

	unsigned int v_nTriangles = v_nCtrlGridTriangs + v_nBezierTriangs;
	unsigned int* v_triangles = new unsigned int[v_nTriangles*3];
	unsigned int iTriangle = 0;

	// Control grid only ======================================================
	for (unsigned int iU = 0; iU < v_nCtrlPointsU - 1; iU++) {
		for (unsigned int iV = 0; iV < v_nCtrlPointsV - 1; iV++) {
			v_triangles[iTriangle*3*2+0] = iU * v_nCtrlPointsV + iV;
			v_triangles[iTriangle*3*2+1] = (iU+1) * v_nCtrlPointsV + iV + 1;
			v_triangles[iTriangle*3*2+2] = iU * v_nCtrlPointsV + iV + 1;
			v_triangles[iTriangle*3*2+3] = iU * v_nCtrlPointsV + iV;
			v_triangles[iTriangle*3*2+4] = (iU+1) * v_nCtrlPointsV + iV;
			v_triangles[iTriangle*3*2+5] = (iU+1) * v_nCtrlPointsV + iV + 1;
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

void cls_bezier_surface::GetPoint(float p_paramU, float p_paramV, float* o_coord)
{
	for (unsigned int iU = 0; iU <= mOrderU; iU++) {
		mCacheU[iU] = BernsteinCoef(p_paramU, mOrderU, iU);
	}
	for (unsigned int iV = 0; iV <= mOrderV; iV++) {
		mCacheV[iV] = BernsteinCoef(p_paramV, mOrderV, iV);
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
