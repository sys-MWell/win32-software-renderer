#pragma once
#include "framework.h"
#include "Vector3D.h"

class AmbientLighting
{
public:
	AmbientLighting();
	AmbientLighting(int redIntensity, int greenIntensity, int blueIntensity);
	int getRedIntensity() const;
	void setRedIntensity(const int redIntensity);
	int getGreenIntensity() const;
	void setGreenIntensity(const int greenIntensity);
	int getBlueIntensity() const;
	void setBlueIntensity(const int blueIntensity);

private:
	int _redIntensity;
	int _greenIntensity;
	int _blueIntensity;
	Vector3D _lightDirection;
};

