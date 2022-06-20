#include "Camera.h"
#include <math.h>
#include "Model.h"

Camera::Camera()
{
	_xRotation = 0.0f;
	_yRotation = 0.0f;
	_zRotation = 0.0f;
}

Camera::Camera(float xRotation, float yRotation, float zRotation, const Vertex& position)
{
	_xRotation = xRotation;
	_yRotation = yRotation;
	_zRotation = zRotation;
	_cameraPosition = position;
	viewingMatrix();
}

Matrix Camera::getCameraMatrix()
{
	return viewMatrix;
}

float Camera::GetXRotation() const
{
	return _xRotation;
}

void Camera::SetXRotation(const float xRotation)
{
	_xRotation = xRotation;
	viewingMatrix();
}

float Camera::GetYRotation() const
{
	return _yRotation;
}

void Camera::SetYRotation(const float yRotation)
{
	_yRotation = yRotation;
	viewingMatrix();
}

float Camera::GetZRotation() const
{
	return _zRotation;
}

void Camera::SetZRotation(const float zRotation)
{
	_zRotation = zRotation;
	viewingMatrix();
}

Vertex Camera::GetCameraPosition() const
{
	return _cameraPosition;
}

void Camera::SetCameraPosition(const Vertex position)
{
	_cameraPosition = position;
}

void Camera::viewingMatrix()
{
	Matrix xTransform{ 1, 0, 0, 0,
					   0, cosf(_xRotation), sinf(_xRotation), 0,
					   0, -sinf(_xRotation), cosf(_xRotation), 0,
					   0, 0, 0, 1 };

	Matrix yTransform{ cosf(_yRotation), 0, -sinf(_yRotation), 0,
					   0, 1, 0, 0,
					   sinf(_yRotation), 0, cosf(_yRotation), 0,
					   0, 0, 0, 1 };

	Matrix zTransform{ cosf(_zRotation), sinf(_zRotation), 0, 0,
					   -sinf(_zRotation), cosf(_zRotation), 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1 };

	Matrix translationTransform{ 1, 0, 0, -_cameraPosition.GetX(),
							0, 1, 0, -_cameraPosition.GetY(),
							0, 0, 1, -_cameraPosition.GetZ(),
							0, 0, 0, 1 };

	viewMatrix = xTransform * yTransform * zTransform * translationTransform;
}