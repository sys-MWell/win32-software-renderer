#pragma once
#include <math.h>

class Vector3D
{
public:
	Vector3D();
	Vector3D(float vectorX, float vectorY, float vectorZ);
	Vector3D(const Vector3D& other);

	float GetVectorX() const;
	void SetVectorX(const float vectorX);
	float GetVectorY() const;
	void SetVectorY(const float vectorY);
	float GetVectorZ() const;
	void SetVectorZ(const float vectorZ);

	Vector3D& operator=(const Vector3D& rhs);

	const Vector3D operator+ (const Vector3D& other) const;

	static float DotProduct(Vector3D vectorU, Vector3D vectorV);
	static Vector3D VectorCrossProduct(Vector3D vectorU, Vector3D vectorV);

	void normalise();

private:
	float _vectorX;
	float _vectorY;
	float _vectorZ;
};

