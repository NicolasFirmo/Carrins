#include "Solid.h"

Solid::Solid(float mass, float rotationalInertia, float restitutionFactor, const glm::vec3& position, const glm::vec3& initialLinearVelocity, const glm::vec3& initialAngularVelocity) :
	m_Mass(mass), m_RotationalInertia(rotationalInertia), m_RestitutionFactor(restitutionFactor),
	m_Position(position), m_LinearVelocity(initialLinearVelocity), m_AngularVelocity(initialAngularVelocity)
{
}

void Solid::Update(const float deltaTime)
{
	m_Position += m_LinearVelocity * deltaTime;
}

void Solid::Colide(Solid& other, const glm::vec3& colisionNormal, const glm::vec3& contactPoint)
{
	//const float colisionRestitutionFactor = m_RestitutionFactor;

	//m_Position += colisionNormal * (depth * (1 + colisionRestitutionFactor));

	const glm::vec3 r1 = contactPoint - m_Position;
	const glm::vec3 r2 = contactPoint - other.m_Position;

	const glm::vec3 effectiveRadius1 = glm::cross(r1, colisionNormal);
	const glm::vec3 effectiveRadius2 = glm::cross(r2, colisionNormal);

	const float effectiveMass1 = m_Mass / (1.0f + glm::dot(effectiveRadius1, effectiveRadius1) / m_RotationalInertia);
	const float effectiveMass2 = other.m_Mass / (1.0f + glm::dot(effectiveRadius2, effectiveRadius2) / other.m_RotationalInertia);

	const float ApproximationVelocity = glm::dot(colisionNormal, ((other.m_LinearVelocity + glm::cross(other.m_AngularVelocity, r2)) - (m_LinearVelocity + glm::cross(m_AngularVelocity, r1))));

	const float inelasticImpulseMagnitude = ((effectiveMass1 * effectiveMass2) / (effectiveMass1 + effectiveMass2)) * (ApproximationVelocity);

	const float colisionRestitutionFactor = sqrt(m_RestitutionFactor * other.m_RestitutionFactor);

	const glm::vec3 colisionImpulse = colisionNormal * (inelasticImpulseMagnitude * (1 + colisionRestitutionFactor));

	Thrust(colisionImpulse, r1);
	other.Thrust(-colisionImpulse, r2);
}

void Solid::Colide(const glm::vec3& colisionNormal, const glm::vec3& contactPoint)
{
	const float colisionRestitutionFactor = m_RestitutionFactor;

	const glm::vec3 r = contactPoint - m_Position;

	const glm::vec3 effectiveRadius = glm::cross(r, colisionNormal);

	const float effectiveMass = m_Mass / (1.0f + glm::dot(effectiveRadius, effectiveRadius) / m_RotationalInertia);

	const float ApproximationVelocity = glm::dot(colisionNormal, -(m_LinearVelocity + glm::cross(m_AngularVelocity, r)));

	const float inelasticImpulseMagnitude = effectiveMass * ApproximationVelocity;

	const glm::vec3 colisionImpulse = colisionNormal * (inelasticImpulseMagnitude * (1 + colisionRestitutionFactor));

	Thrust(colisionImpulse, r);
}

void Solid::Thrust(const glm::vec3& impulse, const glm::vec3& applicationPoint)
{
	m_LinearVelocity += impulse / m_Mass;
	m_AngularVelocity += glm::cross(applicationPoint, impulse) / (m_Mass * m_RotationalInertia);
}

void Solid::Drag(const glm::vec3& deltaVelocity)
{
	m_LinearVelocity += deltaVelocity;
}