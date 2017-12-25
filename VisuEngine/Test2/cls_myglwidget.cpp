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
    // No OpenGL resource initialization is done here. (Qt docu)

    qDebug().nospace() << "cls_myGLwidget::cls_myGLwidget()";

    mShaderVpassthrought = nullptr;
    mShaderVwire = nullptr;
    mShaderGshading = nullptr;
    mShaderGwire = nullptr;
    mShaderFsmooth = nullptr;
    mShaderFflat = nullptr;

    mProgShading = nullptr;
    mProgWire = nullptr;

    mVAO = nullptr;
    mVBO = nullptr;
    mIBOshading = nullptr;
    mIBOwire = nullptr;

    //mMVPshadingUniform = ?;
    //mMVPwireUniform = ?;

    mCamera = nullptr;
    mModel = nullptr;

    mCurrentAction = ACT_NO_ACT;
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
    delete mShaderFsmooth;
    delete mShaderFflat;

    delete mProgShading;
    delete mProgWire;

    delete mVAO;
    delete mVBO;
    delete mIBOshading;
    delete mIBOwire;

    delete mOpenGLlogger;

    delete mCamera;
    delete mModel;
}

// ------------------------------------------------------------------------------------------------

void cls_myGLwidget::initializeGL()
{
    qDebug().nospace() << "cls_myGLwidget::initializeGL()";

    this->makeCurrent(); // method of QOpenGLWidget (mother) class

    this->initializeOpenGLFunctions(); // method of QOpenGLFunctions_4_5_Core (mother) class

    this->InitProgramsAndShaders(); // method of this class cls_myGLwidget

    this->InitBuffers(); // method of this class cls_myGLwidget

    this->InitGLparameters(); // method of this class cls_myGLwidget

    //QOpenGLContext* v_ctx = QOpenGLContext::currentContext();
    mOpenGLlogger = new QOpenGLDebugLogger(this);
    mOpenGLlogger->initialize(); // initializes in the current context, i.e. v_ctx

    glm::vec3 v_center(0., 0., 0.);
    float v_radius = 2.;
    mCamera = new cls_myCamera(v_center, v_radius, this);

    mModel = new cls_DisplayModel(this);

    // Send model to GPU
    mModel->SendToGPU();
}

void cls_myGLwidget::resizeGL(int w, int h)
{
    qDebug().nospace() << "cls_myGLwidget::resizeGL(" << w << ", " << h << ")";
    ////qDebug().nospace() << "QWidget->width(): " << this->width();
    ////qDebug().nospace() << "QWidget->height(): " << this->height();

    mCamera->SendCamToGPU();
    glViewport(0, 0, w, h); //TODO why do we need that?
}

void cls_myGLwidget::paintGL()
{
    qDebug().nospace() << "cls_myGLwidget::paintGL()";

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mModel->Draw();

    const QList<QOpenGLDebugMessage> v_messages = mOpenGLlogger->loggedMessages();
    for (const QOpenGLDebugMessage &v_message : v_messages) {
        qDebug() << v_message;
    }
}

// ------------------------------------------------------------------------------------------------

void cls_myGLwidget::mousePressEvent(QMouseEvent* event)
{
    ////qDebug() << "cls_myGLwidget::mousePressEvent";

    // Current values
    float v_xa = (float)(event->x() - this->GetWinW()/2);  // local coordinates (from screen center)
    float v_ya = -(float)(event->y() - this->GetWinH()/2); // local coordinates (from screen center)
    float v_curR = sqrt(v_xa*v_xa + v_ya*v_ya);
    float v_sphR = this->GetSphR();
    float v_za = sqrt(v_sphR*v_sphR - v_xa*v_xa - v_ya*v_ya);

    // Save values at mouse press
    mStartX = v_xa;
    mStartY = v_ya;
    mStartFrAngle = mCamera->GetFrAngle();
    mStartPBS = mCamera->GetParallelBoxSize();

    if (event->modifiers() == Qt::ControlModifier) {
        mCurrentAction = ACT_ZOOM;
    } else {
        // Check: if we are inside the central circle area - rotate, otherwise - tilt
        if (v_curR > v_sphR) {
            // TILT
            mStartLocalDir = glm::normalize(glm::vec3(v_xa, v_ya, 0.0f));
            mCurrentAction = ACT_TILT;
        } else {
            // ROTATE
            mStartLocalDir = glm::normalize(glm::vec3(v_xa, v_ya, v_za));
            mCurrentAction = ACT_ROTATE;
        }
    }

    this->setMouseTracking(true);
}

void cls_myGLwidget::mouseReleaseEvent(QMouseEvent* /*event*/)
{
    ////qDebug() << "cls_myGLwidget::mouseReleaseEvent";

    mCurrentAction = ACT_NO_ACT;

    this->setMouseTracking(false);
}

void cls_myGLwidget::mouseDoubleClickEvent(QMouseEvent* /*event*/)
{
}

void cls_myGLwidget::mouseMoveEvent(QMouseEvent* event)
{
    ////qDebug() << "cls_myGLwidget::mouseMoveEvent";

    if (this->hasMouseTracking()) {

        // Current values
        float v_xa = (float)(event->x() - this->GetWinW()/2);  // local coordinates (from screen center)
        float v_ya = -(float)(event->y() - this->GetWinH()/2); // local coordinates (from screen center)
        float v_sphR = this->GetSphR();
        float v_za = sqrt(v_sphR*v_sphR - v_xa*v_xa - v_ya*v_ya);
        float v_za2;
        if (std::isnan(v_za)) v_za2 = 0.0f;
        else v_za2 = v_za;

        glm::vec3 v_localDir;

        switch (mCurrentAction) {
        case ACT_PAN:
            break;
        case ACT_ZOOM:
            mCamera->Zoom(v_ya, mStartY, mStartFrAngle, mStartPBS); // PBS - parallel box size
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
            break;
        case ACT_SECTIONROTATE:
            break;
        case ACT_SECTIONTILT:
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
void cls_myGLwidget::wheelEvent(QWheelEvent* /*event*/)
{
}
#endif

void cls_myGLwidget::keyPressEvent(QKeyEvent* /*event*/)
{
}

void cls_myGLwidget::keyReleaseEvent(QKeyEvent* /*event*/)
{
}

// ------------------------------------------------------------------------------------------------

void cls_myGLwidget::InitProgramsAndShaders(void)
{
    qDebug().nospace() << "cls_myGLwidget::InitProgramsAndShaders()";

    // Create shaders
    mShaderVpassthrought = new QOpenGLShader(QOpenGLShader::Vertex);
    mShaderVwire = new QOpenGLShader(QOpenGLShader::Vertex);
    mShaderGshading = new QOpenGLShader(QOpenGLShader::Geometry);
    mShaderGwire = new QOpenGLShader(QOpenGLShader::Geometry);
    mShaderFsmooth = new QOpenGLShader(QOpenGLShader::Fragment);
    mShaderFflat = new QOpenGLShader(QOpenGLShader::Fragment);

    // Compile shaders' source files
    mShaderVpassthrought->compileSourceFile("shaders/vertSh_passthrough.vp");
    mShaderVwire->compileSourceFile("shaders/vertSh_wire.vp");
    mShaderGshading->compileSourceFile("shaders/geomSh_aux.gp");
    mShaderGwire->compileSourceFile("shaders/geomSh_wire.gp");
    mShaderFsmooth->compileSourceFile("shaders/frSh_smooth.fp");
    mShaderFflat->compileSourceFile("shaders/frSh_flat.fp");

    // Prepare the program for shading-style rendering
    mProgShading = new QOpenGLShaderProgram();
    mProgShading->addShader(mShaderVpassthrought);
    mProgShading->addShader(mShaderGshading);
    mProgShading->addShader(mShaderFsmooth);
    mProgShading->link();

    // Connect uniform variables
    mMVPshadingUniform = mProgShading->uniformLocation("MVP");

    // Prepare the program for wire-style rendering
    mProgWire = new QOpenGLShaderProgram();
    mProgWire->addShader(mShaderVwire);
    mProgWire->addShader(mShaderGwire);
    mProgWire->addShader(mShaderFflat);
    mProgWire->link();

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
    } else {
        qDebug().nospace() << "Error creating vertex array object.";
    }

    mVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    mVBO->create();
    if (mVBO->isCreated()) {
        qDebug().nospace() << "Created vertex buffer object.";
    } else {
        qDebug().nospace() << "Error creating vertex buffer object.";
    }

    mIBOshading = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    mIBOshading->create();
    if (mIBOshading->isCreated()) {
        qDebug().nospace() << "Created index buffer object for shading-style rendering.";
    } else {
        qDebug().nospace() << "Error creating index buffer object for shading-style rendering.";
    }

    mIBOwire = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    mIBOwire->create();
    if (mIBOwire->isCreated()) {
        qDebug().nospace() << "Created index buffer object for wireframe-style rendering.";
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

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0f);

    // Set the first vertex of the triangle as the vertex
    // holding the color for the whole triangle for flat shading rendering
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
}

// ------------------------------------------------------------------------------------------------
