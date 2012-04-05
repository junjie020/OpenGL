#include "stdafx.h"

#include "OpenGLDef.h"
#include "OpenGLOutlineFont.h"
#include "OpenGLRenderUtilFunc.h"

namespace OGLKit
{
	inline DWORD	FIXEDFACTOR() { return 0x10000; }
	inline long		FixedToLong(FIXED fx)	{return *(long*) &fx;}

	inline double	FixedToDouble(FIXED fx)			{return (double) FixedToLong(fx) / FIXEDFACTOR();}
	inline float	FixedToFloat(FIXED fx)			{return (float) FixedToLong(fx) / FIXEDFACTOR();}

	template<class TYPE>
	inline void		FixedToTYPE(FIXED fx, TYPE &tt)	{ tt = ((TYPE)FixedToLong(fx)) / FIXEDFACTOR();}

	template<class PTTYPE>
	inline void		FixedToPoint(const POINTFX &ptFX, PTTYPE &tt)	{ FixedToTYPE(ptFX.x, tt.x); FixedToTYPE(ptFX.y, tt.y); }

	typedef Point3DArray ContourPtArray;
	typedef std::vector<ContourPtArray>	PolygonPtArray;

	class ParsingGlyphOutLine
	{
	public:
		ParsingGlyphOutLine(CHAR_T ch);
		~ParsingGlyphOutLine();

		void GetPolygonPts(PolygonPtArray &pts);

	private:
		bool Init(CHAR_T ch);
		bool InitGlyphBuffer(HDC hDC, CHAR_T ch);

		bool GetDeviceInfo(HDC &hDC, HFONT &hFont, LPCTSTR fontFaceName = TEXT("Arial"));
		void ReleaseDeviceInfo(HDC hDC, HFONT hFont);
		bool IsTrueTypeFont(HDC hdc);
	private:
		BYTE *m_pRawBuf;
		DWORD m_BufInBytes;
	};

	ParsingGlyphOutLine::ParsingGlyphOutLine(CHAR_T ch)
		: m_pRawBuf(NULL), m_BufInBytes(0)
	{
		Init(ch);
	}

	ParsingGlyphOutLine::~ParsingGlyphOutLine()
	{
		if ( m_pRawBuf != NULL )
			delete []m_pRawBuf;
	}

	void ParsingGlyphOutLine::GetPolygonPts(PolygonPtArray &polygonPts)
	{
		OGL_ASSERT(polygonPts.empty());

		LPTTPOLYGONHEADER pTTPHeader = reinterpret_cast<LPTTPOLYGONHEADER>(m_pRawBuf);
		const LPTTPOLYGONHEADER pTTPHeaderEnd = reinterpret_cast<LPTTPOLYGONHEADER>(m_pRawBuf + m_BufInBytes);

		while ( pTTPHeader != pTTPHeaderEnd )
		{
			LPTTPOLYCURVE pTTPCurve = reinterpret_cast<LPTTPOLYCURVE>(reinterpret_cast<BYTE*>(pTTPHeader) + sizeof(TTPOLYGONHEADER));
			const LPTTPOLYCURVE pTTPCurveEnd = reinterpret_cast<LPTTPOLYCURVE>(reinterpret_cast<BYTE*>(pTTPCurve) + pTTPHeader->cb - sizeof(TTPOLYGONHEADER));

			ContourPtArray contour;
			Point3D startPt(0); FixedToPoint(pTTPHeader->pfxStart, startPt);
			contour.push_back(startPt);

			while ( pTTPCurve != pTTPCurveEnd )
			{
				if ( pTTPCurve->wType == TT_PRIM_LINE )
				{
					for ( size_t ii = 0; ii < pTTPCurve->cpfx; ++ii )
					{
						Point3D pt(0); FixedToPoint(pTTPCurve->apfx[ii], pt);
						contour.push_back(pt);
					}
				}
				else
				{
					OGL_ASSERT(pTTPCurve->wType == TT_PRIM_QSPLINE);
					OGL_ASSERT(!contour.empty());
					Point3DArray pts(pTTPCurve->cpfx + 1);

					OGL_ASSERT(3 <= pts.size());
					pts[0] = contour.back();

					for ( size_t ii = 0; ii < pTTPCurve->cpfx; ++ii )
					{
						Point3D pt(0); FixedToPoint(pTTPCurve->apfx[ii], pt);
						pts[ii+1] = pt;
					}

					const size_t numOfSegment = pts.size() - 1 - 1;	// first 1 is the begin point, the second 1 is the offset

					contour.push_back(pts[0]);
					contour.push_back(pts[1]);

					for ( auto iter = std::begin(pts) + 2; iter != std::end(pts); ++iter )
					{
						const auto &ptA = *(iter - 2);
						const auto &ptB = *(iter - 1);
						const auto &ptC = *iter;

						const float dStep = 1.0f / 29.0f;
						for ( size_t ii =1; ii < 30; ++ii )
						{
							Point3D ptNew(0);

							const float tt = (dStep * ii);
							ptNew.x = (ptA.x - 2*ptB.x + ptC.x) * tt * tt + (2 * ptB.x - 2 * ptA.x) * tt + ptA.x;
							ptNew.y = (ptA.y - 2*ptB.y + ptC.y) * tt * tt + (2 * ptB.y - 2 * ptA.y) * tt + ptA.y;

							contour.push_back(ptNew);
						}
					}
				}

				const size_t curveSizeInByte = sizeof(TTPOLYCURVE) + (pTTPCurve->cpfx - 1) * sizeof(POINTFX);
				pTTPCurve = reinterpret_cast<LPTTPOLYCURVE>(reinterpret_cast<BYTE*>(pTTPCurve) + curveSizeInByte);

				OGL_ASSERT(pTTPCurve <= pTTPCurveEnd);
			}

			pTTPHeader = reinterpret_cast<LPTTPOLYGONHEADER>(reinterpret_cast<BYTE*>(pTTPHeader) + pTTPHeader->cb);
			polygonPts.push_back(contour);
		}
	}

	bool ParsingGlyphOutLine::Init(CHAR_T ch)
	{
		HDC hDC = NULL;
		HFONT hFont = NULL;

		if ( !GetDeviceInfo(hDC, hFont) )
			return false;

		if ( !InitGlyphBuffer(hDC, ch) )
		{
			ReleaseDeviceInfo(hDC, hFont);
			return false;
		}

		ReleaseDeviceInfo(hDC, hFont);
		return true;
	}

	bool ParsingGlyphOutLine::InitGlyphBuffer(HDC hDC, CHAR_T ch)
	{
		GLYPHMETRICS gm;
		MAT2 mat = { {0, 1}, {0, 0}, {0, 0}, {0, 1} };

		const DWORD dwSize = ::GetGlyphOutline(hDC, ch, GGO_NATIVE, &gm, 0, NULL, &mat);

		if ( GDI_ERROR == dwSize )
			return false;

		OGL_ASSERT(m_pRawBuf == NULL);
		m_pRawBuf = new BYTE[dwSize];

		if ( GDI_ERROR == ::GetGlyphOutline(hDC, ch, GGO_NATIVE, &gm, dwSize, m_pRawBuf, &mat) )
		{
			delete []m_pRawBuf;
			m_pRawBuf;
		}

		m_BufInBytes = dwSize;
		return true;
	}

	bool ParsingGlyphOutLine::GetDeviceInfo(HDC &hDC, HFONT &hFont, LPCTSTR fontFaceName /*= TEXT("Arial")*/)
	{
		HDC hDesktopDC = ::GetDC(NULL);

		hDC = ::CreateCompatibleDC(hDesktopDC);

		if ( IsTrueTypeFont(hDC) )
			return true;

		HFONT hOldFont = reinterpret_cast<HFONT>(::GetCurrentObject(hDC, OBJ_FONT));

		LOGFONT lf;
		if ( GDI_ERROR == ::GetObject(hOldFont, sizeof(LOGFONT), &lf) )
			return false;

		StringUtils::StrCpy(fontFaceName, lf.lfFaceName);

		hFont = ::CreateFontIndirect(&lf);

		::SelectObject(hDC, hFont);

		OGL_ASSERT(IsTrueTypeFont(hDC));
		::ReleaseDC(NULL, hDesktopDC);
		return true;
	}

	void ParsingGlyphOutLine::ReleaseDeviceInfo(HDC hDC, HFONT hFont)
	{
		::DeleteObject(hFont);
		::DeleteDC(hDC);
	}

	bool ParsingGlyphOutLine::IsTrueTypeFont(HDC hdc)
	{
		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);

		return (O_QUERY_BOOL(tm.tmPitchAndFamily, TMPF_TRUETYPE));
	}

	typedef std::vector<GLfloat *>	BufPtr;

	class O3DGLTessellator
	{
	public:
		O3DGLTessellator();
		~O3DGLTessellator();

		void PrepareTess(const PolygonPtArray &polygonPts, GLuint &dlID);
		void RegisterCallBackFunc();
	private:
		typedef void (_stdcall *TESS_CALLBACK_PTR)();

		static void CALLBACK TessBeginContour(GLenum type);
		static void CALLBACK TessEndContour();
		static void CALLBACK TessVertexes(void *vertex_data);
		static void CALLBACK TessVertexCombine(GLdouble coords[3], void *vertex_data[4], GLfloat weight[4], void **outData, void *user_data);
		static void CALLBACK TessError(GLenum errorCode);

		static void printPrimitivesType(GLenum type);
	public:
		class GLTessUserData
		{
		public:
			Point3DList	m_CombinePts;
		};

		GLTessUserData	m_UserData;

	private:
		GLUtesselator	*m_glTess;
	};

	O3DGLTessellator::O3DGLTessellator()
		: m_glTess(gluNewTess())
	{		
	}

	O3DGLTessellator::~O3DGLTessellator()
	{
		if ( m_glTess != NULL )
		{
			gluDeleteTess(m_glTess);
		}
	}

	void O3DGLTessellator::RegisterCallBackFunc()
	{
		OGL_ASSERT(NULL != m_glTess);

		gluTessCallback(m_glTess, GLU_TESS_BEGIN, (TESS_CALLBACK_PTR)TessBeginContour);
		gluTessCallback(m_glTess, GLU_TESS_END, (TESS_CALLBACK_PTR)TessEndContour);
		gluTessCallback(m_glTess, GLU_TESS_VERTEX, (TESS_CALLBACK_PTR)TessVertexes);
		gluTessCallback(m_glTess, GLU_TESS_COMBINE_DATA, (TESS_CALLBACK_PTR)TessVertexCombine);

		gluTessCallback(m_glTess, GLU_TESS_ERROR, (TESS_CALLBACK_PTR)TessError);
	}

	void O3DGLTessellator::PrepareTess(const PolygonPtArray &polygonPts, GLuint &dlID)
	{
		RegisterCallBackFunc();

		dlID = glGenLists(1);
		glNewList(dlID, GL_COMPILE);
		gluTessBeginPolygon(m_glTess, &m_UserData);

		for ( auto iter = std::begin(polygonPts); iter != std::end(polygonPts); ++iter )
		{
			gluTessBeginContour(m_glTess);
			const ContourPtArray &contour = *iter;

			for ( auto iterContour = std::begin(contour); iterContour != std::end(contour); ++iterContour )
			{
				GLdouble coords[] = { iterContour->x, iterContour->y, iterContour->z};
				gluTessVertex(m_glTess, coords, (void*)(const float*)(*iterContour));
			}

			gluTessEndContour(m_glTess);
		}
		gluTessEndPolygon(m_glTess);
		glEndList();
	}

	//static
	void CALLBACK O3DGLTessellator::TessBeginContour(GLenum type)
	{
		glBegin(type);

		printPrimitivesType(type);
	}

	//static
	void CALLBACK O3DGLTessellator::TessEndContour()
	{
		glEnd();

		cout << "End Primitives" << endl;
	}

	//static
	void CALLBACK O3DGLTessellator::TessVertexes(void *vertex_data)
	{
		glVertex3fv((GLfloat*)vertex_data);
	}

	//static
	void CALLBACK O3DGLTessellator::TessVertexCombine(GLdouble coords[3], void *vertex_data[4], GLfloat weight[4], void **outData, void *user_data)
	{
		O3DGLTessellator::GLTessUserData *pUserData = reinterpret_cast<O3DGLTessellator::GLTessUserData*>(user_data);

		pUserData->m_CombinePts.push_back(Point3D((GLfloat)coords[0]
		, (GLfloat)coords[1]
		, (GLfloat)coords[2]));

		*outData = (float*)pUserData->m_CombinePts.back();	// call float*() cast function

	}

	//static 
	void CALLBACK O3DGLTessellator::TessError(GLenum errorCode)
	{
		const GLubyte *errorStr = gluErrorString(errorCode);
		cout << "[ERROR]: " << errorStr << endl;
	}

	// static
	void O3DGLTessellator::printPrimitivesType(GLenum type)
	{
		LPCTSTR PTypeNames[] = 
		{
			TEXT("GL_POINTS"),
			TEXT("GL_LINES"),
			TEXT("GL_LINE_LOOP"),
			TEXT("GL_LINE_STRIP"),
			TEXT("GL_TRIANGLES"),
			TEXT("GL_TRIANGLE_STRIP"),
			TEXT("GL_TRIANGLE_FAN"),
			TEXT("GL_QUADS"),
			TEXT("GL_QUAD_STRIP"),
			TEXT("GL_POLYGON")
		};

		OGL_ASSERT(size_t(std::end(PTypeNames) - std::begin(PTypeNames)) > (size_t)type);

		cout << "Begin Primitives Type : " << PTypeNames[type] << endl;
	}

	OutlineFontDrawObj::OutlineFontDrawObj()
		: m_FontListBase(0)
	{

	}

	OutlineFontDrawObj::~OutlineFontDrawObj()
	{
		glDeleteLists(m_FontListBase, 256);
	}

	//static 
	string OutlineFontDrawObj::GetObjName()
	{
		return STR_OUT_LINE_FONT_OBJ_NAME;
	}

	//virtual 
	string OutlineFontDrawObj::GetName() const
	{
		return GetObjName();
	}


	void OutlineFontDrawObj::Print(const string &strText)
	{
		//float		length = 0.0f;
		//for (size_t loop = 0; loop < strText.length(); loop++ )	// Loop To Find Text Length
		//{
		//	length += m_gmf[strText[loop]].gmfCellIncX;			// Increase Length By Each Characters Width
		//}

		//glTranslatef(-length/2, 0.0f, 0.0f);					// Center Our Text On The Screen

		glPushAttrib(GL_LIST_BIT);											// Pushes The Display List Bits
		glListBase(m_FontListBase);											// Sets The Base Character to 0
		glCallLists((GLsizei)strText.length(), GL_UNSIGNED_SHORT, strText.c_str());	// Draws The Display List Text
		glPopAttrib();														// Pops The Display List Bits
	}

	void OutlineFontDrawObj::BuildFont()									// Build Our Bitmap Font
	{
		HDC hDC = ::GetDC(NULL);  

		m_FontListBase = glGenLists(256);							// Storage For 256 Characters

		HFONT font = ::CreateFont( -12,								// Height Of Font
			0,								// Width Of Font
			0,								// Angle Of Escapement
			0,								// Orientation Angle
			FW_NORMAL,						// Font Weight
			FALSE,							// Italic
			FALSE,							// Underline
			FALSE,							// Strikeout
			ANSI_CHARSET,					// Character Set Identifier
			OUT_TT_PRECIS,					// Output Precision
			CLIP_DEFAULT_PRECIS,			// Clipping Precision
			ANTIALIASED_QUALITY,			// Output Quality
			FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
			TEXT("courier new"));						// Font Name

		SelectObject(hDC, font);							// Selects The Font We Created

		wglUseFontOutlines(	hDC,							// Select The Current DC
			0,								// Starting Character
			255,							// Number Of Display Lists To Build
			m_FontListBase,							// Starting Display Lists
			0.0f,							// Deviation From The True Outlines
			0.0f,							// Font Thickness In The Z Direction
			//WGL_FONT_POLYGONS,				// Use Polygons, Not Lines
			WGL_FONT_LINES,
			m_gmf);							// Address Of Buffer To Recieve Data

		::DeleteObject(font);	

	}

	//virtual 
	bool OutlineFontDrawObj::PrepareObj()
	{
		BuildFont();
		return true;
	}

	//virtual 
	bool OutlineFontDrawObj::Draw()
	{
		glRotatef(45.f, 0.0, 1.0, 0.0);
		Print(TEXT("Kit1111"));
		return true;
	}


	OutlineFontDrawObj::BitmapFontTest::BitmapFontTest()
	{
		m_BitmapFontListBase = glGenLists(96);

		HDC hDC = ::GetDC(NULL);

		HFONT hFont = ::CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH, TEXT("courier new"));

		BOOL bRet = wglUseFontBitmaps(hDC, 32, 96, m_BitmapFontListBase);


		ReleaseDC(NULL, hDC);

	}

	OutlineFontDrawObj::BitmapFontTest::~BitmapFontTest()
	{
		glDeleteLists(m_BitmapFontListBase, 96);
	}
}