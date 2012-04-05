#include <memory>
#include <cstdio>
#include "oglext/glut.h"

int board[3][3];

void init()
{
	memset(board, 0, sizeof(board));
}

void drawSquares(GLenum mode)
{
	for ( size_t ii = 0; ii < 3; ++ii )
	{
		if ( mode == GL_SELECT )
			glLoadName(ii);

		for ( size_t jj = 0; jj < 3; ++jj )
		{
			if ( mode == GL_SELECT )
				glPushName(jj);

			glColor3f(GLfloat(ii/3.0), GLfloat(jj/3.0), GLfloat(board[ii][jj]/3.0));

			glRecti(ii, jj, ii+1, jj+1);
			if ( mode == GL_SELECT )
				glPopName();
		}
	}
}

void processHits(GLint hits, GLuint buffer[])
{
	printf("hints = %d\n", hits);

	GLuint *ptr = buffer;

	size_t boardIdxI, boardIdxJ;
	for ( int i = 0; i < hits; ++i )
	{
		GLuint names = *ptr;
		printf( " number of names for this hit = %d\n", names );
		ptr++;
		printf( " z1 is %g;", (float)*ptr/0x7ffffff); ptr++;
		printf( " z2 is %g;", (float)*ptr/0x7ffffff); ptr++;

		printf( "   names are ");
		for ( size_t j = 0; j < names; j++ )
		{
			printf( "%d ", *ptr);
			if ( j == 0 )
				boardIdxI = *ptr;
			else if ( j == 1 )
				boardIdxJ = *ptr;

			ptr++;
		}

		printf("\n");
		board[boardIdxI][boardIdxJ] = (board[boardIdxI][boardIdxJ] + 1)%3;
	}
}

#define BUFSIZE 512

void pickSquares(int button, int state, int x, int y)
{
	if ( button != GLUT_LEFT_BUTTON || state != GLUT_DOWN )
		return;

	GLuint selectBuf[BUFSIZ];
	glSelectBuffer(BUFSIZE, selectBuf);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix(GLdouble(x), GLdouble(viewport[3] - y), 5.0, 5.0, viewport);
	gluOrtho2D(0.0, 6.0, 0.0, 6.0);

	drawSquares(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glFlush();
	GLint hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawSquares(GL_RENDER);
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 3.0, 0.0, 3.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(100, 100);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutMouseFunc(pickSquares);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return (0);
}