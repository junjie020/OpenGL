
#ifndef _OPENGLPBOOBJ_H_
#define _OPENGLPBOOBJ_H_
#include "OpenGLRenderableObj.h"
namespace OGLKit
{
	class PboDrawingObj : public RenderableObj
	{
	public:
		PboDrawingObj();
		static string GetObjName() { return STR_PBO_DRAWING_OBJ_NAME; }

		virtual string GetName() const{ return GetObjName(); }
		virtual bool PrepareObj();
		virtual bool Draw();

	private:
		GLuint	m_PBOID;

		GLuint	m_TexID;
	};
}

#endif	//_OPENGLPBOOBJ_H_