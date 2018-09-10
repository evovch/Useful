#include "cls_bezier_spline.h"

// STD
#include <ctime>

// Project
#include "base/cls_logger.h"
#include "graphics/cls_model.h"
#include "brep/cls_cartesian_point.h"
#include "brep/cls_direction.h"

cls_bezier_spline::cls_bezier_spline() :
	mOrder(0),
	mControlPoints(nullptr),
	mWeights(nullptr),
	mCache(nullptr)
{
	std::srand(std::time(nullptr));
}

cls_bezier_spline::~cls_bezier_spline()
{
	if (mControlPoints) { delete [] mControlPoints; mControlPoints = nullptr; }
	if (mWeights) { delete [] mWeights; mWeights = nullptr; }
	if (mCache) { delete [] mCache; mCache = nullptr; }
}

void cls_bezier_spline::Generate(unsigned int p_order)
{
	mOrder = p_order;
	unsigned int v_nCtrlPoints = mOrder + 1;

	mControlPoints = new cls_cartesian_point<float>[v_nCtrlPoints];
	mWeights = new float[v_nCtrlPoints];
	mCache = new float[v_nCtrlPoints];

	for (unsigned int iCtrlPoint = 0; iCtrlPoint < v_nCtrlPoints; iCtrlPoint++) {
		mControlPoints[iCtrlPoint].mCoord[0] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		mControlPoints[iCtrlPoint].mCoord[1] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		mControlPoints[iCtrlPoint].mCoord[2] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5); //FIXME
		mWeights[iCtrlPoint] = 1.; // * ((float)std::rand()/(float)RAND_MAX);
		LOG(DEBUG4) << "weight" << iCtrlPoint << " = " << mWeights[iCtrlPoint] << cls_logger::endl;
	}
}

/*
void cls_bezier_spline::BuildModel1(cls_model* p_model)
{
	unsigned int v_nCtrlPoints = mOrder + 1;
	unsigned int v_nVertices = v_nCtrlPoints;

	// Allocate memory
	float* v_vertices = new float[v_nVertices*3]; // each vertex is 3 cordinates, so *3

	// Fill
	for (unsigned int iVertex = 0; iVertex < v_nVertices; iVertex++)
	{
		v_vertices[iVertex*3+0] = mControlPoints[iVertex].mCoord[0];
		v_vertices[iVertex*3+1] = mControlPoints[iVertex].mCoord[1];
		v_vertices[iVertex*3+2] = mControlPoints[iVertex].mCoord[2];
	}

	p_model->AppendPoints(v_nVertices, v_vertices);
	p_model->SetConstructed();

	// Free memory
	delete [] v_vertices;

	p_model->SetConstructed();
}
*/

void cls_bezier_spline::BuildModel(cls_model* p_model)
{
	unsigned int v_nCtrlPoints = mOrder + 1;
	unsigned int v_nPoints = 20;

	// Allocate memory
	float* v_vertices = new float[(v_nCtrlPoints + v_nPoints)*3]; // each vertex is 3 cordinates, so *3

	// Fill
	for (unsigned int iCtrlPoint = 0; iCtrlPoint < v_nCtrlPoints; iCtrlPoint++)
	{
		v_vertices[iCtrlPoint*3+0] = mControlPoints[iCtrlPoint].mCoord[0];
		v_vertices[iCtrlPoint*3+1] = mControlPoints[iCtrlPoint].mCoord[1];
		v_vertices[iCtrlPoint*3+2] = mControlPoints[iCtrlPoint].mCoord[2];
	}

	for (unsigned int iPoint = 0; iPoint < v_nPoints; iPoint++) {
		unsigned int iVertex = v_nCtrlPoints + iPoint;
		float v_param = (float)(iPoint)/(float)(v_nPoints-1);
		LOG(DEBUG4) << "param=" << v_param << cls_logger::endl;

		this->GetPoint(v_param, &v_vertices[iVertex*3]);
	}

	unsigned int v_vertShift = v_nCtrlPoints;

	unsigned int v_nWires = v_nPoints - 1;
	unsigned int* v_wires = new unsigned int[v_nWires*2];

	for (unsigned int iWire = 0; iWire < v_nWires; iWire++) {
		v_wires[iWire*2+0] = v_vertShift + iWire+0;
		v_wires[iWire*2+1] = v_vertShift + iWire+1;
		LOG(DEBUG4) << "wire " << iWire << ": " << v_wires[iWire*2+0] << ", " << v_wires[iWire*2+1] << cls_logger::endl;
	}

	p_model->AppendPoints(v_nCtrlPoints, &v_vertices[0], 0.9f, 0.2f, 0.2f);
	p_model->AppendPoints(v_nPoints, &v_vertices[v_nCtrlPoints*3]);
	p_model->AppendWires(v_nWires, v_wires);
	p_model->SetConstructed();

	// Free memory
	delete [] v_vertices;
}

/*template <typename TYPE>*/
void cls_bezier_spline::TestInter(cls_model* p_model) const
{
	unsigned int nIntersMax = 2;

	float* v_vertices = new float[(2+nIntersMax)*3];

	// Ray
	for (unsigned int i=0; i<2; i++) {
		v_vertices[i*3+0] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		v_vertices[i*3+1] = 100. * ((float)std::rand()/(float)RAND_MAX - 0.5);
		v_vertices[i*3+2] = 0.; //100. * ((float)std::rand()/(float)RAND_MAX - 0.5); //FIXME
	}
	unsigned int* v_wires = new unsigned int[1*2];
	v_wires[0] = 0;
	v_wires[1] = 1;

	cls_cartesian_point<float> stPoint(v_vertices[0], v_vertices[1], v_vertices[2]);
	cls_direction<float> trDir(v_vertices[3]-v_vertices[0], v_vertices[4]-v_vertices[1], v_vertices[5]-v_vertices[2]);

	/*unsigned int nInters;*/
	std::vector< cls_cartesian_point<float> > interPoints;

	this->Intersect(&stPoint, &trDir/*, &nInters*/, interPoints);

	unsigned int i=0;
	std::vector< cls_cartesian_point<float> >::const_iterator iter;
	for (iter = interPoints.begin(); iter != interPoints.end(); ++iter)
	{
		v_vertices[3*(2+i)+0] = (*iter).mCoord[0];
		v_vertices[3*(2+i)+1] = (*iter).mCoord[1];
		v_vertices[3*(2+i)+2] = (*iter).mCoord[2];
		i++;
	}

	p_model->AppendPoints((2+i), v_vertices);
	p_model->AppendWires(1, v_wires);
	p_model->SetConstructed();

	delete [] v_vertices;
	delete [] v_wires;
}

void cls_bezier_spline::GetPoint(float p_param, float* o_coord) const
{
/*
	switch (mOrder) {
		case 1:
			for (unsigned int v_index = 0; v_index<3; v_index++) {
				o_coord[v_index] = (1.-p_param) * mControlPoints[0].mCoord[v_index] +
				                   p_param      * mControlPoints[1].mCoord[v_index];
			}
		break;
		case 2:
			for (unsigned int v_index = 0; v_index<3; v_index++) {
				o_coord[v_index] = (1.-p_param)*(1.-p_param) * mControlPoints[0].mCoord[v_index] +
				                   2.*(1.-p_param)*p_param   * mControlPoints[1].mCoord[v_index] +
				                   p_param*p_param           * mControlPoints[2].mCoord[v_index];
			}
		break;
		case 3:
			for (unsigned int v_index = 0; v_index<3; v_index++) {
				o_coord[v_index] = (1.-p_param)*(1.-p_param)*(1.-p_param) * mControlPoints[0].mCoord[v_index] +
				                   3.*(1.-p_param)*(1.-p_param)*p_param   * mControlPoints[1].mCoord[v_index] +
				                   3.*(1.-p_param)*p_param*p_param        * mControlPoints[2].mCoord[v_index] +
				                   p_param*p_param*p_param                * mControlPoints[3].mCoord[v_index];
			}
		break;
		default:
			LOG(ERROR) << "NOT IMPLEMENTED" << cls_logger::endl;
			break;
	}
*/
	float denom = 0.f;
	for (unsigned int iT = 0; iT <= mOrder; iT++) {
		mCache[iT] = BernsteinCoef(p_param, mOrder, iT) * mWeights[iT];
		denom += mCache[iT];
	}
	for (unsigned int v_index = 0; v_index<3; v_index++) {
		float nom = 0.f;
		for (unsigned int i=0; i<=mOrder; i++) {
			nom += mCache[i] * mControlPoints[i].mCoord[v_index];
		}
		o_coord[v_index] = nom/denom;
	}
}

void cls_bezier_spline::GetPoint(float p_param, cls_cartesian_point<float>* o_point) const
{
	float v_point[3];
	this->GetPoint(p_param, v_point);
	o_point->mCoord[0] = v_point[0];
	o_point->mCoord[1] = v_point[1];
	o_point->mCoord[2] = v_point[2];
}

template <typename TYPE>
void cls_bezier_spline::Intersect(cls_cartesian_point<TYPE>* p_start,
                                  cls_direction<TYPE>* p_dir,
                                  /*unsigned int* o_nSolutions,*/
                                  std::vector< cls_cartesian_point<float> >& o_inters) const
{
	//TODO research code here.

	if (mOrder == 1) {
		// Analytical solution for the 1st-order equation (line)
		TYPE xd = mControlPoints[1].mCoord[0] - mControlPoints[0].mCoord[0];
		TYPE yd = mControlPoints[1].mCoord[1] - mControlPoints[0].mCoord[1];

		TYPE nom = yd * (p_start->mCoord[0] - mControlPoints[0].mCoord[0]) / xd +
		            mControlPoints[0].mCoord[1] - p_start->mCoord[1];
		TYPE denom = p_dir->mComponent[1] - yd * p_dir->mComponent[0] / xd;

		TYPE q = nom/denom;

		//TYPE t = (p_start->mCoord[0] + q * p_dir->mComponent[0] - mControlPoints[0].mCoord[0]) / xd;
		//LOG(INFO) << "Intersection: t=" << t << cls_logger::endl;

		cls_cartesian_point<TYPE> v_inter1(p_start->mCoord[0] + q * p_dir->mComponent[0],
		                                   p_start->mCoord[1] + q * p_dir->mComponent[1],
		                                   p_start->mCoord[2] + q * p_dir->mComponent[2]);

		o_inters.push_back(v_inter1);

	} else if (mOrder == 2) {

		//cls_cartesian_point v_d = mControlPoints[0] - static_cast<TYPE>(2.)*mControlPoints[1] + mControlPoints[2];

		// This is supposed to be a little bit more efficient

		// d = p0 - 2*p1 + p2
		cls_cartesian_point<TYPE> v_d(mControlPoints[0]);
		v_d -= static_cast<TYPE>(2.)*mControlPoints[1];
		v_d += mControlPoints[2];

		// e = 2*p1 - 2*p0
		cls_cartesian_point<TYPE> v_e(static_cast<TYPE>(2.)*mControlPoints[1]);
		v_e -= static_cast<TYPE>(2.)*mControlPoints[0];

		// g = p0 - a
		cls_cartesian_point<TYPE> v_g(mControlPoints[0]);
		v_g -= *p_start;


		LOG(INFO) << "p0:\t";
		mControlPoints[0].Dump();
		LOG(INFO) << "p1:\t";
		mControlPoints[1].Dump();
		LOG(INFO) << "p2:\t";
		mControlPoints[2].Dump();
		LOG(INFO) << " a:\t";
		p_start->Dump();

		LOG(INFO) << " d:\t";
		v_d.Dump();
		LOG(INFO) << " e:\t";
		v_e.Dump();
		LOG(INFO) << " g:\t";
		v_g.Dump();

		TYPE A = -v_e.mCoord[0];
		TYPE K = static_cast<TYPE>(2.)*v_d.mCoord[0];
		TYPE F = v_e.mCoord[0]*v_e.mCoord[0] - static_cast<TYPE>(4.)*v_d.mCoord[0]*v_g.mCoord[0];
		TYPE J = static_cast<TYPE>(4.)*v_d.mCoord[0]*p_dir->mComponent[0];

		TYPE M = - (A*A + F)*v_d.mCoord[1] - A*K*v_e.mCoord[1] - K*K*v_g.mCoord[1];
		TYPE N = static_cast<TYPE>(2.)*A*v_d.mCoord[1] + K*v_e.mCoord[1];
		TYPE S = K*K*p_dir->mComponent[1] - J*v_d.mCoord[1];

		TYPE alpha = S*S;
		TYPE beta = static_cast<TYPE>(2.)*S*M - J*N*N;
		TYPE gamma = M*M - F*N*N;

		TYPE delta2 = beta*beta - static_cast<TYPE>(4.)*alpha*gamma;

		TYPE q1 = (-beta + std::sqrt(delta2)) / (static_cast<TYPE>(2.)*alpha);
		TYPE q2 = (-beta - std::sqrt(delta2)) / (static_cast<TYPE>(2.)*alpha);

		cls_cartesian_point<TYPE> v_inter1(p_start->mCoord[0] + q1 * p_dir->mComponent[0],
		                                   p_start->mCoord[1] + q1 * p_dir->mComponent[1],
		                                   p_start->mCoord[2] + q1 * p_dir->mComponent[2]);

		cls_cartesian_point<TYPE> v_inter2(p_start->mCoord[0] + q2 * p_dir->mComponent[0],
		                                   p_start->mCoord[1] + q2 * p_dir->mComponent[1],
		                                   p_start->mCoord[2] + q2 * p_dir->mComponent[2]);

		o_inters.push_back(v_inter1);
		o_inters.push_back(v_inter2);

		/*TYPE t1 = (A + std::sqrt(F+J*q1)) / K;
		TYPE t2 = (A - std::sqrt(F+J*q2)) / K;
		this->GetPoint(t1, o_inter);*/
	}
}