#pragma once

#include <QOpenGLWidget> // mother class
#include <QOpenGLFunctions_4_5_Core>

class QOpenGLVertexArrayObject;
class QOpenGLBuffer;
class QOpenGLShader;
class QOpenGLShaderProgram;

class cls_DisplayModel;

class cls_myGLwidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public: // methods

    cls_myGLwidget(QWidget *parent);
    ~cls_myGLwidget();

protected: // methods

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private: // methods

    void InitProgramsAndShaders(void);
    void InitBuffers(void);
    void InitGLparameters(void);

private: // data members

    // Shaders
    QOpenGLShader* mShaderVpassthrought; // Vertex shader (for shading-style rendering) - the simplest vertex shader
    QOpenGLShader* mShaderVwire; // Vertex shader for wireframe-style rendering
    QOpenGLShader* mShaderGshading; // Geometry shader for shading-style rendering
    QOpenGLShader* mShaderGwire; // Geometry shader for wireframe-style rendering
    QOpenGLShader* mShaderFflat; // Fragment shader for both shading-style and wireframe-style rendering

    // Program for shading-style rendering
    QOpenGLShaderProgram* mProgShading;
    // Program for wireframe-style rendering
    QOpenGLShaderProgram* mProgWire;

    // Vertex array object
    QOpenGLVertexArrayObject* mVAO;
    // Vertex buffer object
    QOpenGLBuffer* mVBO;
    // Index buffer object for shading-style rendering
    QOpenGLBuffer* mIBOshading;
    // Index buffer object for wireframe-style rendering
    QOpenGLBuffer* mIBOwire;

    // Uniform variables for matrices
    GLuint mMVPshadingUniform;
    GLuint mMVPwireUniform;

    cls_DisplayModel* mModel;

};
