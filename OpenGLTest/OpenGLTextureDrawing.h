#ifndef _OPENGLTEXTUREDRAWING_H_
#define _OPENGLTEXTUREDRAWING_H_

#include "OpenGLRenderableObj.h"
#include "OpenGLKeyboardCtrl.h"
namespace OGLKit
{
	class TextureDrawingObj : public RenderableObj, public KeyboardCtrl
	{
	public:
		TextureDrawingObj();
		virtual string	GetName()const;
		virtual bool	Draw();
		virtual bool	PrepareObj();

		virtual bool	OnKeyUp(const KeyInfo &info);
		virtual bool	OnKeyDown(const KeyInfo &info);
	public:
		static string	GetObjName();

	private:
		GLuint			m_TextureID;
		GLuint			m_TextureIDNoMipmap;
	};
}
#endif	//_OPENGLTEXTUREDRAWING_H_