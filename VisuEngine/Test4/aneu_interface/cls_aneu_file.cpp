#include "cls_aneu_file.h"

// Project
#include "graphics/cls_model.h"

cls_aneu_file::cls_aneu_file()
{
}

cls_aneu_file::~cls_aneu_file()
{
}

void cls_aneu_file::Export(std::string p_filename) const
{
}

void cls_aneu_file::BuildModel(cls_model* p_model) const
{
	unsigned int v_nWires = mNtriangles*3;
	unsigned int* v_wires = new unsigned int[v_nWires*2];

	for (unsigned int iTr=0; iTr<mNtriangles; iTr++) {
		// total 3 wires per triangle
		// wire 1
		v_wires[iTr*6 + 0] = mTriangles[iTr*3 + 0];
		v_wires[iTr*6 + 1] = mTriangles[iTr*3 + 1];
		// wire
		v_wires[iTr*6 + 2] = mTriangles[iTr*3 + 1];
		v_wires[iTr*6 + 3] = mTriangles[iTr*3 + 2];
		// wire 3
		v_wires[iTr*6 + 4] = mTriangles[iTr*3 + 2];
		v_wires[iTr*6 + 5] = mTriangles[iTr*3 + 0];
	}

	p_model->AppendPoints(mNnodes, mNodes);
	p_model->AppendTriangles(mNtriangles, mTriangles);
	p_model->AppendWires(v_nWires, v_wires);
	p_model->SetConstructed();
}