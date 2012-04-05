#include "stdafx.h"
#include "OpenGLRenderWin.h"
#include "OpenGLFrame.h"
#include "OpenGLFrameBufferObj.h"


namespace OGLKit
{
	enum
	{
		Color = 0,
		Depth,
		Totals,
	};


	GLuint framebuf, renderbuf[Totals];


	//virtual 
	bool FrameBufferDrawObj::PrepareObj()
	{
		glGenRenderbuffers(Totals, renderbuf);

		glBindRenderbuffer(GL_RENDERBUFFER, renderbuf[Color]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 256, 256);

		glBindRenderbuffer(GL_RENDERBUFFER, renderbuf[Depth]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 256, 256);

		glGenFramebuffers(1, &framebuf);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuf);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuf[Color]);

		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuf[Depth]);

		glEnable(GL_DEPTH_TEST);

		return true;
	}

	bool checkFramebufferStatus()
	{
		// check FBO status
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
		switch(status)
		{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			std::cout << "Framebuffer complete." << std::endl;
			return true;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
			return false;

		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			std::cout << "[ERROR] Unsupported by FBO implementation." << std::endl;
			return false;

		default:
			std::cout << "[ERROR] Unknow error." << std::endl;
			return false;
		}
	}

	//virtual 
	bool FrameBufferDrawObj::Draw()
	{

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuf);
		glViewport(0, 0, 256, 256);

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glColor3ub(255, 0, 0);
		glRectf(0.0, 0.0, 2.0, 2.0);

		checkFramebufferStatus();

		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuf);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);


		checkFramebufferStatus();

		glViewport(0, 0, 600, 480);

		glClearColor(0.0, 1.0, 1.0, 1.0);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		checkFramebufferStatus();

		glBlitFramebuffer(0, 0, 255, 255, 0, 0, 255, 200, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		checkFramebufferStatus();
		return true;
	}
}