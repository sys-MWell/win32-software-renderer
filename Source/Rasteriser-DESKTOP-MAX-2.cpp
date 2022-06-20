#include "Rasteriser.h"
#include "Vertex.h"
#include "Bitmap.h"
#include "Matrix.h"
#include "Model.h"
#include "MD2Loader.h"
#include <math.h>
#include "Camera.h"
#include <algorithm>

Rasteriser app;
Model _model;

bool Rasteriser::Initialise()
{
	//D:\\OneDrive - University of Derby\\Programs\\C++\\Exercises\\models\\cube.md2
	//C:\\Users\\100540796\\OneDrive - University of Derby\\Programs\\C++\\Exercises\\models\\cube.md2
	if (!MD2Loader::LoadModel("D:\\OneDrive - University of Derby\\Programs\\C++\\Exercises\\models\\cartman.md2", _model, &Model::AddPolygon, &Model::AddVertex))
		
	{
		return false;
	}

	lightDirection = { 1,0,1 };

	newDirectionalLighting = { 0,255,0, lightDirection};
	directionalLights.push_back(newDirectionalLighting);

	amibentLights = { 100,0,100};

	lightPosition = Vertex( 0,0,-50, 1);
	newPointLighting = PointLighting(0,0,255,0,1,0,lightPosition);
	pointLights.push_back(newPointLighting);

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


	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		Vertex updatedVerticesPoint1 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(0)];
		Vertex updatedVerticesPoint2 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(1)];
		Vertex updatedVerticesPoint3 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(2)];

		if (_model.GetPolygons().at(i).GetPolygonCulled() != true)
		{
			HPEN pen = CreatePen(PS_SOLID, 1, _model.GetPolygons().at(i).getColourProperty());
			HPEN oldPen = (HPEN)SelectObject(hDc, pen);
			HBRUSH hBrush = CreateSolidBrush(_model.GetPolygons().at(i).getColourProperty());
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hDc, hBrush);

			POINT polygonPoint1 = POINT{ (long)updatedVerticesPoint1.GetX(), (long)updatedVerticesPoint1.GetY() };
			POINT polygonPoint2 = POINT{ (long)updatedVerticesPoint2.GetX(), (long)updatedVerticesPoint2.GetY() };
			POINT polygonPoint3 = POINT{ (long)updatedVerticesPoint3.GetX(), (long)updatedVerticesPoint3.GetY() };

			POINT polygonPoints[3] = { polygonPoint1, polygonPoint2, polygonPoint3 };
			Polygon(hDc, polygonPoints, sizeof(polygonPoints) / sizeof(polygonPoints[0]));

			SelectObject(hDc, oldPen);
			SelectObject(hDc, hOldBrush);
			DeleteObject(hBrush);
			DeleteObject(pen);
		}
	}
}

bool Rasteriser::sortVerticesAscendingByY(const Vertex& lhs, const Vertex& rhs)
{
	return (lhs.GetY() > rhs.GetY());
}

void Rasteriser::MyDrawSolidFlat(const Bitmap& bitmap)
{
	HDC hdc = bitmap.GetDC();

	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		Vertex updatedVerticesPoint1 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(0)];
		Vertex updatedVerticesPoint2 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(1)];
		Vertex updatedVerticesPoint3 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(2)];

		updatedVerticesPoint1.SetVertexRGB(_model.GetPolygons().at(i).getColourProperty());
		updatedVerticesPoint2.SetVertexRGB(_model.GetPolygons().at(i).getColourProperty());
		updatedVerticesPoint3.SetVertexRGB(_model.GetPolygons().at(i).getColourProperty());

		if (_model.GetPolygons().at(i).GetPolygonCulled() != true)
		{
			vector<Vertex> culledPolygonVertices;
			culledPolygonVertices.push_back(updatedVerticesPoint1);
			culledPolygonVertices.push_back(updatedVerticesPoint2);
			culledPolygonVertices.push_back(updatedVerticesPoint3);
		}
	}
}

void Rasteriser::FillPolygon(HDC& hdc, vector<Vertex>& polygonVertices)
{

	sort(polygonVertices.begin(), polygonVertices.end(), sortVerticesAscendingByY);

	Vertex vertex1 = polygonVertices[0];
	Vertex vertex2 = polygonVertices[1];
	Vertex vertex3 = polygonVertices[2];

	COLORREF vertexColour1 = vertex1.GetVertexRGB();
	COLORREF vertexColour2 = vertex2.GetVertexRGB();
	COLORREF vertexColour3 = vertex3.GetVertexRGB();

	if ((int)vertex2.GetY() == (int)vertex3.GetY())
	{
		FillBottomFlatTriangle(hdc, vertex1, vertex2, vertex3, vertexColour1, vertexColour2, vertexColour3);
	}
	else if ((int)vertex1.GetY() == (int)vertex2.GetY())
	{
		FillTopFlatTriangle(hdc, vertex1, vertex2, vertex3, vertexColour1, vertexColour2, vertexColour3);
	}
	else
	{
		Vertex vTmp = Vertex((vertex1.GetX() + ((float)(vertex2.GetY() - vertex1.GetY()) / (float)(vertex3.GetY() - vertex1.GetY())) * (vertex3.GetX() - vertex1.GetX())), (float)1, (float)1, (float)1);
		float cBlue = GetBValue(vertexColour1) + ((float)(vertex2.GetY() - vertex1.GetY()) / (float)(vertex3.GetY() - vertex1.GetY())) * (GetBValue(vertexColour3) - GetBValue(vertexColour1));
		float cRed = GetRValue(vertexColour1) + ((float)(vertex2.GetY() - vertex1.GetY()) / (float)(vertex3.GetY() - vertex1.GetY())) * (GetRValue(vertexColour3) - GetRValue(vertexColour1));
		float cGreen = GetGValue(vertexColour1) + ((float)(vertex2.GetY() - vertex1.GetY()) / (float)(vertex3.GetY() - vertex1.GetY())) * (GetGValue(vertexColour3) - GetGValue(vertexColour1));
		COLORREF cTmp = RGB(cRed, cGreen, cBlue);

		FillBottomFlatTriangle(hdc, vertex1, vertex2, vTmp, vertexColour1, vertexColour2, cTmp);
		//FillBottomFlatTriangle(hdc, vertex1, vertex2, vTmp, cTmp, vertexColour1, vertextColour2);
		//FillTopFlatTriangle(hdc, vertex2, vTmp, vertex3, vertexColour2, cTmp, vertexColour3);
		FillTopFlatTriangle(hdc, vertex2, vTmp, vertex3, vertexColour3, vertexColour2, cTmp);
	}
}


void Rasteriser::FillBottomFlatTriangle(const HDC& hdc, Vertex vertex1, Vertex vertex2, Vertex vertex3, COLORREF vertColour1, COLORREF vertColour2, COLORREF vertColour3)
{
	float slope1 = (float)(vertex2.GetX() - vertex1.GetX()) / (float)(vertex2.GetY() - vertex1.GetY());
	float slope2 = (float)(vertex3.GetX() - vertex1.GetX()) / (float)(vertex3.GetY() - vertex1.GetY());

	float x1 = (float)vertex1.GetX();
	float x2 = (float)vertex1.GetX() + 0.5f;

	/* get the change of color components along edge (v2,v1) */
	float v2v1Diff = (float)(vertex2.GetY() - vertex1.GetY());
	float colourSlopeBlue1 = (GetBValue(vertColour2) - GetBValue(vertColour1)) / v2v1Diff;
	float colourSlopeRed1 = (GetRValue(vertColour2) - GetRValue(vertColour1)) / v2v1Diff;
	float colourSlopeGreen1 = (GetGValue(vertColour2) - GetGValue(vertColour1)) / v2v1Diff;

	/* get the change of color components along edge (v3,v1) */
	float v3v1Diff = (float)(vertex3.GetY() - vertex1.GetY());
	float colourSlopeBlue2 = (GetBValue(vertColour3) - GetBValue(vertColour1)) / v3v1Diff;
	float colourSlopeRed2 = (GetRValue(vertColour3) - GetRValue(vertColour1)) / v3v1Diff;
	float colourSlopeGreen2 = (GetGValue(vertColour3) - GetGValue(vertColour1)) / v3v1Diff;

	/* get starting values */
	float cBlue1 = GetBValue(vertColour1);
	float cRed1 = GetRValue(vertColour1);
	float cGreen1 = GetGValue(vertColour1);
	float cBlue2 = GetBValue(vertColour1);
	float cRed2 = GetRValue(vertColour1);
	float cGreen2 = GetGValue(vertColour1);

	/* as we will do not fill in a complete line using fillrect but instead
	 * we will loop over all pixels of a horizontal line, we need a predefined
	 * direction -> choose left to right. This means that x1 must be the smaller
	 * compared to x2 so slope1 must be smaller than slope2. If not we gonna
	 * swap it here. Of course we have also to swap the colors of both line endpoints.
	 */

	if (slope2 < slope1)
	{
		float slopeTmp = slope1;
		slope1 = slope2;
		slope2 = slopeTmp;

		slopeTmp = colourSlopeRed1;
		colourSlopeRed1 = colourSlopeRed2;
		colourSlopeRed2 = slopeTmp;

		slopeTmp = colourSlopeGreen1;
		colourSlopeGreen1 = colourSlopeGreen2;
		colourSlopeGreen2 = slopeTmp;

		slopeTmp = colourSlopeBlue1;
		colourSlopeBlue1 = colourSlopeBlue2;
		colourSlopeBlue2 = slopeTmp;
	}

	for (int scanlineY = (int)vertex1.GetY(); scanlineY <= (int)vertex2.GetY(); scanlineY++)
	{
		/* loop over each pixel of horizontal line */
		for (int xPos = (int)ceil(x1); xPos < (int)x2; xPos++)
		{
			float t = (xPos - x1) / (x2 - x1);

			int red = (int)((1 - t) * cRed1 + t * cRed2);
			int green = (int)((1 - t) * cGreen1 + t * cGreen2);
			int blue = (int)((1 - t) * cBlue1 + t * cBlue2);

			COLORREF fillColour = RGB(red, green, blue);

			SetPixel(hdc, xPos, scanlineY, fillColour);
			//g.setColor(new Color(red, green, blue));
			//g.fillRect(xPos, scanlineY, 1, 1);
		}
		/* get new x-coordinate of endpoints of horizontal line */
		x1 += slope1;
		x2 += slope2;
		/* get new color of left endpoint of horizontal line */
		cRed1 += colourSlopeRed1;
		cGreen1 += colourSlopeGreen1;
		cBlue1 += colourSlopeBlue1;
		/* get new color of right endpoint of horizontal line */
		cRed2 += colourSlopeRed2;
		cGreen2 += colourSlopeGreen2;
		cBlue2 += colourSlopeBlue2;
	}
}

void Rasteriser::FillTopFlatTriangle(const HDC& hdc, Vertex vertex1, Vertex vertex2, Vertex vertex3, COLORREF vertColour1, COLORREF vertColour2, COLORREF vertColour3)
{

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
	bitmap.Clear(RGB(0, 0, 0));

	_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform

	_model.Sort();

	_model.CalculateBackfaces(newCamera);

	_model.CalculateLightingAmbient(amibentLights);

	_model.CalculateLightingDirectional(directionalLights);

	_model.CalculateLightingPoint(pointLights);

	_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix()); //

	_model.ApplyTransformToTransformedVertices(perspectiveMatrix); //Apply perspective transform to the transformed vertices

	_model.ApplyDehomogenizeToTransformedVertices();

	_model.ApplyTransformToTransformedVertices(screenTransformation);

	//MyDrawSolidFlat(bitmap);
	DrawSolidFlat(bitmap);
	//DrawWireFrame(bitmap);
}