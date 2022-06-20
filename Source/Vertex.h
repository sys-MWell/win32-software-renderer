#pragma once
#include "Vector3D.h"
#include "framework.h"

class Vertex
{
public:
	Vertex();
	Vertex(float x, float y, float z, float w);
	Vertex(const Vertex& other);

	// Accessors
	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float y);
	float GetW() const;
	void SetW(const float w);

	void dehomogenize();
	 
	COLORREF GetVertexRGB() const;
	void SetVertexRGB(const COLORREF vertexColourProperty);

	//Extending vertex class, normal vectors for each vertex
	Vector3D GetNormalVector() const;
	void SetNormalVector(const Vector3D normalVector);

	//Count on each vertex
	int GetVertexCount() const;
	void SetVertexCount(const int vertexCount);

	// Assignment operator
	Vertex& operator= (const Vertex& rhs);

	bool operator== (const Vertex& rhs) const;

	const Vertex operator+ (const Vertex& rhs) const;

	const Vector3D operator- (const Vertex& other) const;

private:
	float _x;
	float _y;
	float _z;
	float _w;
	COLORREF _vertexColourProperty;
	Vector3D _normalVector;
	int _vertexCount;
};

