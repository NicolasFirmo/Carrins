#pragma once
#include "glm/glm.hpp"

class Solid
{
public:
	Solid() = default;
	Solid(float mass,float rotationalInertia, float restitutionFactor, const glm::vec3& position, const glm::vec3& initialLinearVelocity, const glm::vec3& initialAngularVelocity);

	void Update(const float deltaTime);

	void Colide(Solid& other, const glm::vec3& colisionNormal, const glm::vec3& contactPoint);
	void Colide(const glm::vec3& colisionNormal, const glm::vec3& contactPoint);

	void Thrust(const glm::vec3& impulse, const glm::vec3& applicationPoint);
	void Drag(const glm::vec3& deltaVelocity);

	const glm::vec3& GetPosition() const { return m_Position; }
	const glm::vec3& GetLinearVelocity() const { return m_LinearVelocity; }

private:
	const float m_Mass;
	float m_RotationalInertia;
	const float m_RestitutionFactor;

	glm::vec3 m_Position;
	glm::vec3 m_LinearVelocity;
	glm::vec3 m_AngularVelocity;
};