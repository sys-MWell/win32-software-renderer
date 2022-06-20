#pragma once
#include "framework.h"
#include "Vector3D.h"

class DirectionalLighting
{
public:
	DirectionalLighting();
	DirectionalLighting(int redIntensity, int greenIntensity, int blueIntensity, const Vector3D& lightDirection);
	int getRedIntensity() const;
	void setRedIntensity(const int redIntensity);
	int getGreenIntensity() const;
	void setGreenIntensity(const int greenIntensity);
	int getBlueIntensity() const;
	void setBlueIntensity(const int blueIntensity);
	Vector3D getLightDirection() const;
	void setLightDirection(const Vector3D lightDirection);

private:
	int _redIntensity;
	int _greenIntensity;
	int _blueIntensity;
	Vector3D _lightDirection;
};

