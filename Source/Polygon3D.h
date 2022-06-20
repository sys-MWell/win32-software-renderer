#pragma once
#include "framework.h"
#include "Vector3D.h"

class Polygon3D
{
public:
	Polygon3D();
	Polygon3D(int index0, int index1, int index2);
	~Polygon3D();
	Polygon3D(const Polygon3D& p);

	bool GetPolygonCulled() const;
	void SetPolygonCulled(const bool polygonCulled);

	float getPolygonZ() const;
	void setPolygonZ(const float polygonZ);

	Vector3D getNormalVector() const;
	void setNormalVector(const Vector3D normalVector);

	COLORREF getColourProperty() const;
	void setColourProperty(const COLORREF colourProperty);

	//Accessor to return index of specified vertex
	int GetIndex(int index) const;

	Polygon3D& operator= (const Polygon3D& rhs);


private:
	Vector3D _normalVector;
	COLORREF _colourProperty;
	int _indices[3];
	bool _polygonCulled;
	float _polygonZ;
};

