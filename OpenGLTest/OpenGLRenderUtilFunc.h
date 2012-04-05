#ifndef _OPENGLRENDERUTILFUNC_H_
#define _OPENGLRENDERUTILFUNC_H_

namespace OGLKit
{
	class RenderUtilFuncs
	{
		static void DrawTriangle(bool bColor = false,
							const Point3D &pt0 = Point3D(0, 0, 0),
							const Point3D &pt1 = Point3D(0, 1, 0), 
							const Point3D &pt2 = Point3D(1, 1, 0));

		static void DrawCube( bool bColor = false );
	};

	class StringUtils
	{
	public:
		template<class StrType>
		static size_t StrLen(const StrType *str)
		{
			return _StrLen(str);
		}

		template<class StrType>
		static StrType* StrCpy(const StrType *strSrc, StrType *strDst)
		{
			return _StrCpy(strSrc, strDst);
		}

	private:
		inline static size_t _StrLen(const char *str)
		{
			return std::strlen(str);
		}

		inline static size_t _StrLen(const wchar_t *wstr)
		{
			return std::wcslen(wstr);
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		inline static char* _StrCpy(const char *strSrc, char *strDst)
		{
			return std::strcpy(strDst, strSrc);
		}

		inline static wchar_t* _StrCpy(const wchar_t *wstrSrc, wchar_t *wstrDst)
		{
			return std::wcscpy(wstrDst, wstrSrc);
		}
#pragma warning(pop)
	};
}

#endif	//_OPENGLRENDERUTILFUNC_H_