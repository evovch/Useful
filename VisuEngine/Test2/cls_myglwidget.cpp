#include "cls_myglwidget.h"

#include <QDebug>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "cls_displaymodel.h"

//TODO check - initialize all data members
cls_myGLwidget::cls_myGLwidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    // No OpenGL resource initialization is done here. (Qt docu)

    qDebug().nospace() << "cls_myGLwidget::cls_myGLwidget()";

    mShaderVpassthrought = nullptr;
    mShaderVwire = nullptr;
    mShaderGshading = nullptr;
    mShaderGwire = nullptr;
    mShaderFflat = nullptr;

    mProgShading = nullptr;
    mProgWire = nullptr;

    mVAO = nullptr;
    mVBO = nullptr;
    mIBOshading = nullptr;
    mIBOwire = nullptr;

    //mMVPshadingUniform = ?;
    //mMVPwireUniform = ?;

    mModel = nullptr;
}

cls_myGLwidget::~cls_myGLwidget()
{
    qDebug().nospace() << "cls_myGLwidget::~cls_myGLwidget()";

    // Make sure the context is current and then explicitly
    // destroy all underlying OpenGL resources.
    this->makeCurrent(); // method of QOpenGLWidget (mother) class

    delete mShaderVpassthrought;
    delete mShaderVwire;
    delete mShaderGshading;
    delete mShaderGwire;
    delete mShaderFflat;

    delete mProgShading;
    delete mProgWire;

    delete mVAO;
    delete mVBO;
    delete mIBOshading;
    delete mIBOwire;

    delete mModel;
}

void cls_myGLwidget::initializeGL()
{
    qDebug().nospace() << "cls_myGLwidget::initializeGL()";

    this->makeCurrent(); // method of QOpenGLWidget (mother) class

    this->initializeOpenGLFunctions(); // method of QOpenGLFunctions_4_5_Core (mother) class

    this->InitProgramsAndShaders(); // method of this class cls_myGLwidget

    this->InitBuffers(); // method of this class cls_myGLwidget

    mModel = new cls_DisplayModel();

    mModel->SendToGPU();
}

void cls_myGLwidget::resizeGL(int w, int h)
{
    qDebug().nospace() << "cls_myGLwidget::resizeGL(" << w << ", " << h << ")";
}

void cls_myGLwidget::paintGL()
{
    qDebug().nospace() << "cls_myGLwidget::paintGL()";
    mModel->Draw();
}

void cls_myGLwidget::InitProgramsAndShaders(void)
{
    qDebug().nospace() << "cls_myGLwidget::InitProgramsAndShaders()";

    // Create shaders
    mShaderVpassthrought = new QOpenGLShader(QOpenGLShader::Vertex);
    mShaderVwire = new QOpenGLShader(QOpenGLShader::Vertex);
    mShaderGshading = new QOpenGLShader(QOpenGLShader::Geometry);
    mShaderGwire = new QOpenGLShader(QOpenGLShader::Geometry);
    mShaderFflat = new QOpenGLShader(QOpenGLShader::Fragment);

    // Compile shadres' source files
    mShaderVpassthrought->compileSourceFile("shaders/vertSh_passthrough.vp");
    mShaderVwire->compileSourceFile("shaders/vertSh_wire.vp");
    mShaderGshading->compileSourceFile("shaders/geomSh_shading.gp");
    mShaderGwire->compileSourceFile("shaders/geomSh_wire.gp");
    mShaderFflat->compileSourceFile("shaders/frSh_flat.fp");

    // Prepare the program for shading-style rendering
    mProgShading = new QOpenGLShaderProgram();

    mProgShading->addShader(mShaderVpassthrought);
    mProgShading->addShader(mShaderGshading);
    mProgShading->addShader(mShaderFflat);

    mProgShading->link();
    //mProgShading->bind();

    // Connect uniform variables
    mMVPshadingUniform = mProgShading->uniformLocation("MVP");

    // Prepare the program for wire-style rendering
    mProgWire = new QOpenGLShaderProgram();

    mProgWire->addShader(mShaderVwire);
    mProgWire->addShader(mShaderGwire);
    mProgWire->addShader(mShaderFflat);

    mProgWire->link();
    //mProgWire->bind();

    // Connect uniform variables
    mMVPwireUniform = mProgWire->uniformLocation("MVP");
}

void cls_myGLwidget::InitBuffers(void)
{
    qDebug().nospace() << "cls_myGLwidget::InitBuffers()";

    mVAO = new QOpenGLVertexArrayObject();

    mVAO->create();
    if (mVAO->isCreated()) {
        qDebug().nospace() << "Created vertex array object.";
        //mVAO->bind();
    } else {
        qDebug().nospace() << "Error creating vertex array object.";
    }

    mVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    mVBO->create();
    if (mVBO->isCreated()) {
        qDebug().nospace() << "Created vertex buffer object.";
        //mVBO->bind();
    } else {
        qDebug().nospace() << "Error creating vertex buffer object.";
    }

    mIBOshading = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    mIBOshading->create();
    if (mIBOshading->isCreated()) {
        qDebug().nospace() << "Created index buffer object for shading-style rendering.";
        //mIBOshading->bind();
    } else {
        qDebug().nospace() << "Error creating index buffer object for shading-style rendering.";
    }

    mIBOwire = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    mIBOwire->create();
    if (mIBOwire->isCreated()) {
        qDebug().nospace() << "Created index buffer object for wireframe-style rendering.";
        //mIBOwire->bind();
    } else {
        qDebug().nospace() << "Error creating index buffer object for wireframe-style rendering.";
    }
}

void cls_myGLwidget::InitGLparameters(void)
{
    qDebug().nospace() << "cls_myGLwidget::InitGLparameters()";

    // Init culling		//TODO enable/disable
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);

    // Init depth
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    glClearColor(1.0f, 1.0f, 0.9f, 1.0f);
    glClearDepth(1.0f);

    // Set the first vertex of the triangle as the vertex
    // holding the color for the whole triangle for flat shading rendering
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
}
