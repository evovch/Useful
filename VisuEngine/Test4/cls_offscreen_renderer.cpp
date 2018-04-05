#include "cls_offscreen_renderer.h"

// PNG
#include <libpng/png.h>

// Project
#include "cls_renderer.h"
#include "cls_scene.h"

#define NUMOFCOMPONENTS 4

// Constructor with default picture size 2000x2000
cls_offscreen_renderer::cls_offscreen_renderer(cls_renderer* p_renderer) :
    mRenderer(p_renderer),
	mPicWidth(2000),
	mPicHeight(2000),
	mPixels(nullptr)
{
	this->Construct();
}

cls_offscreen_renderer::cls_offscreen_renderer(cls_renderer* p_renderer, GLsizei p_width, GLsizei p_height) :
    mRenderer(p_renderer),
	mPicWidth(p_width),
	mPicHeight(p_height),
	mPixels(nullptr)
{
	this->Construct();
}

cls_offscreen_renderer::~cls_offscreen_renderer(void)
{
	glDeleteRenderbuffers(1, &mRBOcolor);
	glDeleteRenderbuffers(1, &mRBOdepth);
	glDeleteFramebuffers(1, &mFBO);

	if (mPixels) delete [] mPixels;
}

void cls_offscreen_renderer::Construct(void)
{
	// Generate a renderbuffer for color and set its size
	glGenRenderbuffers(1, &mRBOcolor);
	glBindRenderbuffer(GL_RENDERBUFFER, mRBOcolor);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, mPicWidth, mPicHeight);

	// Generate a renderbuffer for depth and set its size
	glGenRenderbuffers(1, &mRBOdepth);
	glBindRenderbuffer(GL_RENDERBUFFER, mRBOdepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mPicWidth, mPicHeight);

	// Generate a framebuffer and connect it with previously generated renderbuffer
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mRBOcolor);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBOdepth);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mPixels = new GLubyte[mPicWidth * mPicHeight * NUMOFCOMPONENTS];
}

void cls_offscreen_renderer::Resize(GLsizei p_width, GLsizei p_height)
{
	mPicWidth = p_width;
	mPicHeight = p_height;

	glDeleteRenderbuffers(1, &mRBOcolor);
	glDeleteRenderbuffers(1, &mRBOdepth);
	glDeleteFramebuffers(1, &mFBO);

	if (mPixels) delete [] mPixels;

	this->Construct();
}

void cls_offscreen_renderer::RenderModelToBuffer(cls_scene* p_scene)
{
	// Switch to offscreen buffer and set corresponding window size and viewport
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	p_scene->Draw(mRenderer);

	// Read data from the offscreen buffer
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, mPicWidth, mPicHeight, GL_RGBA, GL_UNSIGNED_BYTE, mPixels);

	// Switch back to screen rendering and restore windows size and viewport
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void cls_offscreen_renderer::WritePNGfile(const char* p_filename)
{
	png_bytep* row_pointers = new png_bytep[mPicHeight];

	//row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * mPicHeight);

	for (GLsizei i=0; i<mPicHeight; i++) {
		//row_pointers[i] = (png_byte*) malloc(sizeof(png_byte) * mPicWidth * NUMOFCOMPONENTS);

		row_pointers[i] = new png_byte[mPicWidth * NUMOFCOMPONENTS];

		memcpy(row_pointers[i], &mPixels[(mPicHeight-i-1)*mPicWidth*NUMOFCOMPONENTS], mPicWidth*NUMOFCOMPONENTS);
	}

	/* create file */
	FILE* fp = fopen(p_filename, "wb");

	//TODO check that fp is not NULL

	png_structp png_ptr;
	png_infop info_ptr;
	png_byte color_type = PNG_COLOR_TYPE_RGBA;
	png_byte bit_depth = 8;

	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);
	setjmp(png_jmpbuf(png_ptr));
	png_init_io(png_ptr, fp);
	/* write header */
	setjmp(png_jmpbuf(png_ptr));
	png_set_IHDR(png_ptr, info_ptr, mPicWidth, mPicHeight,
		bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);
	/* write bytes */
	setjmp(png_jmpbuf(png_ptr));
	png_write_image(png_ptr, row_pointers);
	/* end write */
	setjmp(png_jmpbuf(png_ptr));
	png_write_end(png_ptr, NULL);

	/* close file */
	fclose(fp);

	// Cleanup
	//for (int i=0; i<mPicHeight; i++) free(row_pointers[i]);
	//free(row_pointers);
	for (GLsizei i=0; i<mPicHeight; i++) delete [] row_pointers[i];
	delete [] row_pointers;
}
