#include "stdafx.h"

#include "OpenGLFrame.h"
#include "OpenGLRenderWin.h"
#include "OpenGLSceneMgr.h"
#include "OpenGLRenderWin.h"
#include "OpenGLRenderableObj.h"
#include "OpenGLRoot.h"

namespace OGLKit
{
	OpenGLFramework* OpenGLFramework::ms_pOpenGLFrame = NULL;

	OpenGLFramework::OpenGLFramework()
		: m_pSceneMgr(0), m_pRenderWin(0)
	{
		if ( ms_pOpenGLFrame == NULL )
			ms_pOpenGLFrame = this;

		m_pRoot = new RenderRoot;
	}

	//virtual 
	OpenGLFramework::~OpenGLFramework()
	{
	}

	bool OpenGLFramework::InitFramework()
	{
		return nullptr != CreateSceneMgr();
	}

	bool OpenGLFramework::RenderOneFrame()
	{
		return m_pRoot->RenderOneFrame();
	}

	SceneManager* OpenGLFramework::CreateSceneMgr()
	{
		return (m_pSceneMgr = m_pRoot->CreateSceneMgr());
	}

	RenderWindow* OpenGLFramework::CreateRenderWin(const string &strTitle, size_t width, size_t height)
	{
		return (m_pRenderWin = static_cast<RenderWindow*>(m_pRoot->CreateRenderWin(TEXT("Default"), strTitle, width, height)));
	}

	RenderWindow* OpenGLFramework::GetRenderWin()
	{
		return m_pRenderWin;
	}

	bool OpenGLFramework::BuildSceneObj()
	{
		m_pSceneMgr->CreateMainCamera(Unit3f(0.0, 0.0, 10.0), Unit3f(0.0, 0.0, 0.0));

		//CreateRenderableObj(STR_FTGL_DRAWING_OBJ_NAME);
		//CreateRenderableObj(STR_TEX_DRAWING_OBJ_NAME);

		//CreateRenderableObj(STR_PBO_DRAWING_OBJ_NAME);
		
		//CreateRenderableObj(STR_COMMON_DRAWING_OBJ_NAME);
		//CreateRenderableObj(STR_FBO_NAME);

		//CreateRenderableObj(STR_OUT_LINE_FONT_OBJ_NAME);

		CreateRenderableObj(STR_POLYGON_STIPPLE_NAME);


		return true;
	}

	RenderableObj*	OpenGLFramework::CreateRenderableObj(const string &objName)
	{
		RenderableObj *pRenderableObj = m_pSceneMgr->CreateRenderableObj(objName);
		if ( nullptr != pRenderableObj )
		{
			pRenderableObj->BindCamera(m_pSceneMgr->GetMainCamera());
			pRenderableObj->PrepareObj();
			m_pSceneMgr->AttachObj(pRenderableObj);
		}

		return pRenderableObj;
	}

	void OpenGLFramework::MessageLoop()
	{
		while ( !GetCurrentWin()->IsClose() )
		{
			MSG  msg;
			if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
	}

	//static 
	LRESULT	OpenGLFramework::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_CREATE)
		{	// Store pointer to Win32Window in user data area
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams));
			return 0;
		}

		RenderWindow* pRenderWin = reinterpret_cast<RenderWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (!pRenderWin)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		switch (uMsg)									// Check For Windows Messages
		{
		case WM_ACTIVATE:								// Watch For Window Activate Message
			{
				return 0;;								// Return To The Message Loop
			}

		case WM_PAINT:
			{
				if ( pRenderWin->IsActive() )
				{
					pRenderWin->BeoforeRender();
					if ( pRenderWin->Render() )
						pRenderWin->AfterRender();
				}

				break;
			}

		case WM_SYSCOMMAND:
			{
				switch (wParam)						
				{
				case SC_SCREENSAVE:					
				case SC_MONITORPOWER:				
					break;						
				}
				break;									// Exit
			}

		case WM_CLOSE:
			{
				pRenderWin->Destory();
				PostQuitMessage(0);
				break;							
			}

		case WM_KEYDOWN:							
			{
				if ( VK_ESCAPE == wParam )
					PostMessage(hWnd, WM_CLOSE, 0, 0);

				KeyboardCtrl::KeyInfo info = {0};
				info.keyDown = static_cast<UINT>(wParam);
				info.dwAuxiliaryKeys = 0;

				if ( pRenderWin->NotifyKeyboardCtrlDown(info) )
				{
					SendMessage(hWnd, WM_PAINT, 0, 0);	
				}
				else if ( VK_F5 == wParam )
				{
					SendMessage(hWnd, WM_PAINT, 0, 0);
				}
				break;
			}

		case WM_KEYUP:
			{
				//keys[wParam] = FALSE;
				KeyboardCtrl::KeyInfo info = {0};
				info.keyDown = static_cast<UINT>(wParam);
				info.dwAuxiliaryKeys = 0;

				pRenderWin->NotifyKeyboardCtrlUp(info);
				break;
			}

		case WM_SIZE:								
			{
				pRenderWin->ResizeWin(static_cast<size_t>(LOWORD(lParam)), static_cast<size_t>(HIWORD(lParam)));
				return 0;
			}
		}
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
}