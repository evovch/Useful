#include "cls_myglwidget.h"

#include <QDebug>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram> // this header contains also QOpenGLShader class definition
#include <QOpenGLDebugLogger>
#include <QMouseEvent>

#include "cls_mycamera.h"
#include "cls_displaymodel.h"

float cls_myGLwidget::mCentralCircleK = 0.8f;

//TODO check - initialize all data members
cls_myGLwidget::cls_myGLwidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    //// No OpenGL resource initialization is done here. (Qt docu)

    qDebug().nospace() << "cls_myGLwidget::cls_myGLwidget()";

    //// Shaders
    mShaderVshading = nullptr;
    mShaderGshading = nullptr;
    mShaderFshading = nullptr;

    mShaderVwire = nullptr;
    mShaderGwire = nullptr;
    mShaderFwire = nullptr;

    mShaderVpoints = nullptr;
    mShaderGpoints = nullptr;
    mShaderFpoints = nullptr;

    //// Programs
    mProgShading = nullptr;
    mProgWire = nullptr;
    mProgPoints = nullptr;

    //// Buffers
    mVAO = nullptr;
    mVBO = nullptr;
    mIBOshading = nullptr;
    mIBOwire = nullptr;
    mIBOpoints = nullptr;

    //mMVPshadingUniform = ?;
    //mMVPwireUniform = ?;
    //mMVPpointsUniform = ?;

    mCamera = nullptr;
    mModel = nullptr;

    mCurrentAction = ACT_NO_ACT;
}

cls_myGLwidget::~cls_myGLwidget()
{
    qDebug().nospace() << "cls_myGLwidget::~cls_myGLwidget()";

    //// Make sure the context is current and then explicitly
    //// destroy all underlying OpenGL resources.
    this->makeCurrent(); //// method of QOpenGLWidget (mother) class

    //// Shaders
    delete mShaderVshading;
    delete mShaderGshading;
    delete mShaderFshading;

    delete mShaderVwire;
    delete mShaderGwire;
    delete mShaderFwire;

    delete mShaderVpoints;
    delete mShaderGpoints;
    delete mShaderFpoints;

    //// Programs
    delete mProgShading;
    delete mProgWire;
    delete mProgPoints;

    //// Buffers
    delete mVAO;
    delete mVBO;
    delete mIBOshading;
    delete mIBOwire;
    delete mIBOpoints;

    delete mOpenGLlogger;

    delete mCamera;
    ////delete mModel; //// The display model should be an external object
}

void cls_myGLwidget::SetModel(cls_DisplayModel* v_model)
{
    mModel = v_model;
    mModel->SendToGPU();
}

// ------------------------------------------------------------------------------------------------

void cls_myGLwidget::initializeGL()
{
    qDebug().nospace() << "cls_myGLwidget::initializeGL()";

    this->makeCurrent(); //// method of QOpenGLWidget (mother) class
    this->initializeOpenGLFunctions(); //// method of QOpenGLFunctions_4_5_Core (mother) class
    this->InitProgramsAndShaders(); //// method of this class cls_myGLwidget
    this->InitBuffers(); //// method of this class cls_myGLwidget
    this->InitGLparameters(); //// method of this class cls_myGLwidget

    //QOpenGLContext* v_ctx = QOpenGLContext::currentContext();
    mOpenGLlogger = new QOpenGLDebugLogger(this);
    mOpenGLlogger->initialize(); //// initializes in the current context, i.e. v_ctx

    glm::vec3 v_center(0., 0., 0.);
    float v_radius = 2.;
    mCamera = new cls_myCamera(v_center, v_radius, this);
}

void cls_myGLwidget::resizeGL(int w, int h)
{
    ////qDebug().nospace() << "cls_myGLwidget::resizeGL(" << w << ", " << h << ")";
    ////qDebug().nospace() << "QWidget->width(): " << this->width();
    ////qDebug().nospace() << "QWidget->height(): " << this->height();

    mCamera->SendCamToGPU();
    glViewport(0, 0, w, h); //TODO why do we need that?
}

void cls_myGLwidget::paintGL()
{
    ////qDebug().nospace() << "cls_myGLwidget::paintGL()";

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mModel) {
        mModel->Draw();
    }

    const QList<QOpenGLDebugMessage> v_messages = mOpenGLlogger->loggedMessages();
    for (const QOpenGLDebugMessage &v_message : v_messages) {
        qDebug() << v_message;
    }
}

// ------------------------------------------------------------------------------------------------

void cls_myGLwidget::mousePressEvent(QMouseEvent* event)
{
    //qDebug() << "cls_myGLwidget::mousePressEvent";

    //// Current values
    float v_xa = (float)(event->x() - this->GetWinW()/2);  //// local coordinates (from screen center)
    float v_ya = -(float)(event->y() - this->GetWinH()/2); //// local coordinates (from screen center)
    float v_curR = sqrt(v_xa*v_xa + v_ya*v_ya);
    float v_sphR = this->GetSphR();
    float v_za = sqrt(v_sphR*v_sphR - v_xa*v_xa - v_ya*v_ya);

    //// Save values at mouse press
    mStartXa = v_xa;
    mStartYa = v_ya;
    mStartFrAngle = mCamera->GetFrAngle();
    mStartPBS = mCamera->GetParallelBoxSize();
    mStartLookPt = mCamera->GetLookPt();

    if (event->button() == Qt::LeftButton) {
        //// Check: if we are inside the central circle area - rotate, otherwise - tilt
        if (v_curR > v_sphR) {
            //// TILT
            mStartLocalDir = glm::normalize(glm::vec3(v_xa, v_ya, 0.0f));
            mCurrentAction = ACT_TILT;
        } else {
            //// ROTATE
            mStartLocalDir = glm::normalize(glm::vec3(v_xa, v_ya, v_za));
            mCurrentAction = ACT_ROTATE;
        }
    } else if (event->button() == Qt::RightButton) {
        mCurrentAction = ACT_ZOOM;
    } else if (event->button() == Qt::MiddleButton) {
        mCurrentAction = ACT_PAN;
    } else {

    }

    this->setMouseTracking(true);
}

void cls_myGLwidget::mouseReleaseEvent(QMouseEvent* /*event*/)
{
    //qDebug() << "cls_myGLwidget::mouseReleaseEvent";
    mCurrentAction = ACT_NO_ACT;
    this->setMouseTracking(false);
}

void cls_myGLwidget::mouseDoubleClickEvent(QMouseEvent* /*event*/) {}

void cls_myGLwidget::mouseMoveEvent(QMouseEvent* event)
{
    //qDebug() << "cls_myGLwidget::mouseMoveEvent";

    if (this->hasMouseTracking()) {

        //// Current values
        float v_xa = (float)(event->x() - this->GetWinW()/2);  //// local coordinates (from screen center)
        float v_ya = -(float)(event->y() - this->GetWinH()/2); //// local coordinates (from screen center)
        float v_sphR = this->GetSphR();
        float v_za = sqrt(v_sphR*v_sphR - v_xa*v_xa - v_ya*v_ya);
        float v_za2;
        if (std::isnan(v_za)) v_za2 = 0.0f; //TODO check
        else v_za2 = v_za;

        glm::vec3 v_localDir;

        switch (mCurrentAction) {
        case ACT_PAN:
            mCamera->Pan(v_xa, v_ya, mStartXa, mStartYa, mStartLookPt);
            break;
        case ACT_ZOOM:
            mCamera->Zoom(v_ya, mStartYa, mStartFrAngle, mStartPBS); //// PBS - parallel box size
            break;
        case ACT_ROTATE:
            v_localDir = glm::normalize(glm::vec3(v_xa, v_ya, v_za2));
            if (mStartLocalDir != v_localDir) mCamera->Rotate(v_localDir, mStartLocalDir);
            mStartLocalDir = v_localDir;
            break;
        case ACT_TILT:
            v_localDir = glm::normalize(glm::vec3(v_xa, v_ya, 0.0f));
            if (mStartLocalDir != v_localDir) mCamera->Rotate(v_localDir, mStartLocalDir);
            mStartLocalDir = v_localDir;
            break;
        case ACT_SECTIONMOVE:
            qDebug() << "cls_myGLwidget::mouseMoveEvent:" << "ACT_SECTIONMOVE";
            //TODO implement
            break;
        case ACT_SECTIONROTATE:
            qDebug() << "cls_myGLwidget::mouseMoveEvent:" << "ACT_SECTIONROTATE";
            //TODO implement
            break;
        case ACT_SECTIONTILT:
            qDebug() << "cls_myGLwidget::mouseMoveEvent:" << "ACT_SECTIONTILT";
            //TODO implement
            break;
        case ACT_NO_ACT:
            break;
        }

        this->makeCurrent(); //TODO understand! why?!
        mCamera->SendCamToGPU();
        this->update();
    }
}

#ifndef QT_NO_WHEELEVENT
void cls_myGLwidget::wheelEvent(QWheelEvent* /*event*/) {}
#endif
void cls_myGLwidget::keyPressEvent(QKeyEvent* /*event*/) {}
void cls_myGLwidget::keyReleaseEvent(QKeyEvent* /*event*/) {}

void cls_myGLwidget::InitProgramsAndShaders(void)
{
    qDebug().nospace() << "cls_myGLwidget::InitProgramsAndShaders()";

    //// Create shaders - shading
    mShaderVshading = new QOpenGLShader(QOpenGLShader::Vertex);
    mShaderGshading = new QOpenGLShader(QOpenGLShader::Geometry);
    mShaderFshading = new QOpenGLShader(QOpenGLShader::Fragment);

    //// Create shaders - wire
    mShaderVwire = new QOpenGLShader(QOpenGLShader::Vertex);
    mShaderGwire = new QOpenGLShader(QOpenGLShader::Geometry);
    mShaderFwire = new QOpenGLShader(QOpenGLShader::Fragment);

    //// Create shaders - points
    mShaderVpoints = new QOpenGLShader(QOpenGLShader::Vertex);
    mShaderGpoints = new QOpenGLShader(QOpenGLShader::Geometry);
    mShaderFpoints = new QOpenGLShader(QOpenGLShader::Fragment);

    //// Compile shaders' source files - shading
    mShaderVshading->compileSourceFile("shaders/vertSh_shading.vp");
    mShaderGshading->compileSourceFile("shaders/geomSh_shading.gp");
    mShaderFshading->compileSourceFile("shaders/frSh_shading.fp");

    //// Compile shaders' source files - wire
    mShaderVwire->compileSourceFile("shaders/vertSh_wire.vp");
    mShaderGwire->compileSourceFile("shaders/geomSh_wire.gp");
    mShaderFwire->compileSourceFile("shaders/frSh_wire.fp");

    //// Compile shaders' source files - points
    mShaderVpoints->compileSourceFile("shaders/vertSh_points.vp");
    mShaderGpoints->compileSourceFile("shaders/geomSh_points.gp");
    mShaderFpoints->compileSourceFile("shaders/frSh_points.fp");

    //// Prepare the program for shading-style rendering
    mProgShading = new QOpenGLShaderProgram();
    mProgShading->addShader(mShaderVshading);
    mProgShading->addShader(mShaderGshading);
    mProgShading->addShader(mShaderFshading);
    mProgShading->link();

    //// Connect uniform variables
    mMVPshadingUniform = mProgShading->uniformLocation("MVP");

    //// Prepare the program for wire-style rendering
    mProgWire = new QOpenGLShaderProgram();
    mProgWire->addShader(mShaderVwire);
    mProgWire->addShader(mShaderGwire);
    mProgWire->addShader(mShaderFwire);
    mProgWire->link();

    //// Connect uniform variables
    mMVPwireUniform = mProgWire->uniformLocation("MVP");

    //// Prepare the program for points rendering
    mProgPoints = new QOpenGLShaderProgram();
    mProgPoints->addShader(mShaderVpoints);
    mProgPoints->addShader(mShaderGpoints);
    mProgPoints->addShader(mShaderFpoints);
    mProgPoints->link();

    //// Connect uniform variables
    mMVPpointsUniform = mProgWire->uniformLocation("MVP");
}

void cls_myGLwidget::InitBuffers(void)
{
    qDebug().nospace() << "cls_myGLwidget::InitBuffers()";

    //// Vertex array object
    mVAO = new QOpenGLVertexArrayObject();
    mVAO->create();
    if (mVAO->isCreated()) {
        qDebug().nospace() << "Created vertex array object.";
    } else {
        qDebug().nospace() << "Error creating vertex array object.";
    }

    //// Vertex buffer object
    mVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    mVBO->create();
    if (mVBO->isCreated()) {
        qDebug().nospace() << "Created vertex buffer object.";
    } else {
        qDebug().nospace() << "Error creating vertex buffer object.";
    }

    //// Inbex buffer object - shading
    mIBOshading = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    mIBOshading->create();
    if (mIBOshading->isCreated()) {
        qDebug().nospace() << "Created index buffer object for shading-style rendering.";
    } else {
        qDebug().nospace() << "Error creating index buffer object for shading-style rendering.";
    }

    //// Inbex buffer object - wire
    mIBOwire = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    mIBOwire->create();
    if (mIBOwire->isCreated()) {
        qDebug().nospace() << "Created index buffer object for wireframe-style rendering.";
    } else {
        qDebug().nospace() << "Error creating index buffer object for wireframe-style rendering.";
    }

    //// Inbex buffer object - points
    mIBOpoints = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    mIBOpoints->create();
    if (mIBOpoints->isCreated()) {
        qDebug().nospace() << "Created index buffer object for points rendering.";
    } else {
        qDebug().nospace() << "Error creating index buffer object for points rendering.";
    }
}

void cls_myGLwidget::InitGLparameters(void)
{
    qDebug().nospace() << "cls_myGLwidget::InitGLparameters()";

    this->makeCurrent(); //TODO understand! why?! // Seems not to be neccessary here, but still...

    //// Init culling             //TODO enable/disable
    //this->glEnable(GL_CULL_FACE);
    //this->glCullFace(GL_BACK);
    //this->glFrontFace(GL_CCW);

    //// Init depth
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthMask(GL_TRUE);
    this->glDepthFunc(GL_LEQUAL);
    this->glDepthRange(0.0f, 1.0f);

    this->glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    this->glClearDepth(1.0f);

    this->glEnable(GL_PROGRAM_POINT_SIZE);
    this->glPointSize(10.);

    //// Set the first vertex of the triangle as the vertex
    //// holding the color for the whole triangle for flat shading rendering
    this->glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
}
