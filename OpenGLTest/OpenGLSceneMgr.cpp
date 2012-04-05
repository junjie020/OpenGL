#include "stdafx.h"

#include "OpenGLSceneMgr.h"
#include "OpenGLRenderableObj.h"
#include "OpenGLCamera.h"

namespace OGLKit
{
	SceneManager::SceneManager(const string &strName)
		: m_strName(strName)
	{

	}

	//virtual 
	SceneManager::~SceneManager()
	{
		std::for_each(m_RenderObjList.begin(), m_RenderObjList.end(), [](RenderObjList::value_type &val){ delete val; });
	}

	void SceneManager::AttachObj(RenderableObj *renderObj)
	{
		renderObj->SetSceneMgr(this);
		m_RenderObjList.push_back(renderObj);
	}

	bool SceneManager::RenderScene()
	{
		const auto iterEnd = m_RenderObjList.end();
		for ( auto iter = m_RenderObjList.begin(); iter != iterEnd; ++iter )
		{
			(*iter)->GetCamera()->PutCamera();
			(*iter)->Draw();
		}

		return true;
	}

#include "OpenGLCamera.h"
	Camera*	SceneManager::CreateMainCamera(const Unit3f &pos, const Unit3f &dirt)
	{
		m_pMainCam = new Camera(pos, dirt);
		return m_pMainCam;
	}
}

#include "OpenGLPBOObj.h"
#include "OpenGLTextureDrawing.h"
#include "OpenGLFTGLFont.h"
#include "OpenGLCommonDrawObj.h"
#include "OpenGLOutlineFont.h"

#include "OpenGLFrameBufferObj.h"
#include "OpenGLPolygonStipple.h"


namespace OGLKit
{
	RenderableObj*	SceneManager::CreateRenderableObj(const string &strObjName)
	{
		if ( strObjName == PboDrawingObj::GetObjName() )
			return new PboDrawingObj;

		if ( strObjName == TextureDrawingObj::GetObjName() )
			return new TextureDrawingObj;

		if ( strObjName == FTGLFontDrawingObj::GetObjName() )
			return new FTGLFontDrawingObj;

		if ( strObjName == CommonDrawObj::GetObjName() )
			return new CommonDrawObj;

		if ( strObjName == OutlineFontDrawObj::GetObjName() )
			return new OutlineFontDrawObj;

		if ( strObjName == FrameBufferDrawObj::GetObjName() )
			return new FrameBufferDrawObj;

		if ( strObjName == PolygonStippleDrawObj::GetObjName() )
			return new PolygonStippleDrawObj;

		return NULL;
	}
}