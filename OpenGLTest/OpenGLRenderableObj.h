#ifndef _OPENGLRENDERABLEOBJ_H_
#define _OPENGLRENDERABLEOBJ_H_

namespace OGLKit
{
	class SceneManager;
	class Camera;
	class RenderableObj
	{
	public:
		RenderableObj() 
			: m_pSceneMgr(nullptr)
			, m_pCamera(nullptr)
		{}
		virtual ~RenderableObj(){}

		virtual string GetName() const = 0;

		virtual bool PrepareObj() = 0;

		virtual bool Draw() = 0;

		void	SetSceneMgr(SceneManager *pSceneMgr){ m_pSceneMgr; }
		SceneManager* GetSceneMgr()const { return m_pSceneMgr; }

		Camera*	GetCamera() const {return m_pCamera; }
		void	BindCamera(Camera *pCam){ m_pCamera = pCam; }

	protected:
		SceneManager *m_pSceneMgr;
		Camera		 *m_pCamera;

	};
}
#endif	//_OPENGLRENDERABLEOBJ_H_