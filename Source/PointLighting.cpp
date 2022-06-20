#include "PointLighting.h"
#include "Vertex.h"

PointLighting::PointLighting()
{
	_redIntensity = 0;
	_greenIntensity = 0;
	_blueIntensity = 0;
	_aCoefficient = 0;
	_bCoefficient = 0;
	_cCoefficient = 0;
	_lightPosition = Vertex(0,0,0,1);
}

PointLighting::PointLighting(int redIntensity, int greenIntensity, int blueIntensity, int aCoefficient, int bCoefficient, int cCoefficient, Vertex lightPosition)
{
	_redIntensity = redIntensity;
	_greenIntensity = greenIntensity;
	_blueIntensity = blueIntensity;
	_aCoefficient = aCoefficient;
	_bCoefficient = bCoefficient;
	_cCoefficient = cCoefficient;
	_lightPosition = lightPosition;
}

int PointLighting::getRedIntensity() const
{
	return _redIntensity;
}

void PointLighting::setRedIntensity(const int redIntensity)
{
	_redIntensity = redIntensity;
}

int PointLighting::getGreenIntensity() const
{
	return _greenIntensity;
}

void PointLighting::setGreenIntensity(const int greenIntensity)
{
	_greenIntensity = greenIntensity;
}

int PointLighting::getBlueIntensity() const
{
	return _blueIntensity;
}

void PointLighting::setBlueIntensity(const int blueIntensity)
{
	_blueIntensity = blueIntensity;
}

int PointLighting::getACoefficient() const
{
	return _aCoefficient;
}

void PointLighting::setACoefficient(const int aCoefficient)
{
	_aCoefficient = aCoefficient;
}

int PointLighting::getBCoefficient() const
{
	return _bCoefficient;
}

void PointLighting::setBCoefficient(const int bCoefficient)
{
	_bCoefficient = bCoefficient;
}

int PointLighting::getCCoefficient() const
{
	return _cCoefficient;
}

void PointLighting::setCCoefficient(const int cCoefficient)
{
	_cCoefficient = cCoefficient;
}

Vertex PointLighting::getLightDirection()
{
	return _lightPosition;
}

void PointLighting::setLightDirection(Vertex lightPosition)
{
	_lightPosition = lightPosition;
}
