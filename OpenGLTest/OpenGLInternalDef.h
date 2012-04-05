#ifndef _OPENGLINTERNALDEF_H_
#define _OPENGLINTERNALDEF_H_
namespace OGLKit
{
	inline bool O_QUERY_BOOL(DWORD dwBits, DWORD dwVal) { return !!(dwBits & dwVal); }
	inline void O_ADD_BIT(DWORD &dwBits, DWORD dwVal) { dwBits |= dwVal; }
	inline void O_REMOVE_BIT(DWORD &dwBits, DWORD dwVal) { dwBits &= ~dwVal; }
	inline void O_SET_BIT(DWORD &dwBits, DWORD dwVal, bool bSet) { bSet ? O_ADD_BIT(dwBits, dwVal) : O_REMOVE_BIT(dwBits, dwVal); }

	#ifdef	_DEBUG
	#define OGL_ASSERT(expr) \
		do { if (!(expr) && \
		(1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL))) \
		_CrtDbgBreak(); } while (0)
	#else //!_DEBUG
		#define OGL_ASSERT(expr) (void*)0
	#endif	//_DEBUG

#ifdef	_UNICODE
	typedef wchar_t CHAR_T;
#else	//!_UNICODE
	typedef char	CHAR_T;
#endif	//_UNICODE
	
}


#endif	//_OPENGLINTERNALDEF_H_
