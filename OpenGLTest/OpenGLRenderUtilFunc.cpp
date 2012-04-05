#include "stdafx.h"

#include "OpenGLRenderUtilFunc.h"

namespace OGLKit
{
	//static 
	void RenderUtilFuncs::DrawTriangle(bool bColor /*= false*/, const Point3D &pt0 /*= Point3D(0, 0, 0)*/, const Point3D &pt1 /*= Point3D(0, 1, 0)*/, const Point3D &pt2 /*= Point3D(1, 1, 0)*/)
	{
		Point3D vertexes[3] = {pt0, pt1, pt2};

		Color3D clr[3] = { Color3D(255, 0, 0), Color3D(255, 255, 0), Color3D(255, 0, 255)};

		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertexes[0].GetData());
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, &clr[0].r);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	//static 
	void RenderUtilFuncs::DrawCube( bool bColor /*= false */)
	{
		Point3D vertexes[8];

		const float cubeLen = 2.0f;

		vertexes[0] = Point3D(0.0, 0.0, 0.0);
		vertexes[1] = Point3D(cubeLen, 0.0, 0.0);
		vertexes[2] = Point3D(cubeLen, 0.0, cubeLen);
		vertexes[3] = Point3D(0.0, 0.0, cubeLen);

		vertexes[4] = Point3D(0.0, cubeLen, 0.0);
		vertexes[5] = Point3D(0.0, cubeLen, cubeLen);
		vertexes[6] = Point3D(cubeLen, cubeLen, cubeLen);
		vertexes[7] = Point3D(cubeLen, cubeLen, 0.0);

		Color3D clrs[8];
		clrs[0] = Color3D(255, 0, 0);
		clrs[1] = Color3D(255, 255, 0);
		clrs[2] = Color3D(255, 0, 255);
		clrs[3] = Color3D(0, 255, 0);
		clrs[4] = Color3D(0, 255, 255);
		clrs[5] = Color3D(0, 0, 255);
		clrs[6] = Color3D(0, 0, 0);
		clrs[7] = Color3D(255, 255, 255);

		GLuint indexes[24] = 
		{
			0, 1, 2, 3,	// bottom
			4, 5, 6, 7, // top

			0, 3, 5, 4, // left
			1, 7, 6, 2, // right

			3, 5, 6, 2, // front
			0, 4, 7, 1, // back
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		if ( bColor )
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_UNSIGNED_BYTE, 0, &clrs[0].r);
		}

		glVertexPointer(3, GL_FLOAT, 0, vertexes[0].GetData());
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, indexes);

		if ( bColor )
			glDisableClientState(GL_COLOR_ARRAY);

		glDisableClientState(GL_VERTEX_ARRAY);

	}
}