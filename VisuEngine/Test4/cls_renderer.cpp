#include "cls_renderer.h"

// STD
#include <algorithm>
#include <cstdio>

cls_renderer::cls_renderer()
{
	this->Init();
}

cls_renderer::~cls_renderer()
{
}

void cls_renderer::Init(void)
{
	this->InitProgs();
	this->InitBuffers();
	this->InitGLparameters();

	mProgs.push_back(mShadingDrawProgram); mUnifs.push_back(mMVPshadingUniform);
	mProgs.push_back(mWireDrawProgram);    mUnifs.push_back(mMVPwireUniform);
	mProgs.push_back(mPointsDrawProgram);  mUnifs.push_back(mMVPpointsUniform);
}

void cls_renderer::InitProgs(void)
{
	// Create program and shaders

	// ------------------------------ Shading draw program ------------------------------
	mShadingDrawProgram = glCreateProgram();
	std::vector<GLuint> v_shaderList;
	v_shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "shaders/vertSh_shading.vp"));
	v_shaderList.push_back(CreateShader(GL_GEOMETRY_SHADER, "shaders/geomSh_shading.gp"));
	v_shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "shaders/frSh_shading.fp"));

	this->CreateProg(mShadingDrawProgram, v_shaderList);

	// Cleanup
	std::for_each(v_shaderList.begin(), v_shaderList.end(), glDeleteShader);

	// Connect uniform variables
	mMVPshadingUniform = glGetUniformLocation(mShadingDrawProgram, "MVP");

	mEyePosUniform = glGetUniformLocation(mShadingDrawProgram, "EyePos");
	mLightPosUniform = glGetUniformLocation(mShadingDrawProgram, "LightPos");

	// ------------------------------ Wireframe draw program ------------------------------
	mWireDrawProgram = glCreateProgram();
	std::vector<GLuint> shaderList2;
	shaderList2.push_back(CreateShader(GL_VERTEX_SHADER, "shaders/vertSh_wire.vp"));
	shaderList2.push_back(CreateShader(GL_GEOMETRY_SHADER, "shaders/geomSh_wire.gp"));
	shaderList2.push_back(CreateShader(GL_FRAGMENT_SHADER, "shaders/frSh_wire.fp"));

	this->CreateProg(mWireDrawProgram, shaderList2);

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

	this->CreateProg(mPointsDrawProgram, v_shaderList3);

	// Cleanup
	std::for_each(v_shaderList3.begin(), v_shaderList3.end(), glDeleteShader);

	// Connect uniform variables
	mMVPpointsUniform = glGetUniformLocation(mPointsDrawProgram, "MVP");

	// ---------------------------------------------------------------------------------
	fprintf(stderr, "[INFO] Initialized programs.\n");
}

void cls_renderer::InitBuffers(void)
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBOshading);
	glGenBuffers(1, &mIBOwire);
	glGenBuffers(1, &mIBOpoints);
	fprintf(stderr, "[INFO] Initialized buffers.\n");
}

void cls_renderer::InitGLparameters(void)
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
	glPointSize(0.);

	//// Set the first vertex of the triangle as the vertex
	//// holding the color for the whole triangle for flat shading rendering
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	fprintf(stderr, "[INFO] Initialized OpenGL parameters.\n");
}

GLuint cls_renderer::CreateShader(GLenum p_eShaderType, const std::string p_strShaderFile)
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

void cls_renderer::CreateProg(GLuint p_program, const std::vector<GLuint>& p_shaderList)
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
