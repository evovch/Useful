#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#include <GL/glew.h>
#include <GL/glut.h>

#include "cls_camera.h"
#include "cls_model.h"
#include "support.h"
#include "cls_axis2_placement_3d.h"
#include "cls_cylinder.h"

// Program object for shading-style rendering
GLuint mShadingDrawProgram;
// Program object for wireframe-style rendering
GLuint mWireDrawProgram;
// Program object for points rendering
GLuint mPointsDrawProgram;

// Uniform object for shading-style rendering
GLuint mMVPshadingUniform;
// Uniform object for wireframe-style rendering
GLuint mMVPwireUniform;
// Uniform object for points rendering
GLuint mMVPpointsUniform;

// Vertex array object
GLuint mVAO;
// Vertex buffer object
GLuint mVBO;

// Index buffer object for shading-style rendering
GLuint mIBOshading;
// Index buffer object for wireframe-style rendering
GLuint mIBOwire;
// Index buffer object for points rendering
GLuint mIBOpoints;

std::vector<GLuint> mProgs;
std::vector<GLuint> mUnifs;

cls_model* mModel;

cls_camera* mCamera;

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

	mModel->DrawTriangles(mShadingDrawProgram, mVAO, mIBOshading);
	mModel->DrawWires(mWireDrawProgram, mVAO, mIBOwire);
	mModel->DrawPoints(mPointsDrawProgram, mVAO, mIBOpoints);

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
		mStartFrAngle = mCamera->GetFrAngle();
		mStartPBS = mCamera->GetParallelBoxSize();
		mStartLookPt = mCamera->GetLookPt();

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
			mCamera->Pan(v_xa, v_ya, mStartXa, mStartYa, mStartLookPt);
			break;
		case ACT_ZOOM:
			//// PBS - parallel box size
			mCamera->Zoom(v_ya, mPrevYa, mStartYa, mStartFrAngle, mStartPBS);
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
		case ACT_NO_ACT:
			break;
		}

		mPrevXa = v_xa;
		mPrevYa = v_ya;

		mCamera->SendCamToGPU(mProgs, mUnifs);
	}
}

static void ReshapeFunc(GLsizei width, GLsizei height)
{
	//fprintf(stderr, "[DEBUG] width=%d, height=%d\n", width, height);

	if (height == 0) {
		fprintf(stderr, "[DEBUG] width=%d, height=%d\n", width, height);
		height = 1;
	}

	mCamera->SetWinSize(width, height);
	mCamera->SendCamToGPU(mProgs, mUnifs);
	glViewport(0, 0, width, height); //TODO why do we need that?
}

static void IdleFunc(void)
{
	glutPostRedisplay();
}

GLuint CreateShader(GLenum p_eShaderType, const std::string p_strShaderFile)
{
	//fprintf(stderr, "[DEBUG] Creating shader from file '%s'\n", p_strShaderFile.c_str());

	// Open and read input file with shader code
	FILE* v_inFile = NULL;
	v_inFile = fopen(p_strShaderFile.c_str(), "rb");
	//TODO check return value of fopen
	if (v_inFile == NULL) {
		fprintf(stderr, "[ERROR] Could not open file '%s'\n", p_strShaderFile.c_str());
		return 0;
	}

	fseek(v_inFile, 0, SEEK_END);
	long v_inFileSize = ftell(v_inFile);
	fseek(v_inFile, 0, SEEK_SET);

	char* v_cstrFullShaderCode = (char*)malloc(v_inFileSize+1);
	fread(v_cstrFullShaderCode, v_inFileSize, 1, v_inFile);
	fclose(v_inFile);
	v_cstrFullShaderCode[v_inFileSize] = 0;

	// Prepare and compile shader
	GLuint v_shader = glCreateShader(p_eShaderType);
	glShaderSource(v_shader, 1, (const GLchar**)&v_cstrFullShaderCode, NULL);
	glCompileShader(v_shader);

	// Get diagnostics info and print it in case of error
	GLint v_status;
	glGetShaderiv(v_shader, GL_COMPILE_STATUS, &v_status);

	std::string v_strShaderType;
	switch(p_eShaderType) {
	case GL_VERTEX_SHADER: v_strShaderType = "vertex"; break;
	case GL_GEOMETRY_SHADER: v_strShaderType = "geometry"; break;
	case GL_FRAGMENT_SHADER: v_strShaderType = "fragment"; break;
	}

	if (v_status == GL_FALSE) {
		GLint v_infoLogLength;
		glGetShaderiv(v_shader, GL_INFO_LOG_LENGTH, &v_infoLogLength);

		GLchar *v_strInfoLog = new GLchar[v_infoLogLength + 1];
		glGetShaderInfoLog(v_shader, v_infoLogLength, NULL, v_strInfoLog);

		fprintf(stderr, "[ERROR] Compile failure in %s shader from %s:\n%s\n", v_strShaderType.c_str(), p_strShaderFile.c_str(), v_strInfoLog);
		delete[] v_strInfoLog;
	} else {
		fprintf(stderr, "[INFO] Successfully compiled %s shader from %s.\n", v_strShaderType.c_str(), p_strShaderFile.c_str());
	}

	// Finalize
	free(v_cstrFullShaderCode);
	return v_shader;
}

void CreateProg(GLuint p_program, const std::vector<GLuint>& p_shaderList)
{
	// (Create the program outside)
	// Here - attach the shaders, link.

	for (size_t iLoop = 0; iLoop < p_shaderList.size(); iLoop++)
		glAttachShader(p_program, p_shaderList[iLoop]);

	glLinkProgram(p_program);

	GLint v_status;
	glGetProgramiv (p_program, GL_LINK_STATUS, &v_status);
	if (v_status == GL_FALSE) {
		GLint v_infoLogLength;
		glGetProgramiv(p_program, GL_INFO_LOG_LENGTH, &v_infoLogLength);

		GLchar *v_strInfoLog = new GLchar[v_infoLogLength + 1];
		glGetProgramInfoLog(p_program, v_infoLogLength, NULL, v_strInfoLog);
		fprintf(stderr, "[ERROR] Linker failure: %s\n", v_strInfoLog);
		delete[] v_strInfoLog;
	}

	// Cleanup
	for (size_t iLoop = 0; iLoop < p_shaderList.size(); iLoop++)
		glDetachShader(p_program, p_shaderList[iLoop]);
}

void InitProgs(void)
{
	// Create program and shaders

	// ------------------------------ Shading draw program ------------------------------
	mShadingDrawProgram = glCreateProgram();
	std::vector<GLuint> v_shaderList;
	v_shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "shaders/vertSh_shading.vp"));
	v_shaderList.push_back(CreateShader(GL_GEOMETRY_SHADER, "shaders/geomSh_shading.gp"));
	v_shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "shaders/frSh_shading.fp"));

	CreateProg(mShadingDrawProgram, v_shaderList);

	// Cleanup
	std::for_each(v_shaderList.begin(), v_shaderList.end(), glDeleteShader);

	// Connect uniform variables
	mMVPshadingUniform = glGetUniformLocation(mShadingDrawProgram, "MVP");

	// ------------------------------ Wireframe draw program ------------------------------
	mWireDrawProgram = glCreateProgram();
	std::vector<GLuint> shaderList2;
	shaderList2.push_back(CreateShader(GL_VERTEX_SHADER, "shaders/vertSh_wire.vp"));
	shaderList2.push_back(CreateShader(GL_GEOMETRY_SHADER, "shaders/geomSh_wire.gp"));
	shaderList2.push_back(CreateShader(GL_FRAGMENT_SHADER, "shaders/frSh_wire.fp"));

	CreateProg(mWireDrawProgram, shaderList2);

	// Cleanup
	std::for_each(shaderList2.begin(), shaderList2.end(), glDeleteShader);

	// Connect uniform variables
	mMVPwireUniform = glGetUniformLocation(mWireDrawProgram, "MVP");

	// ------------------------------ Points draw program ------------------------------
	mPointsDrawProgram = glCreateProgram();
	std::vector<GLuint> v_shaderList3;
	v_shaderList3.push_back(CreateShader(GL_VERTEX_SHADER, "shaders/vertSh_points.vp"));
	v_shaderList3.push_back(CreateShader(GL_GEOMETRY_SHADER, "shaders/geomSh_points.gp"));
	v_shaderList3.push_back(CreateShader(GL_FRAGMENT_SHADER, "shaders/frSh_points.fp"));

	CreateProg(mPointsDrawProgram, v_shaderList3);

	// Cleanup
	std::for_each(v_shaderList3.begin(), v_shaderList3.end(), glDeleteShader);

	// Connect uniform variables
	mMVPpointsUniform = glGetUniformLocation(mPointsDrawProgram, "MVP");

	// ---------------------------------------------------------------------------------
	fprintf(stderr, "[INFO] Initialized programs.\n");
}

void InitBuffers(void)
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBOshading);
	glGenBuffers(1, &mIBOwire);
	glGenBuffers(1, &mIBOpoints);
	fprintf(stderr, "[INFO] Initialized buffers.\n");
}

void InitGLparameters(void)
{
	//// Init culling             //TODO enable/disable
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	//// Init depth
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(10.);

	//// Set the first vertex of the triangle as the vertex
	//// holding the color for the whole triangle for flat shading rendering
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	fprintf(stderr, "[INFO] Initialized OpenGL parameters.\n");
}

void Init(void)
{
	InitProgs();
	InitBuffers();
	InitGLparameters();

	mProgs.push_back(mShadingDrawProgram); mUnifs.push_back(mMVPshadingUniform);
	mProgs.push_back(mWireDrawProgram);    mUnifs.push_back(mMVPwireUniform);
	mProgs.push_back(mPointsDrawProgram);  mUnifs.push_back(mMVPpointsUniform);
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

	// PROGRAMS, BUFFERS, UNIFORMS
	Init();

	// CAMERA
	glm::vec3 v_center(0., 0., 0.);
	float v_radius = 1000.;
	mCamera = new cls_camera(v_center, v_radius);

	mCamera->SendCamToGPU(mProgs, mUnifs);

	// LOCAL AXIS SYSTEM
	cls_axis2_placement_3d v_localAxisSystem;

	// Basic
	v_localAxisSystem.mLocation = point_t(0.5, 0.6, 0.7);
	v_localAxisSystem.mAxis = direction_t(0.3, 1., 0.2);
	v_localAxisSystem.mAxis.Normalize();
	v_localAxisSystem.mRefDirection = direction_t(0.1, 0.7, 0.); // a - approximate X axis
	v_localAxisSystem.mRefDirection.Normalize();

	// Derived
	double az = dotProd(v_localAxisSystem.mRefDirection, v_localAxisSystem.mAxis); // az = a*z
	direction_t azz = v_localAxisSystem.mAxis; // azz = (a*z) x z // 'x' meaning cross product
	azz.Multiply(az);
	// Exact X axis
	v_localAxisSystem.mXaxis = v_localAxisSystem.mRefDirection - azz; // X = a - (a*z) x z // 'x' meaning cross product
	v_localAxisSystem.mXaxis.Normalize();
	// Y axis
	crossProd(v_localAxisSystem.mAxis, v_localAxisSystem.mXaxis, v_localAxisSystem.mYaxis); // Y = Z x X // 'x' meaning cross product
	v_localAxisSystem.mYaxis.Normalize();

	// CYLINDER
	cls_cylinder v_cyl;
	v_cyl.mR = 0.4;
	v_cyl.mPosition = &v_localAxisSystem;

	// TRACKS
	point_t pTrackStart(0., 0., 0.);  // in global CS
	direction_t dTrackDir;  // in global CS

	unsigned int nTracks = 100;

	point_t v_inter1;
	point_t v_inter2;

	float* coordsArray = new float[nTracks*3*2];
	unsigned int counter = 0;

	// Loop over tracks
	for (unsigned int i=0; i<nTracks; i++)
	{
		// Generate random direction
		double theta;
		double phi;
		theta = ((double)rand()/(double)RAND_MAX) * M_PI;
		phi = ((double)rand()/(double)RAND_MAX) * 2. * M_PI;
		dTrackDir.fPx = sin(theta) * cos(phi);
		dTrackDir.fPy = sin(theta) * sin(phi);
		dTrackDir.fPz = cos(theta);
		dTrackDir.Normalize();

		unsigned int curNinter = v_cyl.Intersect(pTrackStart, dTrackDir, v_inter1, v_inter2);

		switch (curNinter) {
		case 1:
			////fprintf(stderr, "%f, %f, %f\n", v_inter1.fX, v_inter1.fY, v_inter1.fZ);
			coordsArray[counter*3+0] = v_inter1.fX;
			coordsArray[counter*3+1] = v_inter1.fY;
			coordsArray[counter*3+2] = v_inter1.fZ;
			counter++;
			break;
		case 2:
			////fprintf(stderr, "%f, %f, %f\n", v_inter1.fX, v_inter1.fY, v_inter1.fZ);
			coordsArray[counter*3+0] = v_inter1.fX;
			coordsArray[counter*3+1] = v_inter1.fY;
			coordsArray[counter*3+2] = v_inter1.fZ;
			counter++;
			////fprintf(stderr, "%f, %f, %f\n", v_inter2.fX, v_inter2.fY, v_inter2.fZ);
			coordsArray[counter*3+0] = v_inter2.fX;
			coordsArray[counter*3+1] = v_inter2.fY;
			coordsArray[counter*3+2] = v_inter2.fZ;
			counter++;
			break;
		default:
			break;
		}
	}

	// MODEL
	mModel = new cls_model();

//	mModel->GenerateLocalAxisSystem(&v_localAxisSystem);
//	mModel->AppendPoints(counter, coordsArray);
//	mModel->Dump();

	mModel->ImportGDML("tesselated.gdml");

	mModel->SendToGPUFull(mVAO, mVBO, mIBOshading, mIBOwire, mIBOpoints);

	glutKeyboardFunc(KeyFunc);
	glutSpecialFunc(SpecKeyFunc);
	glutDisplayFunc(DisplayFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MouseMoveFunc);
	glutReshapeFunc(ReshapeFunc);
	glutIdleFunc(IdleFunc);

	glutMainLoop();

	return 0;
}
