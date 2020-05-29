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
	void Colide(const glm::vec3& colisionNormal, const glm::vec3& contactPoint, const float colisionDepth, const float wallRestitutionFactor);

	void Thrust(const glm::vec3& impulse, const glm::vec3& applicationPoint);
	void Thrust(const glm::vec3& impulse);

	float GetRestitutionFactor() const noexcept { return m_RestitutionFactor; }
	void SetRestitutionFactor(float restitutionFactor) noexcept { m_RestitutionFactor = restitutionFactor; }

	float GetMass() const noexcept { return m_Mass; }
	void SetMass(float mass) noexcept { m_Mass = mass; m_2verticesMass = m_Mass / 12; }

	float GetRotationalInertia(const glm::vec3& rotationAxisDirectionNormalized) const noexcept;

	const glm::vec3& GetPosition() const noexcept { return m_Position; }
	std::array<glm::vec3, 8> GetVertices() const;

	const glm::vec3& GetLinearVelocity() const noexcept { return m_LinearVelocity; }
	void TransformLinearVelocity(const glm::vec3& deltaVelocity) { m_LinearVelocity += deltaVelocity; }

	const glm::vec3& GetAngularVelocity() const noexcept { return m_AngularVelocity; }
	void TransformAngularVelocity(const glm::vec3& deltaVelocity) { m_AngularVelocity += deltaVelocity; ; }

	glm::mat4 GetTransfomation() const noexcept;

private:
	void ColideWithTorque(const glm::vec3& colisionNormal, const float colisionDepth, const glm::vec3& r, const glm::vec3& effectiveRadius, const float colisionRestitutionFactor);
	void ColideWithoutTorque(const glm::vec3& colisionNormal, const float colisionDepth, const float colisionRestitutionFactor);

	void Thrust(const glm::vec3& impulse, const glm::vec3& applicationPoint, const glm::mat3& rotation, const float rotationInertia);

private:
	float m_Mass;
	float m_2verticesMass;
	float m_RestitutionFactor;

	const float m_Rx;
	const float m_Ry;
	const float m_Rz;

	glm::vec3 m_Position;
	glm::quat m_Orientation;

	glm::vec3 m_LinearVelocity;
	glm::vec3 m_AngularVelocity;
};