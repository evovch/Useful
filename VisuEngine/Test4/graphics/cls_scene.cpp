#include "cls_scene.h"

// Project
#include "base/cls_logger.h"
#include "cls_model.h"
#include "cls_renderer.h"

cls_scene::cls_scene() :
	 mTotalNvertices(0),
	 mTotalNtriangles(0),
	 mTotalNwires(0),
	 mTotalNpoints(0)
{
}

cls_scene::~cls_scene()
{
}

void cls_scene::SendToGPU(cls_renderer* p_rend)
{
	mTotalNvertices = 0;
	mTotalNtriangles = 0;
	mTotalNwires = 0;
	mTotalNpoints = 0;

	std::vector<cls_model*>::const_iterator iter;

	// Count ======================================================================================

	for (iter = mModels.begin(); iter != mModels.end(); ++iter) {
		mTotalNvertices += (*iter)->GetNumOfVertices();
		mTotalNtriangles += (*iter)->GetNumOfTriangles();
		mTotalNwires += (*iter)->GetNumOfWires();
		mTotalNpoints += (*iter)->GetNumOfPoints();
	}

	// Send the first model to GPU ================================================================
	// Here we specify full size of the overall scene (all models)

	iter = mModels.begin();

	LOG(DEBUG) << "Current: " << mTotalNvertices << " vertices,\t"
	                          << mTotalNtriangles << " triangles,\t"
	                          << mTotalNwires << " wires,\t"
	                          << mTotalNpoints << " points."
	                          << cls_logger::endl;

	(*iter)->SendToGPUvAndC(p_rend->mVAO, p_rend->mVBO, mTotalNvertices);
	(*iter)->SendToGPUtriangles(p_rend->mIBOshading, mTotalNtriangles);
	(*iter)->SendToGPUwires(p_rend->mIBOwire, mTotalNwires);
	(*iter)->SendToGPUpoints(p_rend->mIBOpoints, mTotalNpoints);

	mOffsets.push_back(0);

	unsigned int v_Nvertices = (*iter)->GetNumOfVertices();
	unsigned int v_Ntriangles = (*iter)->GetNumOfTriangles();
	unsigned int v_Nwires = (*iter)->GetNumOfWires();
	unsigned int v_Npoints = (*iter)->GetNumOfPoints();

	// Append remaining models ====================================================================

	++iter;
	for (; iter != mModels.end(); ++iter) {

		LOG(DEBUG) << "Current: " << v_Nvertices << " vertices,\t"
		                          << v_Ntriangles << " triangles,\t"
		                          << v_Nwires << " wires,\t"
		                          << v_Npoints << " points."
		                          << cls_logger::endl;

		(*iter)->AppendToGPUvAndC(p_rend->mVAO, p_rend->mVBO, v_Nvertices*sizeof(stc_VandC));
		(*iter)->AppendToGPUtriangles(p_rend->mIBOshading, v_Ntriangles*3*sizeof(unsigned int), v_Nvertices);
		(*iter)->AppendToGPUwires(p_rend->mIBOwire, v_Nwires*2*sizeof(unsigned int), v_Nvertices);
		(*iter)->AppendToGPUpoints(p_rend->mIBOpoints, v_Npoints*sizeof(unsigned int), v_Nvertices);

		mOffsets.push_back(v_Nvertices);

		v_Nvertices += (*iter)->GetNumOfVertices();
		v_Ntriangles += (*iter)->GetNumOfTriangles();
		v_Nwires += (*iter)->GetNumOfWires();
		v_Npoints += (*iter)->GetNumOfPoints();
	}

	LOG(DEBUG) << "Total:   " << v_Nvertices << " vertices,\t"
	                          << v_Ntriangles << " triangles,\t"
	                          << v_Nwires << " wires,\t"
	                          << v_Npoints << " points."
	                          << cls_logger::endl;
}

void cls_scene::Draw(cls_renderer* p_rend) const
{
	LOG(DEBUG) << "Draw:    " << mTotalNvertices << " vertices,\t"
                              << mTotalNtriangles << " triangles,\t"
                              << mTotalNwires << " wires,\t"
                              << mTotalNpoints << " points."
                              << cls_logger::endl;

	glUseProgram(p_rend->mShadingDrawProgram);
	glBindVertexArray(p_rend->mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_rend->mIBOshading);
	glDrawElements(GL_TRIANGLES, mTotalNtriangles*3, GL_UNSIGNED_INT, NULL);
	//glBindVertexArray(0);
	glUseProgram(0);

	glUseProgram(p_rend->mWireDrawProgram);
	//glBindVertexArray(p_rend->mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_rend->mIBOwire);
	glDrawElements(GL_LINES, mTotalNwires*2, GL_UNSIGNED_INT, NULL);
	//glBindVertexArray(0);
	glUseProgram(0);

	glUseProgram(p_rend->mPointsDrawProgram);
	//glBindVertexArray(p_rend->mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_rend->mIBOpoints);
	glDrawElements(GL_POINTS, mTotalNpoints, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	glUseProgram(0);
}

void cls_scene::AddModel(cls_model* p_model)
{
	mModels.push_back(p_model);
	p_model->SetScene(this, mModels.size()-1);
	LOG(DEBUG3) << "Added a new model into the scene. New total number of models: " << mModels.size() << cls_logger::endl;
}
