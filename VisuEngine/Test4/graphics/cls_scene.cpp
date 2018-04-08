#include "cls_scene.h"

// Project
#include "base/cls_logger.h"
#include "cls_model.h"
#include "cls_renderer.h"
#include "cls_offscreen_renderer.h"

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

void cls_scene::SendToGPU(cls_renderer* p_rend, bool p_uniqueColor)
{
	std::vector<cls_model*>::const_iterator iter;

	// Count ======================================================================================

	mTotalNvertices = 0;
	mTotalNtriangles = 0;
	mTotalNwires = 0;
	mTotalNpoints = 0;
	for (iter = mModels.begin(); iter != mModels.end(); ++iter) {
		mTotalNvertices += (*iter)->GetNumOfVertices();
		mTotalNtriangles += (*iter)->GetNumOfTriangles();
		mTotalNwires += (*iter)->GetNumOfWires();
		mTotalNpoints += (*iter)->GetNumOfPoints();
	}

	// Send the first model to GPU ================================================================
	// Here we specify full size of the overall scene (all models)

	iter = mModels.begin();

	LOG(DEBUG) << "Sum:     " << mTotalNvertices << " vertices,\t"
	                          << mTotalNtriangles << " triangles,\t"
	                          << mTotalNwires << " wires,\t"
	                          << mTotalNpoints << " points."
	                          << cls_logger::endl;

	(*iter)->SendToGPUvAndC(p_rend->mVAO, p_rend->mVBO, mTotalNvertices, p_uniqueColor);
	(*iter)->SendToGPUtriangles(p_rend->mIBOshading, mTotalNtriangles);
	(*iter)->SendToGPUwires(p_rend->mIBOwire, mTotalNwires);
	(*iter)->SendToGPUpoints(p_rend->mIBOpoints, mTotalNpoints);

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

		(*iter)->AppendToGPUvAndC(p_rend->mVAO, p_rend->mVBO, v_Nvertices*sizeof(stc_VandC), p_uniqueColor);
		(*iter)->AppendToGPUtriangles(p_rend->mIBOshading, v_Ntriangles*3*sizeof(unsigned int), v_Nvertices);
		(*iter)->AppendToGPUwires(p_rend->mIBOwire, v_Nwires*2*sizeof(unsigned int), v_Nvertices);
		(*iter)->AppendToGPUpoints(p_rend->mIBOpoints, v_Npoints*sizeof(unsigned int), v_Nvertices);

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

void cls_scene::SendToGPUvAndC(cls_renderer* p_rend, bool p_uniqueColor)
{

	std::vector<cls_model*>::const_iterator iter;

	// Count ======================================================================================
	// Probably, we can ship this block, but only if 'normal' SendToGPU() has been called before
	mTotalNvertices = 0;
	for (iter = mModels.begin(); iter != mModels.end(); ++iter) {
		mTotalNvertices += (*iter)->GetNumOfVertices();
	}

	// Send the first model to GPU ================================================================
	// Here we specify full size of the overall scene (all models)

	iter = mModels.begin();

	LOG(DEBUG) << "Sum:     " << mTotalNvertices << " vertices."
	                          << cls_logger::endl;

	(*iter)->SendToGPUvAndC(p_rend->mVAO, p_rend->mVBO, mTotalNvertices, p_uniqueColor);

	unsigned int v_Nvertices = (*iter)->GetNumOfVertices();

	// Append remaining models ====================================================================

	++iter;
	for (; iter != mModels.end(); ++iter) {

		LOG(DEBUG) << "Current: " << v_Nvertices << " vertices."
		                          << cls_logger::endl;

		(*iter)->AppendToGPUvAndC(p_rend->mVAO, p_rend->mVBO, v_Nvertices*sizeof(stc_VandC), p_uniqueColor);

		v_Nvertices += (*iter)->GetNumOfVertices();
	}

	LOG(DEBUG) << "Total:   " << v_Nvertices << " vertices."
	                          << cls_logger::endl;
}

void cls_scene::HighlightTriangle(unsigned int p_index, GLuint p_VAO, GLuint p_VBO) const
{
	//TODO implement!

	LOG(DEBUG2) << "p_index = " << p_index << cls_logger::endl;

	std::vector<unsigned int>::const_iterator mOffsetsIter = mOffsets.begin();
	std::vector<cls_model*>::const_iterator mModelsIter = mModels.begin();

	(*mModelsIter)->HighlightTriangle(p_index, p_VAO, p_VBO);

/*
	unsigned int iModel = 0;
	unsigned int v_trCounter = 0;
	for (; mOffsetsIter != mOffsets.end(); ++mOffsetsIter) {

		//FIXME hack! This relies on the fact that there exactly 3 vertices per triangle
		unsigned int v_curTr = (*mOffsetsIter) / 3;

		LOG(DEBUG2) << iModel << ": " << v_curTr << cls_logger::endl;
		v_trCounter += v_curTr;

		if (p_index < v_trCounter) {
			(*mModelsIter)->HighlightTriangle(p_index, p_VAO, p_VBO);
		}

		iModel++;
		++mModelsIter; // iterator over the models goes in parallel with the iterator over the offsets
	}
*/
}

void cls_scene::Draw(cls_renderer* p_rend) const
{
	LOG(DEBUG3) << "Draw:    " << mTotalNvertices << " vertices,\t"
	                           << mTotalNtriangles << " triangles,\t"
	                           << mTotalNwires << " wires,\t"
	                           << mTotalNpoints << " points."
	                           << cls_logger::endl;

	glUseProgram(p_rend->mShadingDrawProgram);
	glBindVertexArray(p_rend->mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_rend->mIBOshading);
	glDrawElements(GL_TRIANGLES, mTotalNtriangles*3, GL_UNSIGNED_INT, NULL);
	//glBindVertexArray(0);
	//glUseProgram(0);

	glUseProgram(p_rend->mWireDrawProgram);
	//glBindVertexArray(p_rend->mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_rend->mIBOwire);
	glDrawElements(GL_LINES, mTotalNwires*2, GL_UNSIGNED_INT, NULL);
	//glBindVertexArray(0);
	//glUseProgram(0);

	glUseProgram(p_rend->mPointsDrawProgram);
	//glBindVertexArray(p_rend->mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_rend->mIBOpoints);
	glDrawElements(GL_POINTS, mTotalNpoints, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	glUseProgram(0);
}

void cls_scene::Draw(cls_offscreen_renderer* p_rend) const
{
	LOG(DEBUG3) << "Draw off:" << mTotalNvertices << " vertices,\t"
	                           << mTotalNtriangles << " triangles,\t"
	                           << mTotalNwires << " wires,\t"
	                           << mTotalNpoints << " points."
	                           << cls_logger::endl;

	glUseProgram(p_rend->mPickDrawProgram);
	glBindVertexArray(p_rend->mRenderer->mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_rend->mRenderer->mIBOshading);
	glDrawElements(GL_TRIANGLES, mTotalNtriangles*3, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	glUseProgram(0);
}

void cls_scene::AddModel(cls_model* p_model)
{
	mModels.push_back(p_model);
	p_model->SetScene(this, mModels.size()-1);

	unsigned int v_Nvertices = 0;
	/*unsigned int v_Ntriangles = 0;
	unsigned int v_Nwires = 0;
	unsigned int v_Npoints = 0;*/

	std::vector<cls_model*>::const_iterator iter;

	for (iter = mModels.begin(); iter != mModels.end(); ++iter) {
		mOffsets.push_back(v_Nvertices);

		v_Nvertices += (*iter)->GetNumOfVertices();
		/*v_Ntriangles += (*iter)->GetNumOfTriangles();
		v_Nwires += (*iter)->GetNumOfWires();
		v_Npoints += (*iter)->GetNumOfPoints();*/
	}

	LOG(DEBUG3) << "Added a new model into the scene. New total number of models: " << mModels.size() << cls_logger::endl;
}

void cls_scene::Dump(void) const
{
	std::vector<cls_model*>::const_iterator iter;
	for (iter = mModels.begin(); iter != mModels.end(); ++iter) {
		(*iter)->Dump();
	}
}
