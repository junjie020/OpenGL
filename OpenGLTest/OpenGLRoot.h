#ifndef _OPENGLROOT_H_
#define _OPENGLROOT_H_

namespace OGLKit
{
	class RenderTarget;
	class SceneManager;
	class RenderRoot
	{
	public:
		RenderRoot();
		~RenderRoot();
		// render function
	public:
		bool	RenderOneFrame();

		// generate function
	public:
		SceneManager* CreateSceneMgr(const string &strSceneMgrName = STR_DEFAULT_SCENE_MGR);
		SceneManager* GetSceneMgr(const string &strSceneMgrName = STR_DEFAULT_SCENE_MGR);

		RenderTarget* CreateRenderWin(const string &strRenderWinName, const string &strWinTitle, size_t width, size_t height, size_t clrBit = 32);
		RenderTarget* CreateRenderTex(const string &strRenderTexName, size_t width, size_t height);	// now it's empty

		RenderTarget* FindRenderTarget(const string &strRenderWinName);

		// static public function
	public:
		static RenderRoot&	GetSingleton();
		// static private data
	private:
		static RenderRoot*	ms_pRoot;
	private:
		typedef std::map<string, SceneManager*>	SceneManagerMap;
		typedef std::map<string, RenderTarget*>	RenderTargetMap;

		SceneManagerMap	m_SceneMgrMap;
		RenderTargetMap	m_SceneTargetMap;


	};
}
#endif	//_OPENGLROOT_H_