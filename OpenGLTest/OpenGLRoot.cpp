#include "stdafx.h"

#include "OpenGLRoot.h"
#include "OpenGLSceneMgr.h"
#include "OpenGLRenderWin.h"

namespace OGLKit
{
	//static 
	RenderRoot*	RenderRoot::ms_pRoot = nullptr;

	RenderRoot::RenderRoot()
	{
		if ( nullptr != ms_pRoot )
		{
			assert("can't not be empty");
		}

		ms_pRoot = this;
	}

	RenderRoot::~RenderRoot()
	{
		std::for_each(std::begin(m_SceneMgrMap), std::end(m_SceneMgrMap), [](SceneManagerMap::reference refVal){ delete refVal.second;});
		std::for_each(std::begin(m_SceneTargetMap), std::end(m_SceneTargetMap), [](RenderTargetMap::reference refVal){ delete refVal.second;});
	}

	bool	RenderRoot::RenderOneFrame()
	{
		std::for_each(std::begin(m_SceneMgrMap), std::end(m_SceneMgrMap), 
			[](SceneManagerMap::reference refSceneMgr)
		{
			refSceneMgr.second->RenderScene();
		}
		);

		return true;
	}



	SceneManager* RenderRoot::CreateSceneMgr(const string &strSceneMgrName /*= STR_DEFAULT_SCENE_MGR*/)
	{
		auto iterSceneMgr = m_SceneMgrMap.find(strSceneMgrName);
		if ( iterSceneMgr != std::end(m_SceneMgrMap) )
		{
			return iterSceneMgr->second;
		}

		auto sceneMgrPtr = new SceneManager(strSceneMgrName);
		m_SceneMgrMap[strSceneMgrName] = sceneMgrPtr;
		return sceneMgrPtr;
	}

	SceneManager* RenderRoot::GetSceneMgr(const string &strSceneMgrName /*= STR_DEFAULT_SCENE_MGR*/)
	{
		auto iterFound = m_SceneMgrMap.find(strSceneMgrName);

		if ( std::end(m_SceneMgrMap) != iterFound )
			return iterFound->second;

		return nullptr;
	}

	RenderTarget* RenderRoot::CreateRenderWin(const string &strRenderWinName, const string &strWinTitle, size_t width, size_t height, size_t clrBit /*= 32*/)
	{
		auto iterRenderWin = m_SceneTargetMap.find(strRenderWinName);

		if ( iterRenderWin != std::end(m_SceneTargetMap) )
		{
			return iterRenderWin->second;
		}

		auto renderWinPtr = new RenderWindow(strWinTitle, width, height);
		m_SceneTargetMap[strRenderWinName] = renderWinPtr;
		return renderWinPtr;
	}

	RenderTarget* RenderRoot::FindRenderTarget(const string &strRenderWinName)
	{
		auto targetMapPair = m_SceneTargetMap.find(strRenderWinName);

		if ( std::end(m_SceneTargetMap) == targetMapPair )
			return nullptr;

		return targetMapPair->second;
	}

	//static 
	RenderRoot&	RenderRoot::GetSingleton() { return *ms_pRoot; }
}