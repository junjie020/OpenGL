
#include "stdafx.h"

#include "OpenGLPBOObj.h"
namespace OGLKit
{
#define BUFFER_OFFSET(bytes)	((GLubyte*)NULL + (bytes))
	static const size_t IMAGE_WIDTH = 256;
	static const size_t IMAGE_HEIGHT = 256;

	GLubyte g_Image16[IMAGE_WIDTH][IMAGE_HEIGHT][3] = {0};

	PboDrawingObj::PboDrawingObj()
		: m_PBOID(0), m_TexID(0)
	{

	}

	//virtual 
	bool PboDrawingObj::PrepareObj()
	{
		for ( size_t i = 0; i < IMAGE_WIDTH; ++i )
		{
			for ( size_t j = 0; j < IMAGE_HEIGHT; ++j )
			{
#pragma warning(disable : 4554)
				int c = (((i&0x8)==0)^((j&0x8)==0) == 0) * 255;
#pragma warning(default: 4554)

				g_Image16[i][j][0] = c;
				g_Image16[i][j][1] = c;
				g_Image16[i][j][2] = c;
			}
		}

		GLenum	err = glGetError();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenBuffers(1, &m_PBOID);

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBOID);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, 3 * IMAGE_WIDTH * IMAGE_HEIGHT, g_Image16, GL_STATIC_DRAW);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

		err = glGetError();

		glEnable(GL_TEXTURE_2D);

		glGenTextures(1, &m_TexID);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_WIDTH, IMAGE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, g_Image16);

		//err = glGetError();

		//glBindTexture(GL_TEXTURE_2D, m_TexID);
		//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBOID);

		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		//err = glGetError();


		//
		//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		//glBindTexture(GL_TEXTURE_2D, 0);

		//err = glGetError();



		return true;
	}

	//virtual 
	bool PboDrawingObj::Draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glTranslatef(0.0, 0.0, -4.5);
		//gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		//glRasterPos2i(0, 0);
		//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBOID);
		//glDrawPixels(IMAGE_WIDTH, IMAGE_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
		//glLoadIdentity();

		//glColor3i(255, 0, 0);
		////gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		//glTranslatef(0.0, 0.0, -1.0);
		//glBegin(GL_LINE_STRIP);
		//	glVertex3d(0.0, 0.0, 0.0);
		//	glVertex3d(1.0, 0.0, 0.0);
		//glEnd();



		//glBegin(GL_TRIANGLES);								// Drawing Using Triangles
		//glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
		//glVertex3f( 0.0f, 1.0f, 0.0f);					// Top
		//glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
		//glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
		//glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		//glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
		//glEnd();											// Finished Drawing The Triangle
		//glTranslatef(3.0f,0.0f,0.0f);						// Move Right 3 Units
		//glColor3f(0.5f,0.5f,1.0f);							// Set The Color To Blue One Time Only
		//glBegin(GL_QUADS);									// Draw A Quad
		//glVertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
		//glVertex3f( 1.0f, 1.0f, 0.0f);					// Top Right
		//glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
		//glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
		//glEnd();											// Done Drawing The Quad

		//glBindTexture(GL_TEXTURE_2D, m_TexID);

		//glBegin(GL_QUADS);									// Draw A Quad
		//glTexCoord2f(0.0, 0.0);		glVertex3f(-1.0f, 1.0f, 0.0f);
		//glTexCoord2f(1.0, 0.0);		glVertex3f( 1.0f, 1.0f, 0.0f);
		//glTexCoord2f(1.0, 1.0);		glVertex3f( 1.0f,-1.0f, 0.0f);
		//glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0f,-1.0f, 0.0f);
		//glEnd();											// Done Drawing The Quad


		const GLfloat vertexes[][3] = 
		{
			{0.0, 0.0, 0.0}, 
			{1.0, 0.0, 0.0},
			{1.0, 1.0, 0.0},
			{0.0, 1.0, 0.0}
		};
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3fv(vertexes[0]);
		glVertex3fv(vertexes[1]);
		glVertex3fv(vertexes[2]);
		glVertex3fv(vertexes[3]);
		glEnd();

		return true;
	}
}