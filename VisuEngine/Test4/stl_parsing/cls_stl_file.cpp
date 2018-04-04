#include "cls_stl_file.h"

#include <fstream>

using std::endl;

cls_stl_file::cls_stl_file() :
	mName(""),
	mFirstFacet(nullptr)
{
}

cls_stl_file::~cls_stl_file()
{
}

void cls_stl_file::Export(std::string p_filename) const
{
	//TODO tune output stream such that we do not loose any precision!!!

	std::ofstream f(p_filename);

	f << "solid " << mName << endl;

	struct facet_t* curfacet = mFirstFacet;

	do {
		f << "facet normal " << curfacet->mNormal->mX << " " << curfacet->mNormal->mY << " " << curfacet->mNormal->mZ << endl;

		f << " outer loop" << endl;

		f << "  vertex " << curfacet->mLoop->mV1->mX << " " << curfacet->mLoop->mV1->mY << " " << curfacet->mLoop->mV1->mZ << endl;
		f << "  vertex " << curfacet->mLoop->mV2->mX << " " << curfacet->mLoop->mV2->mY << " " << curfacet->mLoop->mV2->mZ << endl;
		f << "  vertex " << curfacet->mLoop->mV3->mX << " " << curfacet->mLoop->mV3->mY << " " << curfacet->mLoop->mV3->mZ << endl;

		f << " endloop" << endl;

		f << "endfacet" << endl;

	} while ((curfacet = curfacet->GetNext()) != nullptr);

}
