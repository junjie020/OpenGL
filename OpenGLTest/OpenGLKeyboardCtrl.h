#ifndef _OPENGLKEYBOARDCTRL_H_
#define _OPENGLKEYBOARDCTRL_H_

namespace OGLKit
{
	class KeyboardCtrl
	{
	public:
		struct KeyInfo
		{
			UINT	keyDown;
			DWORD	dwAuxiliaryKeys;
		};

		virtual bool	OnKeyUp(const KeyInfo &info) = 0;
		virtual bool	OnKeyDown(const KeyInfo &info) = 0;
	};
}
#endif	//_OPENGLKEYBOARDCTRL_H_