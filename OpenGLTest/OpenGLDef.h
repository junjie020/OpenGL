#ifndef _OPENGLDEF_H_
#define _OPENGLDEF_H_

namespace OGLKit
{
	#define STR_TEX_DRAWING_OBJ_NAME		TEXT("TexObj")
	#define STR_FTGL_DRAWING_OBJ_NAME		TEXT("FTGLObj")
	#define STR_PBO_DRAWING_OBJ_NAME		TEXT("PBOObj")
	#define STR_COMMON_DRAWING_OBJ_NAME		TEXT("CommonObj")
	#define STR_OUT_LINE_FONT_OBJ_NAME		TEXT("OutlineFont")
	#define STR_FBO_NAME					TEXT("FBO")
	#define STR_POLYGON_STIPPLE_NAME		TEXT("PolygonStipple")

	#define STR_DEFAULT_SCENE_MGR			TEXT("DefaultSceneMgr")

	#ifdef	_UNICODE
		typedef std::wstring	string;
	#else	//!_UNICODE
		typedef std::string		string;
	#endif	//!_UNICODE

		typedef std::string		stringA;
		typedef std::wstring	stringW;

	#define PI						3.14159265359
	#define DEGREE_TO_RADIAN(_de)	(((PI)/180.0) * (_de))
	#define	RADIAN_TO_DEGREE(_ra)	((180.0/(PI)) * (_ra))
}

#include "OpenGLInternalDef.h"


#endif	//_OPENGLDEF_H_