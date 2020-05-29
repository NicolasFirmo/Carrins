#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class Solid
{
public:
	Solid() = default;
	Solid(float mass, float restitutionFactor, float rX, float rY, float rZ, const glm::vec3& initialPosition, const glm::quat& initialOrientation, const glm::vec3& initialLinearVelocity, const glm::vec3& initialAngularVelocity);

	void Update(const float deltaTime);

	void Colide(Solid& other, const glm::vec3& colisionNormal, const glm::vec3& contactPoint);
	void Colide(const glm::vec3& colisionNormal, const glm::vec3& contactPoint, const float colisionDepth);

	void Thrust(const glm::vec3& impulse, const glm::vec3& applicationPoint);

	float GetRestitutionFactor() const noexcept { return m_RestitutionFactor; }
	void SetRestitutionFactor(float restitutionFactor) noexcept { m_RestitutionFactor = restitutionFactor; }

	float GetRotationalInertia(const glm::vec3& rotationAxisDirection) const noexcept;

	glm::mat4 GetTransfomation() const noexcept;

	const glm::vec3& GetPosition() const noexcept { return m_Position; }
	std::array<glm::vec3, 8> GetVertices() const;

	const glm::vec3& GetLinearVelocity() const noexcept { return m_LinearVelocity; }
	void TransformLinearVelocity(const glm::vec3& deltaVelocity);

	const glm::vec3& GetAngularVelocity() const noexcept { return m_AngularVelocity; }
	void TransformAngularVelocity(const glm::vec3& deltaVelocity);

private:
	float m_Mass;
	float m_RestitutionFactor;

	const float m_Rx;
	const float m_Ry;
	const float m_Rz;

	glm::vec3 m_Position;
	glm::quat m_Orientation;

	glm::vec3 m_LinearVelocity;
	glm::vec3 m_AngularVelocity;
};