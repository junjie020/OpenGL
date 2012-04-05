#ifndef _OPENGLMOUSECTRL_H_
#define _OPENGLMOUSECTRL_H_

namespace OGLKit
{
	class MouseCtrl
	{
	public:
		struct MouseInfo
		{
			UINT	mouseButton;
			//Point	pt;
		};

		virtual bool OnMouseUp(const MouseInfo &info) = 0;
		virtual bool OnMouseDown(const MouseInfo &info) = 0;
		virtual bool OnMouseMove(const MouseInfo &info) = 0;
	};
}

#endif	//_OPENGLMOUSECTRL_H_