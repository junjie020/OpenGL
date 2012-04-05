#ifndef _OPENGLPOLYGONSTIPPLE_H_
#define _OPENGLPOLYGONSTIPPLE_H_

#include "OpenGLRenderableObj.h"


namespace OGLKit
{
	class PolygonStippleDrawObj : public RenderableObj
	{
	public:
		static string GetObjName() { return STR_POLYGON_STIPPLE_NAME; }

		virtual string GetName() const{ return GetObjName(); }
		virtual bool PrepareObj();
		virtual bool Draw();


	};
}


#endif	//_OPENGLPOLYGONSTIPPLE_H_