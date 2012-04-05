#include "stdafx.h"

#include "OpenGLCommonDrawObj.h"
#include "OpenGLRenderUtilFunc.h"


namespace OGLKit
{
	CommonDrawObj::CommonDrawObj()
	{

	}

	CommonDrawObj::~CommonDrawObj()
	{

	}

	//static 
	string CommonDrawObj::GetObjName()
	{
		return STR_COMMON_DRAWING_OBJ_NAME;
	}

	//virtual 
	string CommonDrawObj::GetName() const
	{
		return CommonDrawObj::GetObjName();
	}

	GLuint listID;

	//virtual 
	bool CommonDrawObj::PrepareObj()
	{		
		return true;
	}

	void rect_stipple_draw()
	{
		glClear (GL_COLOR_BUFFER_BIT);

		const float rectHeight = 2.0f;
		const float rectWidth = 2.0f;
	}

	void display(void)
	{
		const BYTE dwFirst	= 0xF0;
		const BYTE dwSecond	= 0xF0;

		GLubyte fly[] = {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

			0x03, 0x80, 0x01, 0xC0, 0x06, 0xC0, 0x03, 0x60,

			0x04, 0x60, 0x06, 0x20, 0x04, 0x30, 0x0C, 0x20,

			0x04, 0x18, 0x18, 0x20, 0x04, 0x0C, 0x30, 0x20,

			0x04, 0x06, 0x60, 0x20, 0x44, 0x03, 0xC0, 0x22,

			0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,

			0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,

			0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,

			0x66, 0x01, 0x80, 0x66, 0x33, 0x01, 0x80, 0xCC,

			0x19, 0x81, 0x81, 0x98, 0x0C, 0xC1, 0x83, 0x30,

			0x07, 0xe1, 0x87, 0xe0, 0x03, 0x3f, 0xfc, 0xc0,

			0x03, 0x31, 0x8c, 0xc0, 0x03, 0x33, 0xcc, 0xc0,

			0x06, 0x64, 0x26, 0x60, 0x0c, 0xcc, 0x33, 0x30,

			0x18, 0xcc, 0x33, 0x18, 0x10, 0xc4, 0x23, 0x08,

			0x10, 0x63, 0xC6, 0x08, 0x10, 0x30, 0x0c, 0x08,

			0x10, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00, 0x08

		};
		GLubyte halftone[] = {
			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond,

			dwFirst, dwFirst, dwFirst, dwFirst, dwSecond, dwSecond, dwSecond, dwSecond

		};

		//glFlush ();

	}

	//virtual 
	bool CommonDrawObj::Draw()
	{
		return true;
	}
}