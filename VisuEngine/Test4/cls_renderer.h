/**

	@class cls_renderer

**/

#ifndef CLS_RENDERER_H
#define CLS_RENDERER_H

// STD
#include <string>
#include <vector>

// OpenGL
#include <GL/glew.h>

class cls_offscreen_renderer;

class cls_renderer
{
public:
	cls_renderer(void);
	~cls_renderer(void);

	void Init(void);
	void InitProgs(void);
	void InitBuffers(void);
	void InitGLparameters(void);
	void InitDatumAll(void);

	GLuint CreateShader(GLenum p_eShaderType, const std::string p_strShaderFile);
	void CreateProg(GLuint p_program, const std::vector<GLuint>& p_shaderList);

public:
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

	GLuint mEyePosUniform;
	GLuint mLightPosUniform;

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

	cls_offscreen_renderer* mOffscreenRenderer;

/*
	// Program object for datum objects rendering
	GLuint mDatumDrawProgram;
	// Uniform object for datum objects rendering
	GLuint mMVPdatumUniform;
	// Vertex array object for datum objects rendering
	GLuint mVAOdatum;
	// Vertex buffer object for datum objects rendering
	GLuint mVBOdatum;
	// Index buffer object for datum triangles rendering
	GLuint mIBOdatumTr;
	// Index buffer object for datum wires rendering
	GLuint mIBOdatumW;
	// Index buffer object for datum points rendering
	GLuint mIBOdatumP;
*/
};

#endif // CLS_RENDERER_H
