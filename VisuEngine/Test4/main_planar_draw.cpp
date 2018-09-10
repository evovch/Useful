// STD
#include <ctime>

// OpenGL
#include <GL/glew.h>
#include <GL/glut.h>

// Project
#include "base/cls_logger.h"

#include "graphics/cls_camera.h"
#include "graphics/cls_model.h"
#include "graphics/cls_renderer.h"
#include "graphics/cls_scene.h"

#include "brep_new/cls_bezier_spline.h"
#include "brep_new/cls_b_spline.h"

cls_renderer* gRenderer;
cls_scene* gScene;
cls_camera* gCamera;

float mCentralCircleK = 0.8f;

action_type mCurrentAction;
float mStartXa;  // At mouse press
float mStartYa;  // At mouse press
float mPrevXa;
float mPrevYa;
glm::vec3 mStartLocalDir;   // Rotating
glm::vec3 mStartLookPt;     // Panning
float mStartFrAngle;        // Zooming (perspective)
float mStartPBS;            // Zooming (parallel), PBS - parallel box size

bool mMouseTracked = false;
bool mMouseMoved = false;

bool mFullScreenMode = false;
int mWindowWidth  = 800;
int mWindowHeight = 600;
int mWindowPosX   = 100;
int mWindowPosY   = 100;

int GetMinWinDim(void) {
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	return (h < w) ? h : w;
}

float GetSphR(void) {
	return (mCentralCircleK * GetMinWinDim() / 2.0f);
}

static void KeyFunc(unsigned char key, int /*x*/, int /*y*/)
{
	switch (key) {
	case 27: // ESC key
		LOG(INFO) << "Caught ESC key press." << cls_logger::endl;
		exit(0);
	case 'r':
		gCamera->Reset();
		gCamera->SendCamToGPU(gRenderer);
		glutPostRedisplay();
		break;
	case 'w':
		gScene->SendToGPU(gRenderer);
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

static void SpecKeyFunc(int key, int /*x*/, int /*y*/)
{
	switch (key) {
	case GLUT_KEY_F11:
		mFullScreenMode = !mFullScreenMode;
		if (mFullScreenMode) {
			mWindowWidth  = glutGet(GLUT_WINDOW_WIDTH);
			mWindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
			mWindowPosX   = glutGet(GLUT_WINDOW_X);
			mWindowPosY   = glutGet(GLUT_WINDOW_Y);
			glutFullScreen();
		} else {
			glutReshapeWindow(mWindowWidth, mWindowHeight);
			glutPositionWindow(mWindowPosX, mWindowPosY);
		}
		break;
	default:
		break;
	}
}

static void DisplayFunc(void)
{
	LOG(DEBUG4) << "main::DisplayFunc()" << cls_logger::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gScene->Draw(gRenderer);

	glutSwapBuffers();
}

static void MouseFunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		LOG(DEBUG4) << "main::MouseFunc(): press at x=" << x << ", y=" << y << cls_logger::endl;

		//// Current values

		//// local coordinates (from screen center)
		float v_xa = (float)(x - glutGet(GLUT_WINDOW_WIDTH)/2);
		float v_ya = -(float)(y - glutGet(GLUT_WINDOW_HEIGHT)/2);

		//float v_curR = sqrt(v_xa*v_xa + v_ya*v_ya);
		//float v_sphR = GetSphR();
		//float v_za = sqrt(v_sphR*v_sphR - v_xa*v_xa - v_ya*v_ya);

		//// Save values at mouse press
		mStartXa = v_xa;
		mStartYa = v_ya;
		mPrevXa = v_xa;
		mPrevYa = v_ya;
		mStartFrAngle = gCamera->GetFrAngle();
		mStartPBS = gCamera->GetParallelBoxSize();
		mStartLookPt = gCamera->GetLookPt();

		if (button == GLUT_RIGHT_BUTTON) {
			mCurrentAction = ACT_ZOOM;
		} else if (button == GLUT_MIDDLE_BUTTON) {
			mCurrentAction = ACT_PAN;
		} else {

		}

		mMouseTracked = true;
		mMouseMoved = false;
	} else if (state == GLUT_UP) {
		LOG(DEBUG4) << "main::MouseFunc(): release at x=" << x << ", y=" << y << cls_logger::endl;

		mCurrentAction = ACT_NO_ACT;

		mMouseTracked = false;
	}
}

static void MouseMoveFunc(int x, int y)
{
	if (mMouseTracked) {

		mMouseMoved = true;

		//// Current values

		//// local coordinates (from screen center)
		float v_xa = (float)(x - glutGet(GLUT_WINDOW_WIDTH)/2);
		float v_ya = -(float)(y - glutGet(GLUT_WINDOW_HEIGHT)/2);

		float v_sphR = GetSphR();
		float v_za = sqrt(v_sphR*v_sphR - v_xa*v_xa - v_ya*v_ya);
		float v_za2;
		if (std::isnan(v_za)) v_za2 = 0.0f; //TODO check
		else v_za2 = v_za;

		LOG(DEBUG4) << "main::MouseMoveFunc(): move at "
		            << "x=" << x << ", y=" << y << ", "
		            << "xa=" << v_xa << ", ya=" << v_ya
		            << "za=" << v_za << ", v_za2=" << v_za2
		            << cls_logger::endl;

		glm::vec3 v_localDir; // no initialization needed

		switch (mCurrentAction) {
		case ACT_PAN:
			gCamera->Pan(v_xa, v_ya, mStartXa, mStartYa, mStartLookPt);
			break;
		case ACT_ZOOM:
			//// PBS - parallel box size
			gCamera->Zoom(v_ya, mPrevYa, mStartYa, mStartFrAngle, mStartPBS);
			break;
		case ACT_NO_ACT:
			break;
		default:
			break;
		}

		mPrevXa = v_xa;
		mPrevYa = v_ya;

		gCamera->SendCamToGPU(gRenderer);

		glutPostRedisplay();
	}
}

static void ReshapeFunc(GLsizei width, GLsizei height)
{
	LOG(DEBUG4) << "main::ReshapeFunc(): width=" << width << ", height=" << height << cls_logger::endl;

	if (height == 0) {
		height = 1;
	}

	gCamera->SetWinSize(width, height);
	gCamera->SendCamToGPU(gRenderer);
	glViewport(0, 0, width, height); //TODO why do we need that?
}

static void IdleFunc(void)
{
	//LOG(DEBUG4) << "main::IdleFunc()" << cls_logger::endl;
	////glutPostRedisplay();
}

int main(int argc, char** argv)
{
	cls_logger::SetLevel(DEBUG4);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(mWindowWidth, mWindowHeight);
	glutInitWindowPosition(mWindowPosX, mWindowPosY);

	glutCreateWindow("Generic visualizer");

	GLenum err = glewInit();

	if (GLEW_OK != err) {
		// Problem: glewInit failed, something is seriously wrong.
		LOG(FATAL) << "main: " << glewGetErrorString(err) << cls_logger::endl;
		return 1;
	}

	LOG(INFO) << "Using GLEW '" << glewGetString(GLEW_VERSION) << "'" << cls_logger::endl;
	LOG(INFO) << "Version of OpenGL detected in the system: '" << glGetString(GL_VERSION) << "'" << cls_logger::endl;

	glutKeyboardFunc(KeyFunc);
	glutSpecialFunc(SpecKeyFunc);
	glutDisplayFunc(DisplayFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MouseMoveFunc);
	glutReshapeFunc(ReshapeFunc);
	glutIdleFunc(IdleFunc);

	gRenderer = new cls_renderer();
	gScene = new cls_scene();
	gCamera = new cls_camera(glm::vec3(0., 0., 0.), 100.);


	std::srand(std::time(nullptr));


	cls_model* v_modelSpline1 = new cls_model();
	cls_b_spline<float, float>* v_spline1 = new cls_b_spline<float, float>();
	v_spline1->Generate(4,7);
	v_spline1->BuildModel(v_modelSpline1);
	gScene->AddModel(v_modelSpline1);


	cls_model* v_modelSpline2 = new cls_model();
	cls_bezier_spline<float, float>* v_spline2 = new cls_bezier_spline<float, float>();
	v_spline2->Generate(3);
	v_spline2->BuildModel(v_modelSpline2);
	gScene->AddModel(v_modelSpline2);


	gScene->SendToGPU(gRenderer);

	gCamera->SendCamToGPU(gRenderer);

	//gScene->Dump();

	glutMainLoop();

	return 0;
}
