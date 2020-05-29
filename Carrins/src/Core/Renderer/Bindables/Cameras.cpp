#include "Cameras.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp> // Projection Matrices
#include <glm/gtx/euler_angles.hpp>			// eulerAngleYXZ

#include "Utility/MathConstants.hpp"
#include "Utility/Manip.hpp"

#include "Instrumentation/Profile.h"

PerspectiveCamera::PerspectiveCamera(const Position &pos, const Orientation &ori, float fov, float aspectRation)
		: m_Position(pos), m_Orientation(ori), m_FOV(fov), m_AspectRatio(aspectRation)
{
}

void PerspectiveCamera::Bind(Shader &shader) const
{
	NIC_PROFILE_FUNCTION();

	auto viewMat = glm::translate(glm::inverse(glm::eulerAngleYXZ(m_Orientation.Yaw, m_Orientation.Pitch, m_Orientation.Roll)),
																{-m_Position.X, -m_Position.Y, -m_Position.Z});

	auto projMat = glm::perspective(m_FOV, m_AspectRatio, 0.01f, 4500.0f);

	shader.SetUniformMat4("u_ViewProjection", projMat * viewMat);
}

void PerspectiveCamera::SetPosition(float x, float y, float z)
{
	m_Position.X = x;
	m_Position.Y = y;
	m_Position.Z = z;
}
void PerspectiveCamera::TransformPosition(float dX, float dY, float dZ)
{
	m_Position.X += dX * std::cos(m_Orientation.Yaw) + dZ * std::sin(m_Orientation.Yaw);
	m_Position.Y += dY;
	m_Position.Z += -dX * std::sin(m_Orientation.Yaw) + dZ * std::cos(m_Orientation.Yaw);
}

const PerspectiveCamera::Position &PerspectiveCamera::GetPosition() const
{
	return m_Position;
}
const PerspectiveCamera::Orientation &PerspectiveCamera::GetOrientation() const
{
	return m_Orientation;
}

void PerspectiveCamera::SetOrientation(float pitch, float yaw, float roll)
{
	m_Orientation.Pitch = nic::Clamp(pitch, float(nic::PI) / 2, -float(nic::PI) / 2);
	m_Orientation.Yaw = nic::Wrap(yaw, float(nic::PI), -float(nic::PI));
	m_Orientation.Roll = nic::Wrap(roll, 2 * float(nic::PI), -2 * float(nic::PI));
}
void PerspectiveCamera::TransformOrientation(float dPitch, float dYaw, float dRoll)
{
	m_Orientation.Pitch = nic::Clamp(m_Orientation.Pitch + dPitch, float(nic::PI) / 2, -float(nic::PI) / 2);
	m_Orientation.Yaw = nic::Wrap(m_Orientation.Yaw + dYaw, float(nic::PI), -float(nic::PI));
	m_Orientation.Roll = nic::Wrap(m_Orientation.Roll + dRoll, 2 * float(nic::PI), -2 * float(nic::PI));
}

void PerspectiveCamera::SetFOV(float fov)
{
	m_FOV = fov;
}
void PerspectiveCamera::SetAspectRatio(float aspectRatio)
{
	m_AspectRatio = aspectRatio;
}