#include "stdafx.h"

#include "OpenGLShadowMap.h"
#include "oglext/glut.h"

#define SHADOW_MAP_WIDTH      256
#define SHADOW_MAP_HEIGHT     256

GLdouble    fovy      = 60.0;
GLdouble    nearPlane = 10.0;
GLdouble    farPlane  = 100.0;

GLfloat     angle = 0.0;
GLfloat     torusAngle = 0.0;

GLfloat     lightPos[] = { 25.0, 25.0, 25.0, 1.0 };
GLfloat     lookat[] = { 0.0, 0.0, 0.0 };
GLfloat     up[] = { 0.0, 0.0, 1.0 };

GLboolean showShadow = GL_FALSE;

void init( void )
{
	GLfloat  white[] = { 1.0, 1.0, 1.0, 1.0 };

	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
		SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );

	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
	glLightfv( GL_LIGHT0, GL_SPECULAR, white );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, white );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
	glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );

	glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	glTexGeni( GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );

	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

	glCullFace( GL_BACK );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_TEXTURE_GEN_S );
	glEnable( GL_TEXTURE_GEN_T );
	glEnable( GL_TEXTURE_GEN_R );
	glEnable( GL_TEXTURE_GEN_Q );
	glEnable( GL_COLOR_MATERIAL );
	glEnable( GL_CULL_FACE );
}

void reshape( int width, int height )
{
	glViewport( 0, 0, width, height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( fovy, (GLdouble) width/height, nearPlane, farPlane );
	glMatrixMode( GL_MODELVIEW );
}

void idle( void )
{
	angle += GLfloat(PI / 10000);
	torusAngle += 0.1f;
}


void keyboard( unsigned char key, int x, int y )
{
	switch( key ) 
	{
	case 27:  /* Escape */
		exit( 0 );
		break;

	case 't': 
		{
			static GLboolean textureOn = GL_TRUE;
			textureOn = !textureOn;

			if ( textureOn )
				glEnable( GL_TEXTURE_2D );
			else
				glDisable( GL_TEXTURE_2D );
		}
		break;

	case 'm': 
		{
			static GLboolean compareMode = GL_TRUE;
			compareMode = !compareMode;
			printf( "Compare mode %s\n", compareMode ? "On" : "Off" );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
				compareMode ? GL_COMPARE_R_TO_TEXTURE : GL_NONE );
		}
		break;

	case 'f': 
		{
			static GLboolean funcMode = GL_TRUE;
			funcMode = !funcMode;
			printf( "Operator %s\n", funcMode ? "GL_LEQUAL" : "GL_GEQUAL" );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,
				funcMode ? GL_LEQUAL : GL_GEQUAL );
		}

		break;

	case 's':
		showShadow = !showShadow;
		break;

	case 'p': 
		{
			static GLboolean  animate = GL_TRUE;
			animate = !animate;
		}

		break;
	}
}


void transposeMatrix( GLfloat m[16] )
{
	std::swap( m[1],  m[4]  );
	std::swap( m[2],  m[8]  );
	std::swap( m[3],  m[12] );
	std::swap( m[6],  m[9]  );
	std::swap( m[7],  m[13] );
	std::swap( m[11], m[14] );
}

void drawObjects( GLboolean shadowRender )
{
	GLboolean textureOn = glIsEnabled( GL_TEXTURE_2D );

	if ( shadowRender )
		glDisable( GL_TEXTURE_2D );

	if ( !shadowRender )
	{
		glNormal3f( 0, 0, 1 );
		glColor3f( 1, 1, 1 );
		glRectf( -20.0, -20.0, 20.0, 20.0 );
	}

	glPushMatrix();

		glTranslatef( 11, 11, 11 );
		glRotatef( 54.73f, -5, 5, 0 );
		glRotatef( torusAngle, 1, 0, 0 );
		glColor3f( 1, 0, 0 );
		glutSolidTorus( 1, 4, 8, 36 );

	glPopMatrix();

	glPushMatrix();

		glTranslatef( 2, 2, 2 );
		glColor3f( 0, 0, 1 );
		glutSolidCube( 4 );

	glPopMatrix();

	glPushMatrix();

		glTranslatef( lightPos[0], lightPos[1], lightPos[2] );
		glColor3f( 1, 1, 1 );
		glutWireSphere( 0.5, 6, 6 );

	glPopMatrix();

	if ( shadowRender && textureOn ) 
		glEnable( GL_TEXTURE_2D );
}

void generateShadowMap( void )
{
	GLint    viewport[4];
	GLfloat  lightPos[4];

	glGetLightfv( GL_LIGHT0, GL_POSITION, lightPos );
	glGetIntegerv( GL_VIEWPORT, viewport );

	glViewport( 0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluPerspective( 80.0, 1.0, 10.0, 1000.0 );
	glMatrixMode( GL_MODELVIEW );

	glPushMatrix();
	glLoadIdentity();
	gluLookAt( lightPos[0], lightPos[1], lightPos[2],
		lookat[0], lookat[1], lookat[2],
		up[0], up[1], up[2] );

	drawObjects( GL_TRUE );

	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );

	glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0 );

	glViewport( viewport[0], viewport[1], viewport[2], viewport[3] );

	if ( showShadow ) 
	{
		GLfloat depthImage[SHADOW_MAP_WIDTH][SHADOW_MAP_HEIGHT];
		glReadPixels( 0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, depthImage );

		glWindowPos2f( viewport[2]/2.0f, 0 );
		glDrawPixels( SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, GL_LUMINANCE, GL_FLOAT, depthImage );
		//      glutSwapBuffers();
	}
}

void generateTextureMatrix( void )
{
	GLfloat  tmpMatrix[16];

	/*
	*  Set up projective texture matrix.  We use the GL_MODELVIEW matrix
	*    stack and OpenGL matrix commands to make the matrix.
	*/
	glPushMatrix();
	glLoadIdentity();
	glTranslatef( 0.5, 0.5, 0.0 );
	glScalef( 0.5, 0.5, 1.0 );
	gluPerspective( 60.0, 1.0, 1.0, 1000.0 );
	gluLookAt(	lightPos[0], lightPos[1], lightPos[2],
				lookat[0], lookat[1], lookat[2],
				up[0], up[1], up[2] );
	glGetFloatv( GL_MODELVIEW_MATRIX, tmpMatrix );
	glPopMatrix();

	transposeMatrix( tmpMatrix );

	glTexGenfv( GL_S, GL_OBJECT_PLANE, &tmpMatrix[0] );
	glTexGenfv( GL_T, GL_OBJECT_PLANE, &tmpMatrix[4] );
	glTexGenfv( GL_R, GL_OBJECT_PLANE, &tmpMatrix[8] );
	glTexGenfv( GL_Q, GL_OBJECT_PLANE, &tmpMatrix[12] );
}

void display( void )
{
	GLfloat  radius = 30;

	generateShadowMap();
	generateTextureMatrix();

	if ( showShadow )
		return ;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();

	gluLookAt(	radius * cos(angle), radius * sin(angle), 30, 
				lookat[0], lookat[1], lookat[2], 
				up[0], up[1], up[2] );

	drawObjects( GL_FALSE );


	glPopMatrix();

	//    glutSwapBuffers();
}

//int main( int argc, char** argv )
//{
//    glutInit( &argc, argv );
//    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
//    glutInitWindowSize( 512, 512 );
//    glutInitWindowPosition( 100, 100 );
//    glutCreateWindow( argv[0] );
//
//    init();
//
//    glutDisplayFunc( display );
//    glutReshapeFunc( reshape );
//    glutKeyboardFunc( keyboard );
//    glutIdleFunc( idle );
//
//    glutMainLoop();
//
//    return 0;
//}



	namespace OGLKit
{

}