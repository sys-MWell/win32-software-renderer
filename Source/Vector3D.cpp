#include "Vector3D.h"

Vector3D::Vector3D()
{
	_vectorX = 0.0f;
	_vectorY = 0.0f;
	_vectorZ = 0.0f;
}

Vector3D::Vector3D(float vectorX, float vectorY, float vectorZ)
{
	_vectorX = vectorX;
	_vectorY = vectorY;
	_vectorZ = vectorZ;
}

Vector3D::Vector3D(const Vector3D& other)
{
	_vectorX = other.GetVectorX();
	_vectorY = other.GetVectorY();
	_vectorZ = other.GetVectorZ();
}

float Vector3D::GetVectorX() const
{
	return _vectorX;
}

void Vector3D::SetVectorX(const float vectorX)
{
	_vectorX = vectorX;
}

float Vector3D::GetVectorY() const
{
	return _vectorY;
}

void Vector3D::SetVectorY(const float vectorY)
{
	_vectorY = vectorY;
}

float Vector3D::GetVectorZ() const
{
	return _vectorZ;
}

void Vector3D::SetVectorZ(const float vectorZ)
{
	_vectorZ = vectorZ;
}
Vector3D& Vector3D::operator=(const Vector3D& rhs)
{
	// Only do the assignment if we are not assigning
	// to ourselves
	if (this != &rhs)
	{
		_vectorX = rhs._vectorX;
		_vectorY = rhs._vectorY;
		_vectorZ = rhs._vectorZ;
		
	}
	return *this;
}
const Vector3D Vector3D::operator+(const Vector3D& other) const
{
	return Vector3D(_vectorX + other.GetVectorX(), _vectorY + other.GetVectorY(), _vectorZ + other.GetVectorZ());
}

float Vector3D::DotProduct(Vector3D vectorU, Vector3D vectorV)
{
	return ((vectorU.GetVectorX() * vectorV.GetVectorX()) + (vectorU.GetVectorY() * vectorV.GetVectorY()) + (vectorU.GetVectorZ() * vectorV.GetVectorZ()));
}

Vector3D Vector3D::VectorCrossProduct(Vector3D vectorU, Vector3D vectorV)
{
	Vector3D crossProduct = Vector3D(
							(vectorU.GetVectorY() * vectorV.GetVectorZ()) - (vectorU.GetVectorZ() * vectorV.GetVectorY()),
						  (vectorU.GetVectorZ() * vectorV.GetVectorX()) - (vectorU.GetVectorX() * vectorV.GetVectorZ()),
						  (vectorU.GetVectorX() * vectorV.GetVectorY()) - (vectorU.GetVectorY() * vectorV.GetVectorX())
	);
	return crossProduct;
}

void Vector3D::normalise()
{
	float xSqr = _vectorX * _vectorX;
	float ySqr = _vectorY * _vectorY;
	float zSqr = _vectorZ * _vectorZ;
	float length = sqrtf(xSqr + ySqr + zSqr);
	_vectorX = _vectorX / length;
	_vectorY = _vectorY / length;
	_vectorZ = _vectorZ / length;
}



