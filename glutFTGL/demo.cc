#include <stdlib.h> // exit()
#include <Windows.h>
#include <iostream>
using namespace std;
#include <GL/gl.h>


#ifdef	_DEBUG
#pragma comment(lib, "ftgl_d.lib")
#else	//_DEBUG
#pragma comment(lib, "ftgl.lib")
#endif	//_DEBUG

#include "glut.h"

#include "FTGLOutlineFont.h"
#include "FTGLPolygonFont.h"
#include "FTGLBitmapFont.h"
#include "FTGLTextureFont.h"
#include "FTGLPixmapFont.h"

static FTFont* fonts[5];

void my_init( const char* font_filename )
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	fonts[0] = new FTGLOutlineFont(font_filename);
	fonts[1] = new FTGLPolygonFont(font_filename);
	fonts[2] = new FTGLTextureFont(font_filename);
	fonts[3] = new FTGLBitmapFont(font_filename);
	fonts[4] = new FTGLPixmapFont(font_filename);
	for (int i=0; i< 5; i++) 
	{
		if (fonts[i]->Error()) 
		{
			cerr << "ERROR: Unable to open file " << font_filename << "\n";
		}
		else 
		{
			const int point_size = 18;
			if (!fonts[i]->FaceSize(point_size)) 
			{
				cerr << "ERROR: Unable to set font face size " << point_size << "\n";
			}
		}
	}
}


void my_reshape(int width, int height)
{
	if (height == 0)
	{
		height = 1;										
	}
	glViewport(0, 0, width, height);

	// We are going to do some 2-D orthographic drawing.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 1000.0f);

	// Now determine where to draw things.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void draw_scene()
{
	/* Set up some strings with the characters to draw. */
	unsigned int count = 0;
	char string[8][256];
	int i;
	for (i=1; i < 32; i++) { /* Skip zero - it's the null terminator! */
		string[0][count] = i;
		count++;
	}
	string[0][count] = '\0';

	count = 0;
	for (i=32; i < 64; i++) {
		string[1][count] = i;
		count++;
	}
	string[1][count] = '\0';

	count = 0;
	for (i=64; i < 96; i++) {
		string[2][count] = i;
		count++;
	}
	string[2][count] = '\0';

	count = 0;
	for (i=96; i < 128; i++) {
		string[3][count] = i;
		count++;
	}
	string[3][count] = '\0';

	count = 0;
	for (i=128; i < 160; i++) {
		string[4][count] = i;
		count++;
	}
	string[4][count] = '\0';

	count = 0;
	for (i=160; i < 192; i++) {
		string[5][count] = i;
		count++;
	}
	string[5][count] = '\0';

	count = 0;
	for (i=192; i < 224; i++) {
		string[6][count] = i;
		count++;
	}
	string[6][count] = '\0';

	count = 0;
	for (i=224; i < 256; i++) {
		string[7][count] = i;
		count++;
	}
	string[7][count] = '\0';

	gluLookAt(0.0, 0.0, 1000, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glColor3f(1.0, 0.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	{
		glTranslatef(-80.0, 0, 0.0);

		fonts[2]->Render(string[2]);
	}
	glDisable(GL_TEXTURE_2D);
}


void my_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	draw_scene();

	glutSwapBuffers();
}

#define FONT_FILE	"C:\\Windows\\Fonts\\Arial.ttf"

int main(int argc, char **argv)
{

	glutInitWindowSize(600, 600);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);

	glutCreateWindow("FTGL demo");

	my_init(FONT_FILE);

	glutDisplayFunc(my_display);
	glutReshapeFunc(my_reshape);
	
	glutMainLoop();
	
	exit(1);
}




