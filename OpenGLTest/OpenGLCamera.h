#ifndef _OPENGLCAMERA_H_
#define _OPENGLCAMERA_H_

namespace OGLKit
{
	class Camera
	{
	public:
		Camera(const Unit3f &pos, const Unit3f &dirt);

		Unit3f&		Position();
		Unit3f&		Direction();
		Unit3f&		Up();


		void		PutCamera(const Unit3f &pos, const Unit3f &dirt);
		void		PutCamera();

	private:
		Unit3f	m_pos;
		Unit3f	m_direction;
		Unit3f	m_up;
	};
}

#endif	//_OPENGLCAMERA_H_