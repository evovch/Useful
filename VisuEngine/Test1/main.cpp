#include <stdio.h>

#include <GL/glew.h>
#include <GL/glut.h>

static void KeyFunc(unsigned char key, int /*x*/, int /*y*/)
{
	switch (key) {
	case 27:
		//Esc key
		exit(0);
	}
}

static void DrawFunc(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutCreateWindow("Test");

	GLenum err = glewInit();

	if (GLEW_OK != err) {
		// Problem: glewInit failed, something is seriously wrong.
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glutKeyboardFunc(KeyFunc);
	glutDisplayFunc(DrawFunc);
	glutMainLoop();

	return 0;
}
