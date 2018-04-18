#include "cls_aneu_interface.h"

// STD
#include <fstream>

// Project
#include "base/cls_logger.h"
#include "cls_aneu_file.h"

cls_aneu_interface::cls_aneu_interface()
{
}

cls_aneu_interface::~cls_aneu_interface()
{
}

//TODO develop
/* static */
cls_aneu_file* cls_aneu_interface::Import(const char* p_filename)
{
	std::ifstream f(p_filename);

	if (!f.is_open()) {
		LOG(ERROR) << "Failed to open file '" << p_filename << "'" << cls_logger::endl;
		return nullptr;
	}

	cls_aneu_file* v_resAneuFile = new cls_aneu_file();

	unsigned int v_nNodes;
	unsigned int v_nElements;
	unsigned int v_nTriangles;
	float x, y, z;
	unsigned int d, n1, n2, n3, n4;
	unsigned int e1, e2;

	f >> v_nNodes;
	v_resAneuFile->mNnodes = v_nNodes;
	v_resAneuFile->mNodes = new float[v_nNodes*3];

	for (unsigned int i=0; i<v_nNodes; i++) {
		f >> x >> y >> z;
		v_resAneuFile->mNodes[i*3+0] = x;
		v_resAneuFile->mNodes[i*3+1] = y;
		v_resAneuFile->mNodes[i*3+2] = z;
	}

	f >> v_nElements;
	v_resAneuFile->mNelements = v_nElements;
	v_resAneuFile->mElements = new unsigned int[v_nElements*5];

	for (unsigned int i=0; i<v_nElements; i++) {
		f >> d >> n1 >> n2 >> n3 >> n4;
		v_resAneuFile->mElements[i*5+0] = d;
		v_resAneuFile->mElements[i*5+1] = n1;
		v_resAneuFile->mElements[i*5+2] = n2;
		v_resAneuFile->mElements[i*5+3] = n3;
		v_resAneuFile->mElements[i*5+4] = n4;
	}

	f >> v_nTriangles;
	v_resAneuFile->mNtriangles = v_nTriangles;
	v_resAneuFile->mTriangles = new unsigned int[v_nTriangles*3];
	v_resAneuFile->mTriangAttr = new unsigned int[v_nTriangles*3];

	for (unsigned int i=0; i<v_nTriangles; i++) {
		f >> d >> n1 >> n2 >> n3 >> e1 >> e2;
		v_resAneuFile->mTriangles[i*3+0] = n1 - 1;
		v_resAneuFile->mTriangles[i*3+1] = n2 - 1;
		v_resAneuFile->mTriangles[i*3+2] = n3 - 1;
		v_resAneuFile->mTriangAttr[i*3+0] = d;
		v_resAneuFile->mTriangAttr[i*3+1] = e1;
		v_resAneuFile->mTriangAttr[i*3+2] = e2;
	}

	LOG(INFO) << v_nNodes << " nodes, "
	          << v_nElements << " elements, "
	          << v_nTriangles << " triangles."
	          << cls_logger::endl;

	f.close();

	return v_resAneuFile;
}
