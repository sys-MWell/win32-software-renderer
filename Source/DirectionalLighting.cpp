#include "DirectionalLighting.h"

DirectionalLighting::DirectionalLighting()
{
	_redIntensity = 50;
	_greenIntensity = 50;
	_blueIntensity = 50;
	_lightDirection = { (int)50.0 ,(int)50.0, (int)50.0};
}

DirectionalLighting::DirectionalLighting(int redIntensity, int greenIntensity, int blueIntensity, const Vector3D& lightDirection)
{
	_redIntensity = redIntensity;
	_greenIntensity = greenIntensity;
	_blueIntensity = blueIntensity;
	_lightDirection = lightDirection;
}

int DirectionalLighting::getRedIntensity() const
{
	return _redIntensity;
}

void DirectionalLighting::setRedIntensity(const int redIntensity)
{
	_redIntensity = redIntensity;
}

int DirectionalLighting::getGreenIntensity() const
{
	return _greenIntensity;
}

void DirectionalLighting::setGreenIntensity(const int greenIntensity)
{
	_greenIntensity = greenIntensity;
}

int DirectionalLighting::getBlueIntensity() const
{
	return _blueIntensity;
}

void DirectionalLighting::setBlueIntensity(const int blueIntensity)
{
	_blueIntensity = blueIntensity;
}

Vector3D DirectionalLighting::getLightDirection() const
{
	return _lightDirection;
}

void DirectionalLighting::setLightDirection(const Vector3D lightDirection)
{
	_lightDirection = lightDirection;
}