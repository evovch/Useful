#include "cls_quick_hull.h"

// Project
#include "base/cls_logger.h"

#include "graphics/cls_model.h" //TODO tmp

/*static*/
unsigned int cls_quick_hull::DIMENSIONALITY = 3;

cls_quick_hull::cls_quick_hull()
{
}

cls_quick_hull::~cls_quick_hull()
{
}

/*static*/
std::list<unsigned int> cls_quick_hull::QuickHull(float* p_vertices,
                                                    unsigned int* p_indices,
                                                    unsigned int p_size,
                                                    cls_model* p_model)
{
	// Output list which is going to be filled
	std::list<unsigned int> res;

	if (p_size < 2) {
		LOG(DEBUG3) << "QuickHull: p_size < 2. Aboring." << cls_logger::endl;
		return res;
	}

	// Find the outermost left and right points ===============================
	// Store the indices of the found points
	unsigned int v_minXindex = p_indices[0];
	unsigned int v_maxXindex = p_indices[0];
	for (unsigned int iVertex = 1; iVertex < p_size; iVertex++) {
		unsigned int v_index = p_indices[iVertex];
		if (p_vertices[v_index*DIMENSIONALITY+0] < p_vertices[v_minXindex*DIMENSIONALITY+0]) {
			v_minXindex = v_index;
		}
		if (p_vertices[v_index*DIMENSIONALITY+0] > p_vertices[v_maxXindex*DIMENSIONALITY+0]) {
			v_maxXindex = v_index;
		}
	}

	if (v_minXindex == v_maxXindex) {
		//TODO
		LOG(ERROR) << "Most left and most right points are the same point." << cls_logger::endl;
		exit(EXIT_FAILURE);
	}

	LOG(DEBUG) << "minXindex=" << v_minXindex << "\tmaxXindex=" << v_maxXindex << cls_logger::endl;

	// Store the coordinates of the found points
	glm::fvec2 v_leftPoint(p_vertices[v_minXindex*DIMENSIONALITY+0],
	                       p_vertices[v_minXindex*DIMENSIONALITY+1]);
	glm::fvec2 v_rightPoint(p_vertices[v_maxXindex*DIMENSIONALITY+0],
	                        p_vertices[v_maxXindex*DIMENSIONALITY+1]);

	// For S1 expected size we take S size (S includes S1)
	// For S2 expected size we take S size (S includes S2)
	// This is easy and safe, though not super memory-efficient
	unsigned int* v_S1indices = new unsigned int[p_size];
	unsigned int* v_S2indices = new unsigned int[p_size];
	unsigned int v_S1size = 0;
	unsigned int v_S2size = 0;

	// Fill S1 and S2
	for (unsigned int iVertex = 0; iVertex < p_size; iVertex++)
	{
		unsigned int v_index = p_indices[iVertex];

		//TODO check
		// Skip P and Q
		if (v_index == v_minXindex || v_index == v_maxXindex) { continue; }

		glm::fvec2 v_curPoint(p_vertices[v_index*DIMENSIONALITY+0],
		                      p_vertices[v_index*DIMENSIONALITY+1]);

		// Fill S1 and S2, count S1 and S2 entries
		if (cls_quick_hull::OnTheLeft(v_leftPoint, v_rightPoint, v_curPoint)) {
			// point on the left - from the set S1
			v_S1indices[v_S1size] = v_index;
			v_S1size++;
			//p_model->SetVertexColor(v_index, 0.0f, 0.3f, 1.0f);
		} else {
			// point on the right - from the set S2
			v_S2indices[v_S2size] = v_index;
			v_S2size++;
			//p_model->SetVertexColor(v_index, 0.0f, 0.3f, 0.8f);
		}
	}

	// #FF0000 #FF0000 #FF0000 #FF0000 #FF0000
	res.push_back(v_minXindex);

	if (v_S1size > 0) {
		std::list<unsigned int> v_s1Hull =
			cls_quick_hull::FindHull(p_vertices, v_S1indices, v_S1size, v_minXindex, v_maxXindex, p_model);
		res.splice(res.end(), v_s1Hull);
	}
	res.push_back(v_maxXindex);
	if (v_S2size > 0) {
		std::list<unsigned int> v_s2Hull =
			cls_quick_hull::FindHull(p_vertices, v_S2indices, v_S2size, v_maxXindex, v_minXindex, p_model);
		res.splice(res.end(), v_s2Hull);
	}
	// #FF0000 #FF0000 #FF0000 #FF0000 #FF0000

	delete [] v_S1indices;
	delete [] v_S2indices;

	return res;
}

/*static*/
/*
std::vector<unsigned int> cls_quick_hull::QuickHull2(float* p_vertices,
                                                    unsigned int* p_indices,
                                                    unsigned int p_size,
                                                    cls_model* p_model)
{
	// Step of the coordinates in the input p_vertices array
	unsigned int DIMENSIONALITY = 3;

	std::vector<unsigned int> res;

	// For recursive function
	static unsigned int depth = 0;

	LOG(DEBUG) << "BuildConvex(" << p_size << ") depth=" << depth << cls_logger::endl;

	depth++;
	if (depth == 20) {
		exit(EXIT_FAILURE);
		return res;
	}
	if (p_size <= 2) {
		LOG(DEBUG) << "\tsize <= 2. Aboring." << cls_logger::endl;
		depth--;
		return res;
	}

	// Find the outermost left and right points ===============================
	// Store the indices of the found points
	unsigned int v_minXindex = p_indices[0];
	unsigned int v_maxXindex = p_indices[0];
	for (unsigned int iVertex = 1; iVertex < p_size; iVertex++) {
		unsigned int v_index = p_indices[iVertex];
		if (p_vertices[v_index*DIMENSIONALITY+0] < p_vertices[v_minXindex*DIMENSIONALITY+0]) {
			v_minXindex = v_index;
		}
		if (p_vertices[v_index*DIMENSIONALITY+0] > p_vertices[v_maxXindex*DIMENSIONALITY+0]) {
			v_maxXindex = v_index;
		}
	}

	if (v_minXindex == v_maxXindex) {
		//TODO
		LOG(ERROR) << "Most left and most right points are the same point." << cls_logger::endl;
		exit(EXIT_FAILURE);
	}

	LOG(DEBUG) << "minXindex=" << v_minXindex << "\tmaxXindex=" << v_maxXindex << cls_logger::endl;

	// Store the coordinates of the found points
	glm::fvec2 v_leftPoint(p_vertices[v_minXindex*DIMENSIONALITY+0],
	                       p_vertices[v_minXindex*DIMENSIONALITY+1]);
	glm::fvec2 v_rightPoint(p_vertices[v_maxXindex*DIMENSIONALITY+0],
	                        p_vertices[v_maxXindex*DIMENSIONALITY+1]);

	// Find the farthest points - S1 and S2 edges =============================
	// Init with the first point - minX (left) point
	unsigned int v_farPointIndexL = v_minXindex;
	unsigned int v_farPointIndexR = v_minXindex;
	glm::fvec2 v_curPoint(p_vertices[v_minXindex*DIMENSIONALITY+0],
	                      p_vertices[v_minXindex*DIMENSIONALITY+1]);
	float v_farPointDistL = 0.;
	float v_farPointDistR = 0.;
	// Just to color
	if (cls_quick_hull::OnTheLeft(v_leftPoint, v_rightPoint, v_curPoint)) {
		// S1
		p_model->SetVertexColor(v_farPointIndexL, 0.0f, 0.3f, 1.0f);
		p_model->SetVertexColor(v_farPointIndexR, 0.0f, 0.3f, 1.0f);
	} else {
		// S2
		p_model->SetVertexColor(v_farPointIndexL, 0.0f, 0.3f, 1.0f);
		p_model->SetVertexColor(v_farPointIndexR, 0.0f, 0.3f, 1.0f);
	}

	// For S1 expected size we take S size (S includes S1)
	// For S2 expected size we take S size (S includes S2)
	// This is easy and safe, though not super memory-efficient
	unsigned int* v_S1indices = new unsigned int[p_size];
	unsigned int* v_S2indices = new unsigned int[p_size];
	unsigned int v_S1size = 0;
	unsigned int v_S2size = 0;

	// Search for any point which is farther
	// Reverse order! This way the last found point will have the
	// maximum angle which is required by the algorithm
	for (unsigned int iVertex = p_size; iVertex >= 1; iVertex--)
	{
		unsigned int v_index = p_indices[iVertex-1];
		v_curPoint = glm::fvec2(p_vertices[v_index*DIMENSIONALITY+0],
		                        p_vertices[v_index*DIMENSIONALITY+1]);

		float len = cls_quick_hull::PointToLineDist(v_leftPoint, v_rightPoint, v_curPoint);

		if (len >= v_farPointDistL) {
			// points from S1 will have len >= 0
			v_farPointDistL = len;
			v_farPointIndexL = v_index;
		}
		if (len < v_farPointDistR) {
			// points from S2 will have len < 0
			v_farPointDistR = len;
			v_farPointIndexR = v_index;
		}

		// Fill S1 and S2, count S1 and S2 entries
		if (cls_quick_hull::OnTheLeft(v_leftPoint, v_rightPoint, v_curPoint)) {
			// point on the left - from the set S1
			v_S1indices[v_S1size] = v_index;
			v_S1size++;
			p_model->SetVertexColor(v_index, 0.0f, 0.3f, 1.0f);
		} else {
			// point on the right - from the set S2
			v_S2indices[v_S2size] = v_index;
			v_S2size++;
			p_model->SetVertexColor(v_index, 0.0f, 0.3f, 1.0f);
		}
	}

	if (v_farPointDistL == 0. && v_S1size > 0) {
		LOG(WARNING) << "Left outermost point has len=0." << cls_logger::endl;
	}

	if (v_farPointDistR == 0. && v_S2size > 0) {
		LOG(WARNING) << "Right outermost point has len=0." << cls_logger::endl;
	}

	// Edges of the S1 and S2 sets - the farthest ponts found in the block above
	glm::fvec2 v_S1edge(p_vertices[v_farPointIndexL*DIMENSIONALITY+0],
	                    p_vertices[v_farPointIndexL*DIMENSIONALITY+1]);
	glm::fvec2 v_S2edge(p_vertices[v_farPointIndexR*DIMENSIONALITY+0],
	                    p_vertices[v_farPointIndexR*DIMENSIONALITY+1]);

	// For S11 and S12 expected size we take S1 size (S1 includes both S11 and S12)
	// For S21 and S22 expected size we take S2 size (S2 includes both S21 and S22)
	// This is easy and safe, though not super memory-efficient
	unsigned int* v_S11indices = new unsigned int[v_S1size]; //TODO +2 ?
	unsigned int* v_S12indices = new unsigned int[v_S1size];
	unsigned int v_S11size = 0;
	unsigned int v_S12size = 0;

	for (unsigned int iVertex = v_S1size; iVertex >= 1; iVertex--)
	{
		unsigned int v_index = v_S1indices[iVertex-1];
		v_curPoint = glm::fvec2(p_vertices[v_index*DIMENSIONALITY+0],
		                        p_vertices[v_index*DIMENSIONALITY+1]);

		if (cls_quick_hull::OnTheLeft(v_leftPoint, v_S1edge, v_curPoint)) {
			// S11
			v_S11indices[v_S11size] = v_index;
			v_S11size++;
			p_model->SetVertexColor(v_index, 1.0f, 0.0f, 1.0f);
		}
		if (cls_quick_hull::OnTheLeft(v_S1edge, v_rightPoint, v_curPoint)) {
			// S12
			v_S12indices[v_S12size] = v_index;
			v_S12size++;
			p_model->SetVertexColor(v_index, 0.0f, 1.0f, 1.0f);
		}
	}

	LOG(DEBUG) << "\t\t\t\t\t\t"
	           << "S1 = " << v_S1size
	           << "\tS11= " << v_S11size << "\tS12= " << v_S12size
	           << "\t\t\tS2 = " << v_S2size
	           << cls_logger::endl;

	std::vector<unsigned int> v_S11convex =
		cls_quick_hull::QuickHull2(p_vertices, v_S11indices, v_S11size, p_model);
	std::vector<unsigned int> v_S12convex =
		cls_quick_hull::QuickHull2(p_vertices, v_S12indices, v_S12size, p_model);

	//TODO keep 2 by now
	if (v_S2size > 2) {
		std::vector<unsigned int> v_S2convex =
			cls_quick_hull::QuickHull2(p_vertices, v_S2indices, v_S2size, p_model);
	}

	delete [] v_S11indices;
	delete [] v_S12indices;
	delete [] v_S1indices;
	delete [] v_S2indices;

	depth--;
	return res;
}
*/

/*static*/
std::list<unsigned int> cls_quick_hull::FindHull(float* p_vertices,
                                   unsigned int* p_Sindices,
                                   unsigned int p_Ssize,
                                   unsigned int p_Pindex,
                                   unsigned int p_Qindex,
                                   cls_model* p_model)
{
	std::list<unsigned int> res;

	// For recursive function
	static unsigned int depth = 0;

	LOG(DEBUG) << "FindHull(" << p_Ssize << ") depth=" << depth << cls_logger::endl;

	depth++;
	if (depth == 10) {
		//exit(EXIT_FAILURE);
		depth--;
		return res;
	}

	if (p_Ssize < 1) {
		LOG(WARNING) << "Empty input set." << cls_logger::endl;
		depth--;
		return res;
	}

	// Store points P and Q for later use
	glm::fvec2 v_Ppoint(p_vertices[p_Pindex*DIMENSIONALITY+0],
	                    p_vertices[p_Pindex*DIMENSIONALITY+1]);
	glm::fvec2 v_Qpoint(p_vertices[p_Qindex*DIMENSIONALITY+0],
	                    p_vertices[p_Qindex*DIMENSIONALITY+1]);

	// Find the farthest point C
	// Init with the first point. Note that P and Q are not inside the input set S
	unsigned int v_farPointIndex = p_Sindices[0];
	glm::fvec2 v_curPoint(p_vertices[v_farPointIndex*DIMENSIONALITY+0],
	                      p_vertices[v_farPointIndex*DIMENSIONALITY+1]);
	float v_farPointDist = cls_quick_hull::PointToLineDist(v_Ppoint, v_Qpoint, v_curPoint);

	// Search for any point which is farther
	// Reverse order! This way the last found point will have the
	// maximum angle which is required by the algorithm
	for (unsigned int iVertex = p_Ssize; iVertex >= 1; iVertex--)
	{
		unsigned int v_index = p_Sindices[iVertex-1];
		v_curPoint = glm::fvec2(p_vertices[v_index*DIMENSIONALITY+0],
		                        p_vertices[v_index*DIMENSIONALITY+1]);

		float len = cls_quick_hull::PointToLineDist(v_Ppoint, v_Qpoint, v_curPoint);

		if (len >= v_farPointDist) {
			// points from S will have len >= 0
			v_farPointDist = len;
			v_farPointIndex = v_index;
		}
	}

	// Store point C for later use
	unsigned int v_Cindex = v_farPointIndex;
	glm::fvec2 v_Cpoint(p_vertices[v_Cindex*DIMENSIONALITY+0],
	                    p_vertices[v_Cindex*DIMENSIONALITY+1]);

	if (v_farPointDist == 0.) {
		LOG(WARNING) << "The outermost point C is on the line PQ" << cls_logger::endl;
		LOG(WARNING) << "P: " << v_Ppoint.x << ", " << v_Ppoint.y << cls_logger::endl;
		LOG(WARNING) << "C: " << v_Cpoint.x << ", " << v_Cpoint.y << cls_logger::endl;
		LOG(WARNING) << "Q: " << v_Qpoint.x << ", " << v_Qpoint.y << cls_logger::endl;
	}


	// For S1 and S2 expected size we take S size (S includes both S1 and S2)
	// This is easy and safe, though not super memory-efficient
	unsigned int* v_S1indices = new unsigned int[p_Ssize];
	unsigned int* v_S2indices = new unsigned int[p_Ssize];
	unsigned int v_S1size = 0;
	unsigned int v_S2size = 0;

	for (unsigned int iVertex = p_Ssize; iVertex >= 1; iVertex--)
	{
		unsigned int v_index = p_Sindices[iVertex-1];

		//TODO check
		// Skip C
		if (v_index == v_Cindex) { continue; }

		v_curPoint = glm::fvec2(p_vertices[v_index*DIMENSIONALITY+0],
		                        p_vertices[v_index*DIMENSIONALITY+1]);

		if (cls_quick_hull::OnTheLeft(v_Ppoint, v_Cpoint, v_curPoint)) {
			// S1
			v_S1indices[v_S1size] = v_index;
			v_S1size++;
			//p_model->SetVertexColor(v_index, 1.0f, 0.0f, 1.0f);
		}
		if (cls_quick_hull::OnTheLeft(v_Cpoint, v_Qpoint, v_curPoint)) {
			// S2
			v_S2indices[v_S2size] = v_index;
			v_S2size++;
			//p_model->SetVertexColor(v_index, 0.0f, 1.0f, 1.0f);
		}
	}

	// #FF0000 #FF0000 #FF0000 #FF0000 #FF0000
	if (v_S1size > 0) {
		std::list<unsigned int> v_s1Hull =
			cls_quick_hull::FindHull(p_vertices, v_S1indices, v_S1size, p_Pindex, v_Cindex, p_model);
		res.splice(res.end(), v_s1Hull);
	}
	res.push_back(v_Cindex);
	if (v_S2size > 0) {
		std::list<unsigned int> v_s2Hull =
			cls_quick_hull::FindHull(p_vertices, v_S2indices, v_S2size, v_Cindex, p_Qindex, p_model);
		res.splice(res.end(), v_s2Hull);
	}
	// #FF0000 #FF0000 #FF0000 #FF0000 #FF0000

	delete [] v_S1indices;
	delete [] v_S2indices;

	depth--;
	return res;
}

/*static*/
float cls_quick_hull::PointToLineDist(glm::fvec2 p_1, glm::fvec2 p_2, glm::fvec2 p_i)
{
	/*LOG(DEBUG) << p_1.x << ",\t" << p_1.y << ",\t" << p_1.z << cls_logger::endl;
	LOG(DEBUG) << p_2.x << ",\t" << p_2.y << ",\t" << p_2.z << cls_logger::endl;
	LOG(DEBUG) << p_i.x << ",\t" << p_i.y << ",\t" << p_i.z << cls_logger::endl;*/

	glm::fvec2 n = p_2 - p_1;
	n = glm::normalize(n);
	glm::fvec2 b = p_1 - p_i; // a - p
	float bn = glm::dot(b, n);
	glm::fvec2 res = b - (bn * n);

	if (cls_quick_hull::OnTheLeft(p_1, p_2, p_i))  {
		return glm::length(res);
	} else {
		return -glm::length(res);
	}
}

/*static*/
bool cls_quick_hull::OnTheLeft(glm::fvec2 p_1, glm::fvec2 p_2, glm::fvec2 p_i)
{
	glm::fvec2 n = p_2 - p_1;
	glm::fvec2 mb = p_i - p_1; // p - a
	glm::fvec3 prod = glm::cross(glm::fvec3(n.x, n.y, 0.0f), glm::fvec3(mb.x, mb.y, 0.0f));
	/*LOG(DEBUG) << "prod.x=" << prod.x << "\tprod.y=" << prod.y
	           << "\tprod.z=" << prod.z << cls_logger::endl;*/

	return (prod.z >= 0.0f) ? true : false;
}
