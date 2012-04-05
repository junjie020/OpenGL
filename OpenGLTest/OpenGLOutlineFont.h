#ifndef _OPENGLOUTLINEFONT_H_
#define _OPENGLOUTLINEFONT_H_

#include "OpenGLRenderableObj.h"

namespace OGLKit
{
	class OutlineFontDrawObj : public RenderableObj
	{
	public:
		OutlineFontDrawObj();
		~OutlineFontDrawObj();

		static string GetObjName();
		virtual string GetName() const;

		virtual bool PrepareObj();

		virtual bool Draw();

	private:
		void	BuildFont();
		void	Print(const string &strText);
	private:
		HDC	m_hDC;

		GLuint m_FontListBase;
		GLYPHMETRICSFLOAT m_gmf[256];		// Storage For Information About Our Outline Font Character


		class BitmapFontTest
		{
		public:
			BitmapFontTest();
			~BitmapFontTest();


		private:
			GLuint m_BitmapFontListBase;
		};

		//BitmapFontTest m_bmpFOnt;
	};
}

#endif	//_OPENGLOUTLINEFONT_H_