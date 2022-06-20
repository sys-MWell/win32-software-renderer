#include "AmbientLighting.h"

AmbientLighting::AmbientLighting()
{
	_redIntensity = 0;
	_greenIntensity = 0;
	_blueIntensity = 0;
}

AmbientLighting::AmbientLighting(int redIntensity, int greenIntensity, int blueIntensity)
{
	_redIntensity = redIntensity;
	_greenIntensity = greenIntensity;
	_blueIntensity = blueIntensity;
}

int AmbientLighting::getRedIntensity() const
{
	return _redIntensity;
}

void AmbientLighting::setRedIntensity(const int redIntensity)
{
	_redIntensity = redIntensity;
}

int AmbientLighting::getGreenIntensity() const
{
	return _greenIntensity;
}

void AmbientLighting::setGreenIntensity(const int greenIntensity)
{
	_greenIntensity = greenIntensity;
}

int AmbientLighting::getBlueIntensity() const
{
	return _blueIntensity;
}

void AmbientLighting::setBlueIntensity(const int blueIntensity)
{
	_blueIntensity = blueIntensity;
}