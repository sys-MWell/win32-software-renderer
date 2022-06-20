#pragma once
#include "Vertex.h"
#include "Matrix.h"


class Camera
{
public:
	Camera();
	Camera(float xRotation, float yRotation, float zRotation, const Vertex& position);
	Matrix getCameraMatrix();
	void viewingMatrix();
	float GetXRotation() const;
	void SetXRotation(const float xRotation);
	float GetYRotation() const;
	void SetYRotation(const float yRotation);
	float GetZRotation() const;
	void SetZRotation(const float zRotation);

	Vertex GetCameraPosition() const;
	void SetCameraPosition(const Vertex position);

private:
	float _xRotation;
	float _yRotation;
	float _zRotation;
	Vertex _cameraPosition;
	Matrix viewMatrix;
};
