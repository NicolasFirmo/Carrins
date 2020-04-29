#pragma once
#include "Shader.h"

#include "Utility/MathConstants.hpp"

class Camera
{
public:
	virtual void Bind(Shader& shader) const = 0;
};

class PerspectiveCamera : public Camera
{
public:
	struct Position
	{
		float X{}, Y{}, Z{};
	};
	struct Orientation
	{
		float Pitch{}, Yaw{};
	};

public:
	PerspectiveCamera(const Position &pos, const Orientation &ori, float fov = nic::PI / 4, float aspectRation = 16.0f / 9.0f);
	void Bind(Shader& shader) const override;

	void SetPosition(float x, float y, float z);
	void SetOrientation(float pitch, float yaw);

	void TransformPosition(float dX, float dY, float dZ);
	void TransformOrientation(float dPitch, float dYaw);

	void SetFOV(float fov);
	void SetAspectRatio(float aspectRatio);

private:
	Position m_Position{};
	Orientation m_Orientation{};

	float m_FOV = nic::PI / 4;
	float m_AspectRatio = 16.0f / 9.0f;
};