#ifndef _OPENGLRENDERWIN_H_
#define _OPENGLRENDERWIN_H_
#include "OpenGLRenderTarget.h"
#include "OpenGLKeyboardCtrl.h"
#include "OpenGLMouseCtrl.h"

namespace OGLKit
{
	class RenderWindow : public RenderTarget
	{
	public:
		RenderWindow(const string &strTitle, size_t width, size_t height);
		virtual ~RenderWindow();

		virtual bool Init();
		virtual void Destory();

		virtual bool ResizeWin(size_t width, size_t height);


		virtual void BeoforeRender();
		virtual bool Render();
		virtual void AfterRender();

		void SwapBuffers();

	public:
		bool		IsClose() const { return m_bClose; }
		bool		IsActive() const { return m_bActive; }
		void		SetActive(bool bActive){ m_bActive = bActive; }

	public:
		bool		InitGLWindowPixelFormat(size_t colorDepth = 16);
	public:
		bool		NotifyKeyboardCtrlDown(KeyboardCtrl::KeyInfo &info);
		bool		NotifyKeyboardCtrlUp(KeyboardCtrl::KeyInfo &info);


		bool		NotifyMouseCtrlDown(MouseCtrl::MouseInfo &info);
		bool		NotifyMouseCtrlUp(MouseCtrl::MouseInfo &info);
		bool		NotifyMouseCtrlMove(MouseCtrl::MouseInfo &info);

		bool		AddKeyboardListener(KeyboardCtrl *pKeyCtrl);
		bool		AddMouseListener(MouseCtrl *pMouseCtrl);
	private:
		HDC			m_hDC;
		HGLRC		m_hRC;
		HWND		m_hWnd;
		HINSTANCE	m_hInstance;

		bool		m_bClose;
		bool		m_bActive;

		typedef std::set<KeyboardCtrl*>	KeyboardCtrlSet;
		KeyboardCtrlSet		m_keyCtrlListener;

		typedef std::set<MouseCtrl*>	MouseCtrlSet;
		MouseCtrlSet		m_mouseCtrlListener;

	};
}
#endif	//_OPENGLRENDERWIN_H_	