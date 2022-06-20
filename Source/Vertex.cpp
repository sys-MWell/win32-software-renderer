#include "Vertex.h"

Vertex::Vertex()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
	_w = 0.0f;
	_vertexColourProperty = RGB(0, 0, 0);
	_normalVector = { 0,0,0 };
	_vertexCount = 0;
}

Vertex::Vertex(float x, float y, float z, float w)
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;
	_vertexColourProperty = _vertexColourProperty;
	_normalVector = { 0,0,0 };
	_vertexCount = 0;
}

Vertex::Vertex(const Vertex& other)
{
	_x = other.GetX();
	_y = other.GetY();
	_z = other.GetZ();
	_w = other.GetW();
	_vertexColourProperty = other.GetVertexRGB();
	_normalVector = other.GetNormalVector();
	_vertexCount = other.GetVertexCount();
}

float Vertex::GetX() const
{
	return _x;
}

void Vertex::SetX(const float x)
{
	_x = x;
}

float Vertex::GetY() const
{
	return _y;
}

void Vertex::SetY(const float y)
{
	_y = y;
}

float Vertex::GetZ() const
{
	return _z;
}

void Vertex::SetZ(const float z)
{
	_z = z;
}

float Vertex::GetW() const
{
	return _w;
}

void Vertex::SetW(const float w)
{
	_w = w;
}

void Vertex::dehomogenize()
{
	_x = _x / _w;
	_y = _y / _w;
	_z = _z / _w;
	_w = _w / _w;
}

COLORREF Vertex::GetVertexRGB() const
{
	return _vertexColourProperty;
}

void Vertex::SetVertexRGB(const COLORREF vertexColourProperty)
{
	_vertexColourProperty = vertexColourProperty;
}

Vector3D Vertex::GetNormalVector() const
{
	return _normalVector;
}

void Vertex::SetNormalVector(const Vector3D normalVector)
{
	_normalVector = normalVector;
}

int Vertex::GetVertexCount() const
{
	return _vertexCount;
}

void Vertex::SetVertexCount(const int vertexCount)
{
	_vertexCount = vertexCount;
}

Vertex& Vertex::operator=(const Vertex& rhs)
{
	// Only do the assignment if we are not assigning
	// to ourselvesn
	if (this != &rhs)
	{
		_x = rhs.GetX();
		_y = rhs.GetY();
		_z = rhs.GetZ();
		_w = rhs.GetW();
	}
	return *this;
}

// The const at the end of the declaraion for '==" indicates that this operation does not change
// any of the member variables in this class.

bool Vertex::operator==(const Vertex& rhs) const
{
	return (_x == rhs.GetX() && _y == rhs.GetY() && _z == rhs.GetZ() && _w == rhs.GetW());
}

// You can see three different uses of 'const' here:
//
// The first const indicates that the method changes the return value, but it is not moved in memory
// The second const indicates that the parameter is passed by reference, but it is not modified
// The third const indicates that the operator does not change any of the memory variables in the class.

const Vertex Vertex::operator+(const Vertex& rhs) const
{
	return Vertex(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ(), _w + rhs.GetW());
}

const Vector3D Vertex::operator-(const Vertex& other) const
{
	return Vector3D((_x - other.GetX()), (_y - other.GetY()), (_z - other.GetZ()));
}

