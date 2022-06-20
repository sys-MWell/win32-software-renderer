#pragma once

#include "Vertex.h"

class PointLighting
{
public:
	PointLighting();
	PointLighting(int redIntensity, int greenIntensity, int blueIntensity, int aCoefficient, int bCoefficient, int cCoefficient, Vertex lightPosition);

	int getRedIntensity() const;
	void setRedIntensity(const int redIntensity);
	int getGreenIntensity() const;
	void setGreenIntensity(const int greenIntensity);
	int getBlueIntensity() const;
	void setBlueIntensity(const int blueIntensity);

	int getACoefficient() const;
	void setACoefficient(const int aCoefficient);
	int getBCoefficient() const;
	void setBCoefficient(const int bCoefficient);
	int getCCoefficient() const;
	void setCCoefficient(const int cCoefficient);

	Vertex getLightDirection();
	void setLightDirection(Vertex lightPosition);

private:
	int _redIntensity;
	int _greenIntensity;
	int _blueIntensity;
	int _aCoefficient;
	int _bCoefficient;
	int _cCoefficient;
	Vertex _lightPosition;
};

