#include "Rasteriser.h"
#include "Vertex.h"
#include "Bitmap.h"
#include "Matrix.h"
#include "Model.h"
#include "MD2Loader.h"
#include <math.h>
#include "Camera.h"

Rasteriser app;
Model _model;

bool Rasteriser::Initialise()
{
	//D:\\OneDrive - University of Derby\\Programs\\C++\\Exercises\\models\\cube.md2
	//C:\\Users\\100540796\\OneDrive - University of Derby\\Programs\\C++\\Exercises\\models\\cube.md2
	if (!MD2Loader::LoadModel("D:\\OneDrive - University of Derby\\Programs\\C++\\Exercises\\models\\cube.md2", _model, &Model::AddPolygon, &Model::AddVertex))
		
	{
		return false;
	}
	_directionalLights.push_back(newDirectionalLighting);
	newCamera.viewingMatrix();
	return true;
}

void Rasteriser::DrawWireFrame(const Bitmap& bitmap)
{
	HDC hDc = bitmap.GetDC();
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	HPEN oldPen = (HPEN)SelectObject(hDc, pen);

	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		Vertex point1 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(0)];
		Vertex point2 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(1)];
		Vertex point3 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(2)];
		if (_model.GetPolygons().at(i).GetPolygonCulled() != true)
			{ 
			MoveToEx(hDc, (int)point1.GetX(), (int)point1.GetY(), NULL);
			LineTo(hDc, (int)point2.GetX(), (int)point2.GetY());
			LineTo(hDc, (int)point3.GetX(), (int)point3.GetY());
			LineTo(hDc, (int)point1.GetX(), (int)point1.GetY());
			}
	}
	SelectObject(hDc, oldPen);
	DeleteObject(pen);
}

void Rasteriser::DrawSolidFlat(const Bitmap& bitmap)
{
	HDC hDc = bitmap.GetDC();
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(73, 243, 238));
	HPEN oldPen = (HPEN)SelectObject(hDc, pen);

	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		Vertex updatedVerticesPoint1 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(0)];
		Vertex updatedVerticesPoint2 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(1)];
		Vertex updatedVerticesPoint3 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(2)];
		if (_model.GetPolygons().at(i).GetPolygonCulled() != true)
		{
			POINT polygonPoint1 = POINT{ (long)updatedVerticesPoint1.GetX(), (long)updatedVerticesPoint1.GetY() };
			POINT polygonPoint2 = POINT{ (long)updatedVerticesPoint2.GetX(), (long)updatedVerticesPoint2.GetY() };
			POINT polygonPoint3 = POINT{ (long)updatedVerticesPoint3.GetX(), (long)updatedVerticesPoint3.GetY() };

			HBRUSH hBrush = CreateSolidBrush(_model.GetPolygons().at(i).getColourProperty());
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hDc, hBrush);

			POINT polygonPoints[3] = { polygonPoint1, polygonPoint2, polygonPoint3 };
			Polygon(hDc, polygonPoints, sizeof(polygonPoints) / sizeof(polygonPoints[0]));

			SelectObject(hDc, hOldBrush);
			DeleteObject(hBrush);
		}
	}
	//SelectObject(hDc, oldPen);
	//DeleteObject(pen);
}

void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	perspectiveMatrix = { d / aspectRatio, 0, 0, 0,
						  0, d, 0, 0,
						  0, 0, d, 0,
						  0, 0, 1, 0 };
}

void Rasteriser::GenerateViewMatrix(float d, int width, int height)
{
	screenTransformation = { (float)width / 2, 0, 0, (float)width / 2,
							0, (float)-height / 2, 0, (float)height / 2,
							0, 0, d / 2, d / 2,
							0, 0, 0, 1 };
}

void Rasteriser::Update(const Bitmap& bitmap)
{
	xDegrees = xDegrees + 1.00;
	double x = xDegrees * 3.14159 / 180.0;
	Matrix rotateY{ (float)cos(x), 0, (float)sin(x), 0,
					0, 1, 0, 0,
					(float)-sin(x), 0, (float)cos(x), 0,
					0, 0, 0, 1 };
	currentModelTransformation = rotateY;
	GeneratePerspectiveMatrix(1, ((float)(bitmap.GetWidth()) / (float)(bitmap.GetHeight())));
	GenerateViewMatrix(1, (bitmap.GetWidth()), (bitmap.GetHeight()));
}

void Rasteriser::Render(const Bitmap& bitmap)
{
	// clear the bitmap to white
	bitmap.Clear(RGB(255, 255, 255));

	_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform

	_model.Sort();

	_model.CalculateBackfaces(newCamera);

	_model.CalculateLightingDirectional(_directionalLights);

	_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix()); //

	_model.ApplyTransformToTransformedVertices(perspectiveMatrix); //Apply perspective transform to the transformed vertices

	_model.ApplyDehomogenizeToTransformedVertices();

	_model.ApplyTransformToTransformedVertices(screenTransformation);

	DrawSolidFlat(bitmap);
	//DrawWireFrame(bitmap);
}