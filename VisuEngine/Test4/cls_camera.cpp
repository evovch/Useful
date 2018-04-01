#include "cls_camera.h"

// GLM
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GL/glut.h>

#include <cstdio>

#include "cls_renderer.h"

cls_camera::cls_camera(glm::vec3 p_center, float p_radius) :
    mDefCenter(p_center), mDefRadius(p_radius)
{
    this->Reset();
}

cls_camera::~cls_camera()
{
}

// Nice pure vector/matrix operations, no OpenGL stuff, except the call to the SendCamToGPU()
void cls_camera::Reset(void)
{
    mWinWidth = 800;
    mWinHeight = 600;

    this->SetLookPt(mDefCenter);
    this->SetDist(mDefRadius*2.0f); //TODO why *2?

    mIsPerspective = true;
    this->SetFrAngle(45.0f);
    this->SetFrNearWidth(mDefRadius*1.2f);

    this->SetParallelBoxSize(mDefRadius*1.2f);
    this->SetParallelBoxDepth(mDefRadius*1.2f);

    mQua = glm::quat();

    //this->SendCamToGPU();
}

void cls_camera::Pan(float /*p_curScreenX*/, float /*p_curScreenY*/,
                       float /*p_startScreenX*/, float /*p_startScreenY*/,
                       glm::vec3 /*p_startLookPt*/)
{
    //TODO implement
/*    qDebug() << "cls_myCamera::Pan";
    float k = 0.05;
    float dx = (float)(p_curScreenX-p_startScreenX);
    float dy = (float)(p_curScreenY-p_startScreenY);
    glm::vec3 v_delta(dx*k, dy*k, 0.);
    glm::vec3 v_deltaLocal = v_delta * this->GetRotM();
    mLookPt += v_delta;*/
}

void cls_camera::Center(float /*p_curScreenX*/, float /*p_curScreenY*/)
{
    //TODO implement
}

// Nice pure vector/matrix operations, no OpenGL stuff, except the call to the SendCamToGPU()
void cls_camera::Zoom(float p_curScreenY,
                      float p_prevScreenY,
                      float p_startScreenY,
                      float /*p_startFrAngle*/,
                      float p_startPBS)
{
    //TODO study!
    /*
    qDebug() << "start =" << p_startScreenY
             << "prev  =" << p_prevScreenY
             << "curr  =" << p_curScreenY;
    */

    mDist += (p_curScreenY-p_prevScreenY);
    float v_rad = mDist / 2.;
    this->SetFrNearWidth(v_rad*1.2f);

    // Perspective
    /*
    float v_newAngle = p_startFrAngle - (p_curScreenY-p_startScreenY)*0.02f;
    if (v_newAngle > 0.0f && v_newAngle < 180.0f) {
        this->SetFrAngle(v_newAngle);
    }
    */

    // Parallel
    // PBS - parallel box size
    float v_newPBS = p_startPBS - (p_curScreenY-p_startScreenY)*0.1f;
    this->SetParallelBoxSize(v_newPBS);

    //this->SendCamToGPU();
}

// Nice pure vector/matrix operations, no OpenGL stuff, except the call to the SendCamToGPU()
void cls_camera::Rotate(glm::vec3 p_curLocalDir, glm::vec3 p_startLocalDir)
{
    glm::vec3 v_perpLocalDir = glm::normalize(glm::cross(p_startLocalDir, p_curLocalDir));
    v_perpLocalDir = v_perpLocalDir * this->GetRotM();
    this->mQua = glm::rotate(mQua, glm::angle(p_curLocalDir, p_startLocalDir), v_perpLocalDir);

    //this->SendCamToGPU();
}

// Nice pure vector/matrix operations, no OpenGL stuff
glm::mat3 cls_camera::GetRotM(void) const
{
    glm::mat3 v_M = glm::mat3_cast(mQua);
    return v_M;
}

// Nice pure vector/matrix operations, no OpenGL stuff
glm::mat4 cls_camera::GetModelToCamera(void) const
{
    // Form a model-to-camera matrix from the quaternion and shift vector
    glm::mat3 v_modelToCamera3x3 = this->GetRotM();
    glm::vec3 v_shift(0.0f, 0.0f, -(this->GetDist()));
    glm::vec3 v_gshift = v_shift * v_modelToCamera3x3;

    glm::mat4 v_modelToCamera;
    v_modelToCamera[0].x = v_modelToCamera3x3[0].x;
    v_modelToCamera[0].y = v_modelToCamera3x3[0].y;
    v_modelToCamera[0].z = v_modelToCamera3x3[0].z;
    v_modelToCamera[1].x = v_modelToCamera3x3[1].x;
    v_modelToCamera[1].y = v_modelToCamera3x3[1].y;
    v_modelToCamera[1].z = v_modelToCamera3x3[1].z;
    v_modelToCamera[2].x = v_modelToCamera3x3[2].x;
    v_modelToCamera[2].y = v_modelToCamera3x3[2].y;
    v_modelToCamera[2].z = v_modelToCamera3x3[2].z;
    v_modelToCamera[3].w = 1.0f;

    v_modelToCamera = glm::translate(v_modelToCamera, -(this->GetLookPt()) + v_gshift);

    return v_modelToCamera;
}

glm::mat4 cls_camera::GetCameraToClip(void) const
{
    // Form a camera-to-clip matrix from the frustum

    ////printf("mWinWidth=%d, mWinHeight=%d\n", mWinWidth, mWinHeight);

    // Nasty here - access to the widget
    float aspectRat = (float)mWinWidth/(float)mWinHeight;

    ////printf("aspectRat=%f\n", aspectRat);

    // Nice pure vector/matrix operations, no OpenGL stuff
    glm::mat4 cameraToClip;
    if (mIsPerspective) {
        cameraToClip = glm::infinitePerspective(this->GetFrAngle(), aspectRat,
                                                this->GetDist() - this->GetFrNearWidth());
    } else {
        float paralS = this->GetParallelBoxSize();
        float paralD = this->GetParallelBoxDepth();
        cameraToClip = glm::ortho(-paralS*aspectRat, paralS*aspectRat, -paralS, paralS,
                                  this->GetDist()-paralD, this->GetDist()+paralD);
    }

    return cameraToClip;
}

// Nice pure vector/matrix operations, no OpenGL stuff
glm::mat4 cls_camera::GetMVP(void) const
{
    glm::mat4 v_modelToCamera = this->GetModelToCamera();
    glm::mat4 v_cameraToClip = this->GetCameraToClip();

    // Form the final matrix
    glm::mat4 v_MVP = v_cameraToClip * v_modelToCamera;

    return v_MVP;
}

// Nice pure vector/matrix operations, no OpenGL stuff
/*
glm::vec3 cls_camera::GetViewerPoint(void) const
{
    glm::vec3 v_lookDirInCamSpace(0.0f, 0.0f, 1.0f);
    glm::vec3 v_lookDirInModelSpace;

    v_lookDirInModelSpace = glm::inverse(this->GetRotM()) * v_lookDirInCamSpace;

    return (this->GetLookPt() + this->GetDist() * v_lookDirInModelSpace);
}
*/

/*
void cls_camera::SendCamToGPU(std::vector<GLuint> p_programs, std::vector<GLuint> p_uniforms) //const
{
    //qDebug() << "cls_myCamera::SendCamToGPU";
    //TODO implement

    // Nasty here - both access to the widget and OpenGL functions
    glm::mat4 v_MVP = this->GetMVP();

	std::vector<GLuint>::iterator v_iterP = p_programs.begin(); // over programs
	std::vector<GLuint>::iterator v_iterU = p_uniforms.begin(); // over uniforms

	for ( ; v_iterP != p_programs.end(); ) {
		glUseProgram(*v_iterP);
		glUniformMatrix4fv(*v_iterU, 1, GL_FALSE, glm::value_ptr(v_MVP));

		++v_iterP;
		++v_iterU;
	}
    glUseProgram(0);
}
*/

void cls_camera::SendCamToGPU(cls_renderer* p_renderer)
{
    glm::mat4 v_MVP = this->GetMVP();

    std::vector<GLuint>::iterator v_iterP = p_renderer->mProgs.begin(); // over programs
    std::vector<GLuint>::iterator v_iterU = p_renderer->mUnifs.begin(); // over uniforms

    for ( ; v_iterP != p_renderer->mProgs.end(); ) {
        glUseProgram(*v_iterP);
        glUniformMatrix4fv(*v_iterU, 1, GL_FALSE, glm::value_ptr(v_MVP));

        ++v_iterP;
        ++v_iterU;
    }
    glUseProgram(0);
}
