#include "Cameras.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp> // Projection Matrices

#include "Instrumentation/Profile.h"

PerspectiveCamera::PerspectiveCamera(const Position &pos, const Orientation &ori, float fov, float aspectRation)
		: m_Position(pos), m_Orientation(ori), m_FOV(fov), m_AspectRatio(aspectRation)
{
}

void PerspectiveCamera::Bind(Shader& shader) const
{
	NIC_PROFILE_FUNCTION();

	auto viewMat = glm::translate(glm::rotate(glm::rotate(glm::mat4(1.0f),
																												-m_Orientation.Pitch, {1.0f, 0.0f, 0.0f}),
																						-m_Orientation.Yaw, {0.0f, 1.0f, 0.0f}),
																{-m_Position.X, -m_Position.Y, -m_Position.Z});

	auto projMat = glm::perspective(m_FOV, m_AspectRatio, 0.01f, 150.0f);

	shader.SetUniformMat4("u_ViewProjection", projMat * viewMat);
}

void PerspectiveCamera::SetPosition(float x, float y, float z)
{
	NIC_PROFILE_FUNCTION();

	m_Position.X = x;
	m_Position.Y = y;
	m_Position.Z = z;
}
void PerspectiveCamera::SetOrientation(float pitch, float yaw)
{
	NIC_PROFILE_FUNCTION();

	m_Orientation.Pitch = pitch;
	m_Orientation.Yaw = yaw;
}

void PerspectiveCamera::TransformPosition(float dX, float dY, float dZ)
{
	NIC_PROFILE_FUNCTION();

	m_Position.X += dX * std::cos(m_Orientation.Yaw) + dZ * std::sin(m_Orientation.Yaw);
	m_Position.Y += dY;
	m_Position.Z += -dX * std::sin(m_Orientation.Yaw) + dZ * std::cos(m_Orientation.Yaw);
}
void PerspectiveCamera::TransformOrientation(float dPitch, float dYaw)
{
	NIC_PROFILE_FUNCTION();

	m_Orientation.Pitch += dPitch;
	m_Orientation.Yaw += dYaw;
}

void PerspectiveCamera::SetFOV(float fov)
{
	NIC_PROFILE_FUNCTION();

	m_FOV = fov;
}
void PerspectiveCamera::SetAspectRatio(float aspectRatio)
{
	NIC_PROFILE_FUNCTION();

	m_AspectRatio = aspectRatio;
}