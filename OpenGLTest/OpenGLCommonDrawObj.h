#ifndef _OPENGLCOMMONDRAWOBJ_H_
#define _OPENGLCOMMONDRAWOBJ_H_

#include "OpenGLRenderableObj.h"

namespace OGLKit
{
	class CommonDrawObj : public RenderableObj
	{
	public:
		CommonDrawObj();
		~CommonDrawObj();

		static string GetObjName();

		virtual string GetName() const;
		virtual bool PrepareObj();
		virtual bool Draw();	
	};
}

#endif	//_OPENGLCOMMONDRAWOBJ_H_