#ifndef _OPENGLSCENEMGR_H_
#define _OPENGLSCENEMGR_H_

namespace OGLKit
{
	class RenderableObj;
	class Camera;
	class SceneManager
	{
	public:
		SceneManager(const string &strName);
		virtual ~SceneManager();

		void			AttachObj(RenderableObj *renderObj);
		bool			RenderScene();

		RenderableObj*	CreateRenderableObj(const string &strObjName);

		Camera*			CreateMainCamera(const Unit3f &pos, const Unit3f &dirt);

		inline Camera*	GetMainCamera(){ return m_pMainCam; }
	private:
		typedef std::vector<RenderableObj*>	RenderObjList;

		RenderObjList	m_RenderObjList;

		Camera			*m_pMainCam;

		string			m_strName;
	};
}

#endif	//_OPENGLSCENEMGR_H_