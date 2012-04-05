#include "stdafx.h"

#include "OpenGLFrame.h"
#include "OpenGLRenderWin.h"

extern void _TRACE(const char* lpcszData);

int main()
{
	OGLKit::OpenGLFramework openGLFrame;

	openGLFrame.InitFramework();
	openGLFrame.CreateRenderWin(TEXT("Kit OpenGL Framework!"), 600, 480);

	if ( !openGLFrame.BuildSceneObj() )
		return -1;

	openGLFrame.MessageLoop();
	return (0);
}
