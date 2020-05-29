#include "Solid.h"

#include "Core.h"

Solid::Solid(float mass, float restitutionFactor, float rX, float rY, float rZ, const glm::vec3& initialPosition, const glm::quat& initialOrientation, const glm::vec3& initialLinearVelocity, const glm::vec3& initialAngularVelocity) :
	m_Mass(mass), m_2verticesMass(m_Mass / 12), m_RestitutionFactor(restitutionFactor), m_Rx(rX), m_Ry(rY), m_Rz(rZ),
	m_Position(initialPosition), m_Orientation(initialOrientation), m_LinearVelocity(initialLinearVelocity), m_AngularVelocity(initialAngularVelocity)
{
}

void Solid::Update(const float deltaTime)
{
	m_Position += m_LinearVelocity * deltaTime;

	if (auto angle = glm::length(m_AngularVelocity); angle > 0.0f)
	{
		auto dir = m_AngularVelocity / angle;
		auto dAngle = angle * deltaTime;

		glm::quat rotation = {
			std::cos(dAngle / 2),
			dir.x * std::sin(dAngle / 2),
			dir.y * std::sin(dAngle / 2),
			dir.z * std::sin(dAngle / 2)
		};

		m_Orientation *= rotation;
	}
}

void Solid::Colide(Solid& other, const glm::vec3& colisionNormal, const glm::vec3& contactPoint)
{
	//const float colisionRestitutionFactor = m_RestitutionFactor;

	//m_Position += colisionNormal * (depth * (1 + colisionRestitutionFactor));

	//const glm::vec3 r1 = contactPoint - m_Position;
	//const glm::vec3 r2 = contactPoint - other.m_Position;

	//const glm::vec3 effectiveRadius1 = glm::cross(r1, colisionNormal);
	//const glm::vec3 effectiveRadius2 = glm::cross(r2, colisionNormal);

	//const float effectiveMass1 = m_Mass / (1.0f + glm::dot(effectiveRadius1, effectiveRadius1) / m_RotationalInertia);
	//const float effectiveMass2 = other.m_Mass / (1.0f + glm::dot(effectiveRadius2, effectiveRadius2) / other.m_RotationalInertia);

	//const float ApproximationVelocity = glm::dot(colisionNormal, ((other.m_LinearVelocity + glm::cross(other.m_AngularVelocity, r2)) - (m_LinearVelocity + glm::cross(m_AngularVelocity, r1))));

	//const float inelasticImpulseMagnitude = ((effectiveMass1 * effectiveMass2) / (effectiveMass1 + effectiveMass2)) * (ApproximationVelocity);

	//const float colisionRestitutionFactor = sqrt(m_RestitutionFactor * other.m_RestitutionFactor);

	//const glm::vec3 colisionImpulse = colisionNormal * (inelasticImpulseMagnitude * (1 + colisionRestitutionFactor));

	//Thrust(colisionImpulse, r1);
	//other.Thrust(-colisionImpulse, r2);
}

void Solid::Colide(const glm::vec3& colisionNormal, const glm::vec3& contactPoint, const float colisionDepth, const float wallRestitutionFactor)
{
	const glm::vec3 r = contactPoint - m_Position;

	const glm::vec3 effectiveRadius = glm::cross(r, colisionNormal);

	const float colisionRestitutionFactor = sqrt(m_RestitutionFactor * wallRestitutionFactor);

	if (glm::length2(effectiveRadius))
		ColideWithTorque(colisionNormal, colisionDepth, r, effectiveRadius, colisionRestitutionFactor);
	else
		ColideWithoutTorque(colisionNormal, colisionDepth, colisionRestitutionFactor);
}

void Solid::ColideWithTorque(const glm::vec3& colisionNormal, const float colisionDepth, const glm::vec3& r, const glm::vec3& effectiveRadius, const float colisionRestitutionFactor)
{
	const auto aparentRotationInertia = GetRotationalInertia(glm::normalize(effectiveRadius));

	const float effectiveMass = m_Mass / (1.0f + (glm::dot(effectiveRadius, effectiveRadius) / aparentRotationInertia));

	const auto rotation = glm::toMat3(m_Orientation);
	const auto worldAngularvelocity = rotation * m_AngularVelocity;
	const float ApproximationVelocity = glm::dot(colisionNormal, -(m_LinearVelocity + glm::cross(worldAngularvelocity, r)));

	const float inelasticImpulseMagnitude = effectiveMass * ApproximationVelocity;

	const auto rebound = colisionNormal * (1 + colisionRestitutionFactor);
	const glm::vec3 colisionImpulse = inelasticImpulseMagnitude * rebound;

	Thrust(colisionImpulse, r, rotation, aparentRotationInertia);

	m_Position += colisionDepth * rebound * (1.0f - glm::length(effectiveRadius) / glm::length(r));
}

void Solid::ColideWithoutTorque(const glm::vec3& colisionNormal, const float colisionDepth, const float colisionRestitutionFactor)
{
	const float ApproximationVelocity = glm::dot(colisionNormal, -m_LinearVelocity);

	const auto rebound = colisionNormal * (1 + colisionRestitutionFactor);
	const glm::vec3 colisionDV = ApproximationVelocity * rebound;

	TransformLinearVelocity(colisionDV);

	m_Position += colisionDepth * rebound;
}

void Solid::Thrust(const glm::vec3& impulse)
{
	TransformLinearVelocity(impulse / m_Mass);
}

void Solid::Thrust(const glm::vec3& impulse, const glm::vec3& applicationPoint)
{
	TransformLinearVelocity(impulse / m_Mass);

	const auto rotation = glm::toMat3(m_Orientation);
	const auto leverage = glm::cross(applicationPoint, impulse);

	if (const auto RotationInertia = GetRotationalInertia(glm::normalize(leverage)); RotationInertia > 0.0f)
		TransformAngularVelocity((glm::inverse(rotation) * leverage) / (m_Mass * RotationInertia));
}

void Solid::Thrust(const glm::vec3& impulse, const glm::vec3& applicationPoint, const glm::mat3& rotation, const float rotationInertia)
{
	TransformLinearVelocity(impulse / m_Mass);
	TransformAngularVelocity((glm::inverse(rotation) * glm::cross(applicationPoint, impulse)) / (m_Mass * rotationInertia));
}

float Solid::GetRotationalInertia(const glm::vec3& rotationAxisDirectionNormalized) const noexcept
{
	const auto r1 = glm::cross(rotationAxisDirectionNormalized, glm::vec3{ m_Rx,m_Ry,m_Rz });
	const auto r2 = glm::cross(rotationAxisDirectionNormalized, glm::vec3{ m_Rx,m_Ry,-m_Rz });
	const auto r3 = glm::cross(rotationAxisDirectionNormalized, glm::vec3{ m_Rx,-m_Ry,m_Rz });
	const auto r4 = glm::cross(rotationAxisDirectionNormalized, glm::vec3{ m_Rx,-m_Ry,-m_Rz });
	//const auto r5 = glm::cross(rotationAxisDirectionNormalized, glm::vec3{ -m_Rx,m_Ry,m_Rz });
	//const auto r6 = glm::cross(rotationAxisDirectionNormalized, glm::vec3{ -m_Rx,m_Ry,-m_Rz });
	//const auto r7 = glm::cross(rotationAxisDirectionNormalized, glm::vec3{ -m_Rx,-m_Ry,m_Rz });
	//const auto r8 = glm::cross(rotationAxisDirectionNormalized, glm::vec3{ -m_Rx,-m_Ry,-m_Rz });

	const auto R1pow2 = glm::dot(r1, r1);
	const auto R2pow2 = glm::dot(r2, r2);
	const auto R3pow2 = glm::dot(r3, r3);
	const auto R4pow2 = glm::dot(r4, r4);
	//const auto R5pow2 = glm::dot(r5, r5);
	//const auto R6pow2 = glm::dot(r6, r6);
	//const auto R7pow2 = glm::dot(r7, r7);
	//const auto R8pow2 = glm::dot(r8, r8);

	return m_2verticesMass * (
		R1pow2 +
		R2pow2 +
		R3pow2 +
		R4pow2 //+
		//R5pow2 +
		//R6pow2 +
		//R7pow2 +
		//R8pow2
		);
}

std::array<glm::vec3, 8> Solid::GetVertices() const
{
	const auto rotation = glm::toMat3(m_Orientation);

	auto r1 = (rotation * glm::vec3{ m_Rx,m_Ry,m_Rz }) + m_Position;
	auto r2 = (rotation * glm::vec3{ m_Rx,m_Ry,-m_Rz }) + m_Position;
	auto r3 = (rotation * glm::vec3{ m_Rx,-m_Ry,m_Rz }) + m_Position;
	auto r4 = (rotation * glm::vec3{ m_Rx,-m_Ry,-m_Rz }) + m_Position;
	auto r5 = (rotation * glm::vec3{ -m_Rx,m_Ry,m_Rz }) + m_Position;
	auto r6 = (rotation * glm::vec3{ -m_Rx,m_Ry,-m_Rz }) + m_Position;
	auto r7 = (rotation * glm::vec3{ -m_Rx,-m_Ry,m_Rz }) + m_Position;
	auto r8 = (rotation * glm::vec3{ -m_Rx,-m_Ry,-m_Rz }) + m_Position;

	return {
	r1,
	r2,
	r3,
	r4,
	r5,
	r6,
	r7,
	r8,
	};
}

glm::mat4 Solid::GetTransfomation() const noexcept
{
	return glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(m_Orientation);
}
