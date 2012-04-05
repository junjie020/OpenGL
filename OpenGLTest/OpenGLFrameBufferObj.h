#ifndef _OPENGLFRAMEBUFFEROBJ_H_
#define _OPENGLFRAMEBUFFEROBJ_H_

#include "OpenGLRenderableObj.h"

namespace OGLKit
{
	class FrameBufferDrawObj : public RenderableObj
	{
	public:
		static string GetObjName() { return STR_FBO_NAME; }
		
		virtual string GetName() const{ return GetObjName(); }

		virtual bool PrepareObj();

		virtual bool Draw();
	};
}

#endif	//_OPENGLFRAMEBUFFEROBJ_H_
