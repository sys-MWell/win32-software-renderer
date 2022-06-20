#include "Polygon3D.h"

Polygon3D::Polygon3D()
{
	_indices[0] = 0;
	_indices[1] = 0;
	_indices[2] = 0;
	_polygonCulled = false;
	_polygonZ = 0.0f;
	_normalVector = { 0,0,0 };
}

Polygon3D::Polygon3D(int index0, int index1, int index2)
{
	_indices[0] = index0;
	_indices[1] = index1;
	_indices[2] = index2;
	_polygonCulled = false;
	_polygonZ = 0.0f;
	_normalVector = { 0,0,0 };
}

Polygon3D::~Polygon3D()
{
}

Polygon3D::Polygon3D(const Polygon3D& p)
{
	_indices[0] = p.GetIndex(0);
	_indices[1] = p.GetIndex(1);
	_indices[2] = p.GetIndex(2);
	_polygonCulled = p._polygonCulled;
	_polygonZ = p._polygonZ;
	_normalVector = p._normalVector;
}

bool Polygon3D::GetPolygonCulled() const
{
	return _polygonCulled;
}

void Polygon3D::SetPolygonCulled(const bool polygonCulled)
{
	_polygonCulled = polygonCulled;
}

float Polygon3D::getPolygonZ() const
{
	return _polygonZ;
}

void Polygon3D::setPolygonZ(const float polygonZ)
{
	_polygonZ = polygonZ;
}

Vector3D Polygon3D::getNormalVector() const
{
	return _normalVector;
}

void Polygon3D::setNormalVector(const Vector3D normalVector)
{
	_normalVector = normalVector;
}

COLORREF Polygon3D::getColourProperty() const
{
	return _colourProperty;
}

void Polygon3D::setColourProperty(const COLORREF colourProperty)
{
	_colourProperty = colourProperty;
}

int Polygon3D::GetIndex(int index) const
{
	return _indices[index];
}

Polygon3D& Polygon3D::operator=(const Polygon3D& rhs)
{
	if (this != &rhs)
	{
		_indices[0] = rhs.GetIndex(0);
		_indices[1] = rhs.GetIndex(1);
		_indices[2] = rhs.GetIndex(2);
		_polygonZ = rhs._polygonZ;
	}
	return *this;
}