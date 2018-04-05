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

	void Construct(void);

	void Resize(GLsizei p_width, GLsizei p_height);

	void RenderModelToBuffer(cls_scene* p_scene);

	void WritePNGfile(const char* p_filename);

private:
	// Corresponding renderer
	cls_renderer* mRenderer;

	// Size of the output picture
	GLsizei mPicWidth;
	GLsizei mPicHeight;

	// Container for pixels received from GPU
	GLubyte* mPixels;

	// Renderbuffer objects for color and depth and framebuffer object
	GLuint mRBOcolor;
	GLuint mRBOdepth;
	GLuint mFBO;

};

#endif // CLS_OFFSCREEN_RENDERER_H
