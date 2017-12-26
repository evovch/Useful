#pragma once

#include "glm/glm.hpp"

#include <QOpenGLWidget> // mother class
#include <QOpenGLFunctions_4_5_Core>

#include "support.h" // for action_type enum

class QOpenGLVertexArrayObject;
class QOpenGLBuffer;
class QOpenGLShader;
class QOpenGLShaderProgram;
class QOpenGLDebugLogger;

class cls_myCamera;
class cls_DisplayModel;

class cls_myGLwidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    friend class cls_myCamera;
    friend class cls_DisplayModel;

public: // methods

    cls_myGLwidget(QWidget *parent);
    ~cls_myGLwidget();

    int GetWinW(void) const { return this->width(); } // method of mother-mother class QWidget
    int GetWinH(void) const { return this->height(); } // method of mother-mother class QWidget
    int GetMinWinDim(void) const { return (this->height() < this->width()) ? this->height() : this->width(); }
    float GetSphR(void) const { return (cls_myGLwidget::mCentralCircleK * this->GetMinWinDim() / 2.0f); }

    void SetModel(cls_DisplayModel* v_model);

public: // data members

    static float mCentralCircleK;

protected: // methods

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected: // methods

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
#endif
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private: // methods

    void InitProgramsAndShaders(void);
    void InitBuffers(void);
    void InitGLparameters(void);

private: // data members

    // Shaders
    QOpenGLShader* mShaderVshading; // Vertex shader for shading-style rendering
    QOpenGLShader* mShaderGshading; // Geometry shader for shading-style rendering
    QOpenGLShader* mShaderFshading; // Fragment shader for shading-style rendering

    QOpenGLShader* mShaderVwire; // Vertex shader for wireframe-style rendering
    QOpenGLShader* mShaderGwire; // Geometry shader for wireframe-style rendering
    QOpenGLShader* mShaderFwire; // Fragment shader for wireframe-style rendering

    QOpenGLShader* mShaderVpoints; // Vertex shader for points rendering
    QOpenGLShader* mShaderGpoints; // Geometry shader for points rendering
    QOpenGLShader* mShaderFpoints; // Fragment shader for points rendering

    // Program for shading-style rendering
    QOpenGLShaderProgram* mProgShading;
    // Program for wireframe-style rendering
    QOpenGLShaderProgram* mProgWire;
    // Program for points rendering
    QOpenGLShaderProgram* mProgPoints;

    // Vertex array object
    QOpenGLVertexArrayObject* mVAO;
    // Vertex buffer object
    QOpenGLBuffer* mVBO;
    // Index buffer object for shading-style rendering
    QOpenGLBuffer* mIBOshading;
    // Index buffer object for wireframe-style rendering
    QOpenGLBuffer* mIBOwire;
    // Index buffer object for points rendering
    QOpenGLBuffer* mIBOpoints;

    // Uniform variables for matrices
    GLuint mMVPshadingUniform;
    GLuint mMVPwireUniform;
    GLuint mMVPpointsUniform;

    // OpenGL logger for easier debug
    QOpenGLDebugLogger* mOpenGLlogger;

    cls_myCamera* mCamera;
    cls_DisplayModel* mModel;

    // Auxiliary for camera manipulation
    action_type mCurrentAction;
    float mStartXa;  // At mouse press
    float mStartYa;  // At mouse press
    glm::vec3 mStartLocalDir;   // Rotating
    glm::vec3 mStartLookPt;     // Panning
    float mStartFrAngle;        // Zooming (perspective)
    float mStartPBS;            // Zooming (parallel), PBS - parallel box size

};
