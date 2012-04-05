#ifndef _OPENGLRENDERTARGET_H_
#define _OPENGLRENDERTARGET_H_

namespace OGLKit
{
	class RenderTarget
	{
	public:
		virtual ~RenderTarget(){}

		virtual bool Init() = 0;
		virtual void Destory() = 0;

		virtual void BeoforeRender() = 0;
		virtual bool Render() = 0;
		virtual void AfterRender() = 0;

	};
}
#endif	//_OPENGLRENDERTARGET_H_