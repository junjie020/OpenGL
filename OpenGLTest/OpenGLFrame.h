

#ifndef _OPENGLFRAME_H_
#define _OPENGLFRAME_H_

namespace OGLKit
{
	class SceneManager;
	class RenderWindow;
	class RenderRoot;
	class RenderableObj;

	class OpenGLFramework
	{
	public:
		OpenGLFramework();
		virtual ~OpenGLFramework();

		bool			InitFramework();
		bool			RenderOneFrame();

		SceneManager*	CreateSceneMgr();
		RenderWindow*	CreateRenderWin(const string &strTitle, size_t width, size_t height);

		RenderWindow*	GetRenderWin();

		bool			BuildSceneObj();
		void			MessageLoop();

		inline RenderWindow*	GetCurrentWin() const{ return m_pRenderWin; }

	private:
		RenderableObj*	CreateRenderableObj(const string &objName);

	public:
		static OpenGLFramework& GetSingleton(){ return *ms_pOpenGLFrame; }
		static OpenGLFramework* GetSingletonPtr(){ return ms_pOpenGLFrame; }

		static LRESULT	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		static OpenGLFramework		*ms_pOpenGLFrame;

	private:
		RenderRoot		*m_pRoot;

		SceneManager    *m_pSceneMgr;
		RenderWindow    *m_pRenderWin;


	};

}

#endif	//_OPENGLFRAME_H_