/**

    @class cls_offscreen_renderer

**/

#ifndef CLS_OFFSCREEN_RENDERER_H
#define CLS_OFFSCREEN_RENDERER_H

// OpenGL
#include <GL/glew.h>

class cls_renderer;
class cls_scene;

class cls_offscreen_renderer
{
public:
	cls_offscreen_renderer(cls_renderer* p_renderer);
	cls_offscreen_renderer(cls_renderer* p_renderer, GLsizei p_width, GLsizei p_height);
	~cls_offscreen_renderer(void);

	void Destruct(void);
	void Construct(void);

	void Resize(GLsizei p_width, GLsizei p_height);

	void RenderSceneToBuffer(cls_scene* p_scene);

	/**
	 * Output o_color has NUMOFCOMPONENTS components
	 */
	void PickColor(GLsizei p_x, GLsizei p_y, GLubyte* o_color) const;

	void WritePNGfile(const char* p_filename);

public:
	// Program object for offscreen rendering for triangle picking
	GLuint mPickDrawProgram;

	// Uniform object for offscreen rendering for triangle picking
	GLuint mMVPpickUniform;

	// Corresponding renderer
	cls_renderer* mRenderer;

private:
	// Size of the output picture
	GLsizei mPicWidth;
	GLsizei mPicHeight;

	// Renderbuffer objects for color and depth and framebuffer object
	GLuint mRBOcolor;
	GLuint mRBOdepth;
	GLuint mFBO;

	// Container for pixels received from GPU
	GLubyte* mPixels;


};

#endif // CLS_OFFSCREEN_RENDERER_H
