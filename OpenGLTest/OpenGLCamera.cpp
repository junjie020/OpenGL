#include "stdafx.h"

#include "OpenGLCamera.h"

namespace OGLKit
{
	Camera::Camera(const Unit3f &pos, const Unit3f &dirt)
	{
		m_up = Unit3f(0.0f, 1.0f, 0.0f);
		m_pos = pos;
		m_direction = dirt;
		PutCamera();
	}

	Unit3f& Camera::Position()
	{
		return m_pos;
	}

	Unit3f& Camera::Direction()
	{
		return m_direction;
	}

	Unit3f& Camera::Up()
	{
		return m_up;
	}

	void Camera::PutCamera(const Unit3f &pos, const Unit3f &dirt)
	{
		Position() = pos, Direction() = dirt;
		PutCamera();
	}

	void Camera::PutCamera()
	{
		glLoadIdentity();
		gluLookAt(m_pos.x, m_pos.y, m_pos.z
			, m_direction.x, m_direction.y, m_direction.z
			, m_up.x, m_up.y, m_up.z);
	}
}