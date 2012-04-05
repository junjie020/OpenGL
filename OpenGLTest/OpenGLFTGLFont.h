#ifndef _OPENGLFTGLFONT_H_
#define _OPENGLFTGLFONT_H_

#include "OpenGLRenderableObj.h"
#include "OpenGLKeyboardCtrl.h"

class FTFont;

namespace OGLKit
{
	class SceneManager;

	typedef std::shared_ptr<FTFont>	FTFontPtr;

	class FTGLFontDrawingObj : public RenderableObj, public KeyboardCtrl
	{
	public:
		FTGLFontDrawingObj();

		~FTGLFontDrawingObj();

		static string	GetObjName() {return STR_FTGL_DRAWING_OBJ_NAME;}
		virtual string	GetName() const{ return FTGLFontDrawingObj::GetObjName();}

		virtual bool	PrepareObj();

		virtual bool	Draw();

		virtual bool	OnKeyUp(const KeyInfo &info);
		virtual bool	OnKeyDown(const KeyInfo &info);

	private:
		enum FTGLFontType
		{
			FTGLFT_Bitmap = 0,
			FTGLFT_Pixmap,
			FTGLFT_Polgon,
			FTGLFT_Texture,

			FTGLFT_Total,
		};
		FTFontPtr		createFont(FTGLFontType fontType);
		FTFontPtr		createFontFromFile(FTGLFontType fontType);
	
		bool			generateFontBufferFromDC(HDC hdc, bool bBold = false, bool bItalic = false
												, bool bUnderLine = false, bool bStrikeout = false);

		FTFontPtr		getFont(FTGLFontType fontType, bool bBold = false, bool bItalic = false
							, bool bUnderLine = false, bool bStrikeout = false);


		void			drawLine(const Unit3f &beg, const Unit3f &end);

		void			drawLineWithNumAtTheEnd(const Unit3f &beg, const Unit3f &end, UINT numSize, FTFont &font);

		void			drawBox();

	private:
		Unit3f			m_camPos;
		Unit3f			m_camLookAt;


		typedef std::shared_ptr<BYTE>	BYTEPtr;
		BYTEPtr			m_pFontBuffer;
		DWORD			m_FontBufSize;
	};
}
#endif	//_OPENGLFTGLFONT_H_