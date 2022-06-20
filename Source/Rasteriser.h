#pragma once
#include "Framework.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Camera.h"
#include "Polygon3D.h"
#include "DirectionalLighting.h"
#include "AmbientLighting.h"
#include "PointLighting.h"
#include <vector>
#include "Model.h"
#include "Bitmap.h"
#include "MD2Loader.h"
#include <math.h>
#include <algorithm>

class Rasteriser : public Framework
{
public:
	bool Initialise();

	void Scale();
	void xRotation();
	void yRotation();
	void zRotation();
	void Translation();

	void Update(const Bitmap& bitmap);
	void Render(const Bitmap& bitmap);
	void DrawWireFrame(const Bitmap& bitmap);
	void DrawSolidFlat(const Bitmap& bitmap);

	static bool sortVerticesAscendingByY(const Vertex& lhs, const Vertex& rhs);

	void MyDrawSolidFlat(const Bitmap& bitmap);
	void FillPolygonGouraud(HDC& hdc, vector<Vertex>& polygonVertices);
	void FillBottomFlatTriangle(const HDC& hdc, Vertex vertex1, Vertex vertex2, Vertex vertex3, COLORREF vertColour1, COLORREF vertColour2, COLORREF vertColour3);
	void FillTopFlatTriangle(const HDC& hdc, Vertex vertex1, Vertex vertex2, Vertex vertex3, COLORREF vertColour1, COLORREF vertColour2, COLORREF vertColour3);

	void DrawString(const Bitmap& bitmap, LPCTSTR text);

	void GeneratePerspectiveMatrix(float d, float aspectRatio);
	void GenerateViewMatrix(float d, int width, int height);

private:
	Model _model;

	//Values for x,y,z, scaling and translation dimensions
	double xDegrees = 1;
	double yDegrees = 1;
	double zDegrees = 1;
	double scaleValue = 1;
	double translationDimension = 1;

	Vertex defaultPosition = { 0,0,-50, 1 };
	Camera newCamera = { 0, 0, 0, defaultPosition };
	Matrix perspectiveMatrix;
	Matrix screenTransformation;

	//Ambient lighting class objects
	AmbientLighting amibentLights;

	//Directional lighting class objects
	Vector3D lightDirection;
	vector<DirectionalLighting> directionalLights;
	DirectionalLighting newDirectionalLighting;

	//Point lighting class objects
	vector<PointLighting> pointLights;
	Vertex lightPosition;
	PointLighting newPointLighting;

	Matrix currentModelTransformation;
	Matrix screenViewTransformation;

	//Count iteration, go through different worldly transformations
	int newcount = 0;
};

