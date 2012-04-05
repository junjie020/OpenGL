#include "stdafx.h"

#include "OpenGLRenderWin.h"
#include "OpenGLFrame.h"
#include "OpenGLKeyboardCtrl.h"
#include "OpenGLMouseCtrl.h"
#include "OpenGLSceneMgr.h"
#include "OpenGLCamera.h"
#include "OpenGLRoot.h"

namespace OGLKit
{
#ifdef _UNICODE
	static inline const wchar_t* GET_WIN32_CLASS_NAME(){ return L"OpenGL"; }
#else	//_UNICODE
	static inline const char* GET_WIN32_CLASS_NAME(){ return "OpenGL"; }
#endif	//_UNICODE


	RenderWindow::RenderWindow(const string &strTitle, size_t width, size_t height)
		: m_bClose(false)
	{
		WNDCLASS	wc;						// Windows Class Structure
		DWORD		dwExStyle;				// Window Extended Style
		DWORD		dwStyle;				// Window Style
		RECT		winRect;				// Grabs Rectangle Upper Left / Lower Right Values
		winRect.left = (long)0;			// Set Left Value To 0
		winRect.right = (long)width;		// Set Right Value To Requested Width
		winRect.top = (long)0;				// Set Top Value To 0
		winRect.bottom = (long)height;		// Set Bottom Value To Requested Height

		m_hInstance			= ::GetModuleHandle(NULL);				// Grab An Instance For Our Window
		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
		wc.lpfnWndProc		= (WNDPROC) OpenGLFramework::WndProc;					// WndProc Handles Messages
		wc.cbClsExtra		= 0;									// No Extra Window Data
		wc.cbWndExtra		= 0;									// No Extra Window Data
		wc.hInstance		= m_hInstance;							// Set The Instance
		wc.hIcon			= ::LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
		wc.hCursor			= ::LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
		wc.hbrBackground	= NULL;									// No Background Required For GL
		wc.lpszMenuName		= NULL;									// We Don't Want A Menu
		wc.lpszClassName	= GET_WIN32_CLASS_NAME();				// Set The Class Name

		if (!RegisterClass(&wc))									// Attempt To Register The Window Class
		{
			cout << "Error : " << "Failed To Register The Window Class." << endl;
			return ;
		}

		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style


		::AdjustWindowRectEx(&winRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

		// Create The Window
		if (!(m_hWnd = ::CreateWindowEx( dwExStyle,							// Extended Style For The Window
			GET_WIN32_CLASS_NAME(),				// Class Name
			strTitle.c_str(),					// Window Title
			dwStyle |							// Defined Window Style
			WS_CLIPSIBLINGS |					// Required Window Style
			WS_CLIPCHILDREN,					// Required Window Style
			0, 0,								// Window Position
			winRect.right - winRect.left,		// Calculate Window Width
			winRect.bottom - winRect.top,		// Calculate Window Height
			NULL,								// No Parent Window
			NULL,								// No Menu
			m_hInstance,						// Instance
			this)))								// Pass this when WM_CREATE
		{
			Destory();													// Reset The Display
			cout << "Error : " << "Window Creation Error." << endl;
			return ;								
		}

		InitGLWindowPixelFormat();

		if (  GLEW_ERROR_NO_GL_VERSION == glewInit() )
			cout << "if (  GLEW_ERROR_NO_GL_VERSION == ret )" << endl;

		::ShowWindow(m_hWnd, SW_SHOW);						// Show The Window
		::SetForegroundWindow(m_hWnd);						// Slightly Higher Priority
		::SetFocus(m_hWnd);									// Sets Keyboard Focus To The Window
	}

	// virtual
	RenderWindow::~RenderWindow()
	{
	}

	bool RenderWindow::InitGLWindowPixelFormat(size_t colorDepth /*= 16*/)
	{
		GLuint		pixelFormat;			// Holds The Results After Searching For A Match

		PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			(BYTE)colorDepth,									// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};

		if (!(m_hDC = ::GetDC(m_hWnd)))							// Did We Get A Device Context?	
		{
			Destory();
			cout << "ERROR : " << "Can't Create A GL Device Context." << endl;
			return false;								// Return FALSE
		}

		if (!(pixelFormat = ::ChoosePixelFormat(m_hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			Destory();
			cout << "ERROR : " << "Can't Find A Suitable PixelFormat." << endl;
			return false;								// Return FALSE
		}

		if ( !::SetPixelFormat(m_hDC, pixelFormat, &pfd) )		// Are We Able To Set The Pixel Format?
		{
			Destory();
			return false;
		}

		if (!(m_hRC = ::wglCreateContext(m_hDC)) )				// Are We Able To Get A Rendering Context?
		{
			Destory();
			cout << "ERROR : " << "Can't Create A GL Rendering Context." << endl;
			return false;								// Return FALSE
		}

		if ( !::wglMakeCurrent(m_hDC,m_hRC) )					// Try To Activate The Rendering Context
		{
			Destory();
			cout << "ERROR : " << "Can't Activate The GL Rendering Context." << endl;
			return false;								// Return FALSE
		}
		return true;
	}

	bool RenderWindow::NotifyKeyboardCtrlDown(KeyboardCtrl::KeyInfo &info)
	{
		bool bNeedRedraw = false;

		std::for_each(begin(m_keyCtrlListener), end(m_keyCtrlListener)
			, [&](KeyboardCtrlSet::value_type pKB)
		{
			bNeedRedraw = pKB->OnKeyDown(info);
		}
		);

		return bNeedRedraw;

	}

	bool RenderWindow::NotifyKeyboardCtrlUp(KeyboardCtrl::KeyInfo &info)
	{
		std::for_each(std::begin(m_keyCtrlListener), std::end(m_keyCtrlListener)
			, [&info](KeyboardCtrlSet::value_type pKB)
		{
			pKB->OnKeyUp(info);
		}
		);

		return true;
	}

	bool RenderWindow::NotifyMouseCtrlDown(MouseCtrl::MouseInfo &info)
	{
		std::for_each(std::begin(m_mouseCtrlListener), std::end(m_mouseCtrlListener)
			, [&info](MouseCtrlSet::value_type pMouse)
		{
			pMouse->OnMouseDown(info);
		}
		);

		return true;
	}

	bool RenderWindow::NotifyMouseCtrlUp(MouseCtrl::MouseInfo &info)
	{
		std::for_each(std::begin(m_mouseCtrlListener), std::end(m_mouseCtrlListener)
			, [&info](MouseCtrlSet::value_type pMouse)
		{
			pMouse->OnMouseUp(info);
		}
		);

		return true;
	}

	bool RenderWindow::NotifyMouseCtrlMove(MouseCtrl::MouseInfo &info)
	{
		std::for_each(std::begin(m_mouseCtrlListener), std::end(m_mouseCtrlListener)
			, [&info](MouseCtrlSet::value_type pMouse)
		{
			pMouse->OnMouseMove(info);
		}
		);

		return true;
	}

	bool RenderWindow::AddKeyboardListener(KeyboardCtrl *pKeyCtrl)
	{
		if ( std::end(m_keyCtrlListener) != m_keyCtrlListener.find(pKeyCtrl) )
			return false;

		m_keyCtrlListener.insert(pKeyCtrl);
		return true;
	}

	bool RenderWindow::AddMouseListener(MouseCtrl *pMouseCtrl)
	{
		if ( m_mouseCtrlListener.end() != m_mouseCtrlListener.find(pMouseCtrl) )
			return false;

		m_mouseCtrlListener.insert(pMouseCtrl);
		return true;
	}

	//virtual 
	bool RenderWindow::Init()
	{
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background

		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

		return true;
	}

	//virtual 
	bool RenderWindow::ResizeWin(size_t width, size_t height)
	{
		if (height == 0)
		{
			height = 1;										
		}

		glViewport(0, 0, (GLsizei)width, (GLsizei)height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 1000.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		return true;
	}
	// virtual
	void RenderWindow::Destory()
	{
		if (m_hRC)											// Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			{
				MessageBox(NULL,TEXT("Release Of DC And RC Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
			}

			if (!wglDeleteContext(m_hRC))						// Are We Able To Delete The RC?
			{
				MessageBox(NULL,TEXT("Release Rendering Context Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
			}
			m_hRC = NULL;										// Set RC To NULL
		}

		if (m_hDC && !ReleaseDC(m_hWnd, m_hDC))					// Are We Able To Release The DC
		{
			MessageBox(NULL,TEXT("Release Device Context Failed."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
			m_hDC = NULL;										// Set DC To NULL
		}

		if (m_hWnd && !DestroyWindow(m_hWnd) )					// Are We Able To Destroy The Window?
		{
			MessageBox(NULL,TEXT("Could Not Release hWnd."), TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
			m_hWnd = NULL;											// Set hWnd To NULL
		}

		if (!UnregisterClass(GET_WIN32_CLASS_NAME(),m_hInstance))			// Are We Able To Unregister Class
		{
			MessageBox(NULL,TEXT("Could Not Unregister Class."),TEXT("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
			m_hInstance = NULL;									// Set hInstance To NULL
		}

		m_bClose = true;
	}

	//virtual 
	void RenderWindow::BeoforeRender()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glLoadIdentity();


		Camera *pCamera = RenderRoot::GetSingleton().GetSceneMgr()->GetMainCamera();
		//pCamera->Direction() = Unit3f(0.0, 0.0, 0.0);
		//pCamera->Position() = Unit3f(0.0, 0.0, 10.0);
	}

	//virtual 
	bool RenderWindow::Render()
	{
		return OpenGLFramework::GetSingleton().RenderOneFrame();
	}

	// virtual
	void RenderWindow::AfterRender()
	{
		SwapBuffers();
	}

	void RenderWindow::SwapBuffers()
	{
		::SwapBuffers(m_hDC);
	}
}