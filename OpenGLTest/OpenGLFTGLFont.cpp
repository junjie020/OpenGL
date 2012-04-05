#include "stdafx.h"

#include "OpenGLRoot.h"
#include "OpenGLSceneMgr.h"
#include "OpenGLCamera.h"
#include "OpenGLRenderWin.h"
#include "OpenGLRenderUtilFunc.h"

#include "OpenGLFTGLFont.h"

#include "FTGL\ftgl.h"
#include "FTGL\FTBBox.h"
#include "FTGL\FTPoint.h"

#include "FTGL\FTGLOutlineFont.h"
#include "FTGL\FTGLPolygonFont.h"
#include "FTGL\FTGLBitmapFont.h"
#include "FTGL\FTGLTextureFont.h"
#include "FTGL\FTGLPixmapFont.h"



namespace OGLKit
{
	//virtual
	FTGLFontDrawingObj::FTGLFontDrawingObj()
		: m_pFontBuffer(nullptr)
		, m_FontBufSize(0)
	{
	}

	FTGLFontDrawingObj::~FTGLFontDrawingObj()
	{	
	}

#define FONT_FILE	"C:\\Windows\\Fonts\\Batang.ttc"

	bool FTGLFontDrawingObj::PrepareObj()
	{
		RenderWindow *pRenderWin = static_cast<RenderWindow*>(RenderRoot::GetSingleton().FindRenderTarget(TEXT("Default")));
		pRenderWin->AddKeyboardListener(this);
		m_pCamera->Position() = Unit3f(0.0, 0.0, 100.0f);

		if ( !generateFontBufferFromDC(NULL) )
		{
			return false;
		}

		return true;
	}

	FTFontPtr FTGLFontDrawingObj::createFont(FTGLFontType fontType)
	{
		switch( fontType )
		{
		case FTGLFT_Bitmap:
			return FTFontPtr(new FTGLBitmapFont(m_pFontBuffer.get(), m_FontBufSize));

		case FTGLFT_Pixmap:
			return FTFontPtr(new FTGLPixmapFont(m_pFontBuffer.get(), m_FontBufSize));
		
		case FTGLFT_Polgon:
			return FTFontPtr(new FTGLPolygonFont(m_pFontBuffer.get(), m_FontBufSize));

		case FTGLFT_Texture:
			return FTFontPtr(new FTGLTextureFont(m_pFontBuffer.get(), m_FontBufSize));

		default:
			return nullptr;
		}

		return nullptr;
	}

	FTFontPtr FTGLFontDrawingObj::createFontFromFile(FTGLFontType fontType)
	{
		switch( fontType )
		{
		case FTGLFT_Bitmap:
			return FTFontPtr(new FTGLBitmapFont(FONT_FILE));

		case FTGLFT_Pixmap:
			return FTFontPtr(new FTGLPixmapFont(FONT_FILE));

		case FTGLFT_Polgon:
			return FTFontPtr(new FTGLPolygonFont(FONT_FILE));

		case FTGLFT_Texture:
			return FTFontPtr(new FTGLTextureFont(FONT_FILE));

		default:
			return nullptr;
		}

		return nullptr;
	}

	bool FTGLFontDrawingObj::generateFontBufferFromDC(HDC hdc, bool bBold /*= false*/, bool bItalic /*= false*/
													, bool bUnderLine /*= false*/, bool bStrikeout /*= false*/)
	{
		HDC hdcNew = ::CreateCompatibleDC(hdc);
		LOGFONT lf = {0};
		HFONT hFont = reinterpret_cast<HFONT>(::GetCurrentObject(hdcNew, OBJ_FONT));

		::GetObject(hFont, sizeof(lf), &lf);

		StringUtils::StrCpy(TEXT("Batang"), lf.lfFaceName);

		lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
		lf.lfItalic = bItalic ? TRUE : FALSE;
		lf.lfUnderline = bUnderLine ? TRUE : FALSE;
		lf.lfStrikeOut = bStrikeout ? TRUE : FALSE;

		hFont = ::CreateFontIndirect(&lf);
		::SelectObject(hdcNew, hFont);

		TEXTMETRIC tm;
		::GetTextMetrics(hdcNew, &tm);

		if ( !(tm.tmPitchAndFamily & TMPF_TRUETYPE) )
		{
			return false;
		}
		


		const DWORD dwFontBufSize = ::GetFontData(hdcNew, 0, 0, nullptr, 0);
		if ( dwFontBufSize == GDI_ERROR )
			return false;

		m_FontBufSize = static_cast<size_t>(dwFontBufSize);
		m_pFontBuffer = BYTEPtr(new BYTE[m_FontBufSize]);
		const DWORD dwRet = ::GetFontData(hdcNew, 0, 0, m_pFontBuffer.get(), m_FontBufSize);

		if ( dwRet == GDI_ERROR )
			return false;

		::DeleteObject(hFont);
		::DeleteDC(hdcNew);

		return true;
	}

	FTFontPtr FTGLFontDrawingObj::getFont(FTGLFontType fontType, bool bBold /*= false*/, bool bItalic /*= false*/
										, bool bUnderLine /*= false*/, bool bStrikeout /*= false*/)
	{
		if ( !generateFontBufferFromDC(nullptr, bBold, bItalic, bUnderLine, bStrikeout) )
			return nullptr;

		return createFont(fontType);
	}

	//void FTGLFontDrawingObj::createFTFontsFromFileName(const stringA &fontFile)
	//{
	//	std::for_each(std::begin(m_Fonts), std::end(m_Fonts),
	//		[&](FTGLFonts::reference curFont)
	//	{
	//		curFont.second = createFont(curFont.first, fontFile);
	//	}
	//	);
	//}

	//void FTGLFontDrawingObj::createFTFontsUsingBuffer()
	//{
	//	if ( nullptr == m_pFontBuffer || 0 == m_FontBufSize )
	//		return ;

	//	std::for_each(std::begin(m_Fonts), std::end(m_Fonts),
	//		[&](FTGLFonts::reference curFont)
	//		{
	//			curFont.second = createFont(curFont.first, m_pFontBuffer.get(), m_FontBufSize);
	//		}
	//	);
	//}
		

	void FTGLFontDrawingObj::drawLine(const Unit3f &beg, const Unit3f &end)
	{
		glBegin(GL_LINE_LOOP);
		glVertex3fv(beg.GetData());
		glVertex3fv(end.GetData());
		glEnd();
	}

	void FTGLFontDrawingObj::drawLineWithNumAtTheEnd(const Unit3f &beg, const Unit3f &end, UINT numSize, FTFont &font)
	{
		drawLine(beg, end);
		//font.Render()
	}

	void FTGLFontDrawingObj::drawBox()
	{
		glColor3f(1.0, 0.0, 0.0);
		drawLine(Unit3f(0.0, 0.0, 0.0), Unit3f(1.0, 0.0, 0.0));	// x axis

		glColor3f(0.0, 1.0, 0.0);
		drawLine(Unit3f(0.0, 0.0, 0.0), Unit3f(0.0, 1.0, 0.0));	// y axis

		glColor3f(0.0, 0.0, 1.0);
		drawLine(Unit3f(0.0, 0.0, 0.0), Unit3f(0.0, 0.0, 1.0));	// z axis
	}
	
	void draw_box(const Point3D &lowerLeft, const Point3D &upRight)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glRectfv(lowerLeft.GetData(), upRight.GetData());
		glPopAttrib();
	}

	//virtual 
	bool FTGLFontDrawingObj::Draw()
	{
		glEnable(GL_TEXTURE_2D);

		//auto texFont = getFont(FTGLFT_Texture);

		UINT fontHeight = 12;
		//texFont->FaceSize(fontHeight);

		//if ( !texFont->Error() )
		//	cout << "After facesize error!!" << endl;

		//texFont->Render("g");

		Point3D leftBottomPt, rightTopPt;
		//texFont->BBox("g", leftUpPt.x, leftUpPt.y, leftUpPt.z, rightBottomPt.x, rightBottomPt.y, rightBottomPt.z);

		//glPushMatrix();
		//glTranslatef(rightBottomPt.x - leftUpPt.x + 10, 0.0, 0.0);
		//glRotatef(45.0f, 0.0, 1.0f, 0);
		//glRotatef(45.0f, 1.0, 0, 0);
		//texFont->Render("g");
		//glPopMatrix();

		//Unit3f curBegPt(0, -(rightBottomPt.y - leftUpPt.y + 10), 0);

		//glTranslatef(curBegPt.x, curBegPt.y, curBegPt.z);

		//auto polyFont = getFont(FTGLFT_Polgon);
		auto polyFont = createFontFromFile(FTGLFT_Polgon);
		polyFont->FaceSize(fontHeight);

		string str(TEXT("fgp1234"));

		polyFont->BBox(str.c_str(), leftBottomPt.x, leftBottomPt.y, leftBottomPt.z, rightTopPt.x, rightTopPt.y, rightTopPt.z);

		glColor3f(1.0, 1.0, 1.0);
		polyFont->Render(str.c_str());

		glColor3f(1.0, 0.0, 0.0);
		draw_box(leftBottomPt, rightTopPt);

		glPushMatrix();
		glTranslatef(rightTopPt.x - leftBottomPt.x, 0.0, 0.0);
		glColor3f(1.0, 1.0, 1.0);
		polyFont->Render(str.c_str());
		glColor3f(0.0, 1.0, 0.0);
		draw_box(leftBottomPt, rightTopPt);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0, rightTopPt.y - leftBottomPt.y, 0.0);
		glColor3f(1.0, 1.0, 1.0);
		polyFont->Render(str.c_str());
		glColor3f(0.0, 0.0, 1.0);
		draw_box(leftBottomPt, rightTopPt);
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

		return true;
	}

	static inline float GET_CAMERA_ADVANCE() { return 0.5f; }

	//virtual 
	bool FTGLFontDrawingObj::OnKeyUp(const KeyInfo &info)
	{
		return true;
	}

	//virtual 
	bool FTGLFontDrawingObj::OnKeyDown(const KeyInfo &info)
	{
		if ( info.keyDown == 'A' || info.keyDown == 'a' )
		{
			m_pCamera->Position().x -= GET_CAMERA_ADVANCE();
		}

		if ( info.keyDown == 'D' || info.keyDown == 'd' )
		{
			m_pCamera->Position().x += GET_CAMERA_ADVANCE();
		}

		if ( info.keyDown == 'Q' || info.keyDown == 'q' )
		{
			m_pCamera->Position().y += GET_CAMERA_ADVANCE();
		}

		if ( info.keyDown == 'E' || info.keyDown == 'e' )
		{
			m_pCamera->Position().y -= GET_CAMERA_ADVANCE();
		}

		if ( info.keyDown == 'W' || info.keyDown == 'w' )
		{
			m_pCamera->Position().z += GET_CAMERA_ADVANCE();
		}

		if ( info.keyDown == 'S' || info.keyDown == 's' )
		{
			m_pCamera->Position().z -= GET_CAMERA_ADVANCE();
		}

		return true;
	}
}