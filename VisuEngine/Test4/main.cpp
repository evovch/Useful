#include <cstdio>

#include <GL/glew.h>
#include <GL/glut.h>

#include "cls_camera.h"
#include "cls_model.h"
#include "cls_scene.h"
#include "cls_renderer.h"

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
		exit(0);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gScene->Draw(gRenderer);

	glutSwapBuffers();
}

static void MouseFunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		//fprintf(stderr, "[DEBUG] DOWN x=%d, y=%d\n", x, y);

		//// Current values

		//// local coordinates (from screen center)
		float v_xa = (float)(x - glutGet(GLUT_WINDOW_WIDTH)/2);
		float v_ya = -(float)(y - glutGet(GLUT_WINDOW_HEIGHT)/2);

		float v_curR = sqrt(v_xa*v_xa + v_ya*v_ya);
		float v_sphR = GetSphR();
		float v_za = sqrt(v_sphR*v_sphR - v_xa*v_xa - v_ya*v_ya);

		//// Save values at mouse press
		mStartXa = v_xa;
		mStartYa = v_ya;
		mPrevXa = v_xa;
		mPrevYa = v_ya;
		mStartFrAngle = gCamera->GetFrAngle();
		mStartPBS = gCamera->GetParallelBoxSize();
		mStartLookPt = gCamera->GetLookPt();

		if (button == GLUT_LEFT_BUTTON) {
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
		} else if (button == GLUT_RIGHT_BUTTON) {
			mCurrentAction = ACT_ZOOM;
		} else if (button == GLUT_MIDDLE_BUTTON) {
			mCurrentAction = ACT_PAN;
		} else {

		}

		mMouseTracked = true;
	} else if (state == GLUT_UP) {
		//fprintf(stderr, "[DEBUG] UP x=%d, y=%d\n", x, y);

		mCurrentAction = ACT_NO_ACT;

		mMouseTracked = false;
	}
}

static void MouseMoveFunc(int x, int y)
{
	if (mMouseTracked) {

		//// Current values

		//// local coordinates (from screen center)
		float v_xa = (float)(x - glutGet(GLUT_WINDOW_WIDTH)/2);
		float v_ya = -(float)(y - glutGet(GLUT_WINDOW_HEIGHT)/2);

		//fprintf(stderr, "[DEBUG] MOVE xa=%f, ya=%f\n", v_xa, v_ya);

		float v_sphR = GetSphR();
		float v_za = sqrt(v_sphR*v_sphR - v_xa*v_xa - v_ya*v_ya);
		float v_za2;
		if (std::isnan(v_za)) v_za2 = 0.0f; //TODO check
		else v_za2 = v_za;

		glm::vec3 v_localDir;

		switch (mCurrentAction) {
		case ACT_PAN:
			gCamera->Pan(v_xa, v_ya, mStartXa, mStartYa, mStartLookPt);
			break;
		case ACT_ZOOM:
			//// PBS - parallel box size
			gCamera->Zoom(v_ya, mPrevYa, mStartYa, mStartFrAngle, mStartPBS);
			break;
		case ACT_ROTATE:
			v_localDir = glm::normalize(glm::vec3(v_xa, v_ya, v_za2));
			if (mStartLocalDir != v_localDir) gCamera->Rotate(v_localDir, mStartLocalDir);
			mStartLocalDir = v_localDir;
			break;
		case ACT_TILT:
			v_localDir = glm::normalize(glm::vec3(v_xa, v_ya, 0.0f));
			if (mStartLocalDir != v_localDir) gCamera->Rotate(v_localDir, mStartLocalDir);
			mStartLocalDir = v_localDir;
			break;
		case ACT_NO_ACT:
			break;
		}

		mPrevXa = v_xa;
		mPrevYa = v_ya;

		gCamera->SendCamToGPU(gRenderer);
	}
}

static void ReshapeFunc(GLsizei width, GLsizei height)
{
	//fprintf(stderr, "[DEBUG] width=%d, height=%d\n", width, height);

	if (height == 0) {
		fprintf(stderr, "[DEBUG] width=%d, height=%d\n", width, height);
		height = 1;
	}

	gCamera->SetWinSize(width, height);
	gCamera->SendCamToGPU(gRenderer);
	glViewport(0, 0, width, height); //TODO why do we need that?
}

static void IdleFunc(void)
{
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(mWindowWidth, mWindowHeight);
	glutInitWindowPosition(mWindowPosX, mWindowPosY);

	glutCreateWindow("Generic visualizer");

	GLenum err = glewInit();

	if (GLEW_OK != err) {
		// Problem: glewInit failed, something is seriously wrong.
		fprintf(stderr, "[ERROR] %s\n", glewGetErrorString(err));
		return 1;
	}

	fprintf(stderr, "[INFO] Using GLEW %s\n", glewGetString(GLEW_VERSION));
	fprintf(stderr, "[INFO] Version of OpenGL detected in the system: '%s'\n", glGetString(GL_VERSION));

	glutKeyboardFunc(KeyFunc);
	glutSpecialFunc(SpecKeyFunc);
	glutDisplayFunc(DisplayFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MouseMoveFunc);
	glutReshapeFunc(ReshapeFunc);
	glutIdleFunc(IdleFunc);

	gRenderer = new cls_renderer();
	gScene = new cls_scene();
	gCamera = new cls_camera(glm::vec3(0., 0., 0.), 1000.);

	cls_model* v_model1 = new cls_model();
	gScene->AddModel(v_model1);

	cls_model* v_model2 = new cls_model();
	v_model2->Shift(500., 300., -50.);
	gScene->AddModel(v_model2);

	cls_model* v_model3 = new cls_model();
	v_model3->Shift(0., -200., 350.);
	gScene->AddModel(v_model3);

	gScene->SendToGPU(gRenderer);

	gCamera->SendCamToGPU(gRenderer);

	glutMainLoop();

	return 0;
}