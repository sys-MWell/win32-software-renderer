#include "Rasteriser.h"

Rasteriser app;

bool Rasteriser::Initialise()
{
	//D:\\maxha\\OneDrive - University of Derby\\Programs\\C++\\Exercises\\models\\cartman.md2
	//D:\\OneDrive - University of Derby\\Programs\\C++\\Exercises\\models\\cube.md2
	//C:\\Users\\100540796\\OneDrive - University of Derby\\Programs\\C++\\Exercises\\models\\cube.md2
	if (!MD2Loader::LoadModel("cartman.md2", _model, &Model::AddPolygon, &Model::AddVertex))
		
	{
		return false;
	}

	//Calculate directional lighting
	lightDirection = { 1,0,1 }; //Directional lighting - light source direction
	newDirectionalLighting = { 0,255,0, lightDirection}; //New directional light object - Assigns colour of directional light and light source direction
	directionalLights.push_back(newDirectionalLighting);

	//Ambient light colour
	amibentLights = { 100,0,100};

	//Calculate point lighting
	lightPosition = Vertex( 0,0,-50, 1); //Point lighting position
	newPointLighting = PointLighting(0,0,255,0,1,0,lightPosition); //Point light object, assigns colours, attenuation coefficients and position
	pointLights.push_back(newPointLighting);

	//New camera object
	newCamera.viewingMatrix();
	return true;
}

void Rasteriser::DrawWireFrame(const Bitmap& bitmap)
{
	HDC hDc = bitmap.GetDC();
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); //Blue pen created
	HPEN oldPen = (HPEN)SelectObject(hDc, pen);

	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		Vertex point1 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(0)];
		Vertex point2 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(1)];
		Vertex point3 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(2)];
		//Wireframe method, uses individual lines to draw each polygon
		if (_model.GetPolygons().at(i).GetPolygonCulled() != true)
			{ 
			MoveToEx(hDc, (int)point1.GetX(), (int)point1.GetY(), NULL);
			LineTo(hDc, (int)point2.GetX(), (int)point2.GetY());
			LineTo(hDc, (int)point3.GetX(), (int)point3.GetY());
			LineTo(hDc, (int)point1.GetX(), (int)point1.GetY());
			}
	}
	//Once pens are used, they are deleted, clears space in memory
	SelectObject(hDc, oldPen);
	DeleteObject(pen);
}

void Rasteriser::DrawSolidFlat(const Bitmap& bitmap)
{
	HDC hDc = bitmap.GetDC();

	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		//For each vertex, points 1,2,3
		Vertex updatedVerticesPoint1 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(0)];
		Vertex updatedVerticesPoint2 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(1)];
		Vertex updatedVerticesPoint3 = _model.GetUpdatedVertices()[_model.GetPolygons()[i].GetIndex(2)];

		if (_model.GetPolygons().at(i).GetPolygonCulled() != true)
		{
			HPEN pen = CreatePen(PS_SOLID, 1, _model.GetPolygons().at(i).getColourProperty());
			HPEN oldPen = (HPEN)SelectObject(hDc, pen);
			HBRUSH hBrush = CreateSolidBrush(_model.GetPolygons().at(i).getColourProperty());
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hDc, hBrush);

			//Converts eacb Vertex Point into a POINT value
			POINT polygonPoint1 = POINT{ (long)updatedVerticesPoint1.GetX(), (long)updatedVerticesPoint1.GetY() };
			POINT polygonPoint2 = POINT{ (long)updatedVerticesPoint2.GetX(), (long)updatedVerticesPoint2.GetY() };
			POINT polygonPoint3 = POINT{ (long)updatedVerticesPoint3.GetX(), (long)updatedVerticesPoint3.GetY() };

			//This method draws a polygon that is connected using points, instead of individual lines that make a polygon
			POINT polygonPoints[3] = { polygonPoint1, polygonPoint2, polygonPoint3 };
			Polygon(hDc, polygonPoints, sizeof(polygonPoints) / sizeof(polygonPoints[0]));

			SelectObject(hDc, oldPen);
			SelectObject(hDc, hOldBrush);
			DeleteObject(hBrush);
			DeleteObject(pen);
		}
	}
}

//Used for standard algorithm, sorts verticies by Y values, smallest to largest
bool Rasteriser::sortVerticesAscendingByY(const Vertex& lhs, const Vertex& rhs)
{
	return (lhs.GetY() < rhs.GetY());
}

void Rasteriser::MyDrawSolidFlat(const Bitmap& bitmap)
{
	HDC hdc = bitmap.GetDC();

	/*MyDrawSolidFlat, gets each individual vertex of a polygon and the polygons colour, pushes them onto a vector array
	and passed through to the FillyPolygonGouraud method to be sorted and either drawn as bottom or top polygon*/
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
			FillPolygonGouraud(hdc, culledPolygonVertices);
			culledPolygonVertices.clear();
		}
	}
}

void Rasteriser::FillPolygonGouraud(HDC& hdc, vector<Vertex>& polygonVertices)
{
	//Sorts the vertices within the polygon from smallest to largest according to the Y axis
	sort(polygonVertices.begin(), polygonVertices.end(), sortVerticesAscendingByY);

	//Takes the 3 vertices passed through from the MyDrawSolidFlat method and assigns them to a local variables
	Vertex vertex1 = polygonVertices[0];
	Vertex vertex2 = polygonVertices[1];
	Vertex vertex3 = polygonVertices[2];

	//Gets the colourref stored within the Vertex class for each vertex and stores them as a local variable
	COLORREF vertexColour1 = vertex1.GetVertexRGB();
	COLORREF vertexColour2 = vertex2.GetVertexRGB();
	COLORREF vertexColour3 = vertex3.GetVertexRGB();

	/*
	 * Fills a triangle whose bottom side is perfectly horizontal.
	 * Precondition is that v2 and v3 perform the flat side and that v1.y < v2.y, v3.y.
	 * @param g Graphics object
	 * @param v1 first vertice, has the smallest y-coordinate
	 * @param v2 second vertice
	 * @param v3 third vertice
	 */

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
		Vertex vTmp = Vertex((vertex1.GetX() + ((float)(vertex2.GetY() - vertex1.GetY()) / (float)(vertex3.GetY() - vertex1.GetY())) * (vertex3.GetX() - vertex1.GetX())), (float)vertex2.GetY(), (float)1, (float)1);
		//Gets the colourref stored within the local variables, alters the colours to correspond to the position of the polygons and converts the colourref to integer RGB values
		float cBlue = GetBValue(vertexColour1) + ((float)(vertex2.GetY() - vertex1.GetY()) / (float)(vertex3.GetY() - vertex1.GetY())) * (GetBValue(vertexColour3) - GetBValue(vertexColour1));
		float cRed = GetRValue(vertexColour1) + ((float)(vertex2.GetY() - vertex1.GetY()) / (float)(vertex3.GetY() - vertex1.GetY())) * (GetRValue(vertexColour3) - GetRValue(vertexColour1));
		float cGreen = GetGValue(vertexColour1) + ((float)(vertex2.GetY() - vertex1.GetY()) / (float)(vertex3.GetY() - vertex1.GetY())) * (GetGValue(vertexColour3) - GetGValue(vertexColour1));
		COLORREF cTmp = RGB(cRed, cGreen, cBlue);

		FillBottomFlatTriangle(hdc, vertex1, vertex2, vTmp, vertexColour1, vertexColour2, cTmp);
		//FillBottomFlatTriangle(hdc, vertex1, vertex2, vTmp, cTmp, vertexColour1, vertexColour2);
		FillTopFlatTriangle(hdc, vertex2, vTmp, vertex3, vertexColour2, cTmp, vertexColour3);
		//FillTopFlatTriangle(hdc, vertex2, vTmp, vertex3, vertexColour3, vertexColour2, cTmp);
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

	//Changed slope values depending on the triangle side positions
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

	for (int scanlineY = (int)round(vertex1.GetY()); scanlineY <= (int)vertex2.GetY(); scanlineY++)
	{
		/* loop over each pixel of horizontal line */
		for (int xPos = (int)ceil(x1); xPos < (int)x2; xPos++)
		{
			float t = (xPos - x1) / (x2 - x1);
			int red = (int)((1 - t) * cRed1 + t * cRed2);
			int green = (int)((1 - t) * cGreen1 + t * cGreen2);
			int blue = (int)((1 - t) * cBlue1 + t * cBlue2);

			COLORREF fillColour = RGB(red, green, blue);
			SetPixel(hdc, xPos, (int)scanlineY, fillColour);
		}
		/* get new x-coordinate of endpoints of horizontal line */
		x1 += slope1;
		x2 += slope2;
		/* get new color of left endpoint of horizontal line */
		cRed1 = cRed1 + colourSlopeRed1;
		cGreen1 = cGreen1 + colourSlopeGreen1;
		cBlue1 = cBlue1 + colourSlopeBlue1;
		/* get new color of right endpoint of horizontal line */
		cRed2 = cRed2 + colourSlopeRed2;
		cGreen2 = cGreen2 + colourSlopeGreen2;
		cBlue2 = cBlue2 + colourSlopeBlue2;
	}
}

void Rasteriser::FillTopFlatTriangle(const HDC& hdc, Vertex vertex1, Vertex vertex2, Vertex vertex3, COLORREF vertColour1, COLORREF vertColour2, COLORREF vertColour3)
{
	float slope1 = (float)(vertex3.GetX() - vertex1.GetX()) / (float)(vertex3.GetY() - vertex1.GetY());
	float slope2 = (float)(vertex3.GetX() - vertex2.GetX()) / (float)(vertex3.GetY() - vertex2.GetY());

	float x1 = vertex3.GetX();
	float x2 = vertex3.GetX() + 0.5f;

	/* get the change of color components along edge (v3,v1) */
	float v3v1Diff = (float)(vertex3.GetY() - vertex1.GetY());
	float colourSlopeBlue1 = (GetBValue(vertColour3) - GetBValue(vertColour1)) / v3v1Diff;
	float colourSlopeRed1 = (GetRValue(vertColour3) - GetRValue(vertColour1)) / v3v1Diff;
	float colourSlopeGreen1 = (GetGValue(vertColour3) - GetGValue(vertColour1)) / v3v1Diff;

	/* get the change of color components along edge (v3,v2) */
	float v3v2Diff = (float)(vertex3.GetY() - vertex2.GetY());
	float colourSlopeBlue2 = (GetBValue(vertColour3) - GetBValue(vertColour2)) / v3v2Diff;
	float colourSlopeRed2 = (GetRValue(vertColour3) - GetRValue(vertColour2)) / v3v2Diff;
	float colourSlopeGreen2 = (GetGValue(vertColour3) - GetGValue(vertColour2)) / v3v2Diff;

	/* set starting values */
	float cBlue1 = GetBValue(vertColour3);
	float cRed1 = GetRValue(vertColour3);
	float cGreen1 = GetGValue(vertColour3);
	float cBlue2 = GetBValue(vertColour3);
	float cRed2 = GetRValue(vertColour3);
	float cGreen2 = GetGValue(vertColour3);

	/* as we will do not fill in a complete line using fillrect but instead
	 * we will loop over all pixels of a horizontal line, we need a predefined
	 * direction -> choose left to right. This means that x1 must be the smaller
	 * compared to x2 so slope1 must be smaller than slope2. If not we gonna
	 * swap it here.
	 */
	if (slope1 < slope2)
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

	for (int scanlineY = (int)round(vertex3.GetY()); (int)scanlineY > (int)vertex1.GetY(); scanlineY--)
	{
		/* loop over each pixel of horizontal line */
		for (int xPos = (int)ceil(x1); xPos < (int)x2; xPos++)
		{
			float t = (xPos - x1) / (x2 - x1);

			int red = (int)((1 - t) * cRed1 + t * cRed2);
			int green = (int)((1 - t) * cGreen1 + t * cGreen2);
			int blue = (int)((1 - t) * cBlue1 + t * cBlue2);

			COLORREF fillColour = RGB(red, green, blue);
			SetPixel(hdc, xPos, (int)scanlineY, fillColour);
		}
		/* get new x-coordinate of endpoints of horizontal line */
		x1 -= slope1;
		x2 -= slope2;
		/* get new color of left endpoint of horizontal line */
		cRed1 -= colourSlopeRed1;
		cGreen1 -= colourSlopeGreen1;
		cBlue1 -= colourSlopeBlue1;
		/* get new color of right endpoint of horizontal line */
		cRed2 -= colourSlopeRed2;
		cGreen2 -= colourSlopeGreen2;
		cBlue2 -= colourSlopeBlue2;
	}
}

//DrawString method is used to display text on screen
void Rasteriser::DrawString(const Bitmap& bitmap, LPCTSTR text)
{
	HDC hDc = bitmap.GetDC();
	HFONT hFont, hOLDFont;

	// Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	// Select the variable stock font into the specified device context. 
	if (hOLDFont = (HFONT)SelectObject(hDc, hFont))
	{
		SetTextColor(hDc, RGB(255, 255, 255));
		SetBkColor(hDc, RGB(0, 0, 0));

		// Display the text string.  
		TextOut(hDc, 10, 10, text, lstrlen(text));

		// Restore the original font.        
		SelectObject(hDc, hOLDFont);
	}
	DeleteObject(hFont);
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

void Rasteriser::Scale()
{
	Matrix scale{ (float)scaleValue, 0, 0, 0,
				  0, (float)scaleValue, 0, 0,
				  0, 0, (float)scaleValue, 0,
				  0, 0, 0, 1 };
	scaleValue = scaleValue + 0.005;
	currentModelTransformation = scale;
}

void Rasteriser::xRotation()
{
	xDegrees = xDegrees + 1.00;
	double x = xDegrees * 3.14159 / 180.0;

	Matrix rotateX{ 1, 0, 0, 0,
					0, (float)cos(x), (float)-sin(x), 0,
					0, (float)sin(x), (float)cos(x), 0,
					0, 0, 0, 1 };
	currentModelTransformation = rotateX;
}

void Rasteriser::yRotation()
{
	yDegrees = yDegrees + 1.00;
	double y = yDegrees * 3.14159 / 180.0;

	Matrix rotateY{ (float)cos(y), 0, (float)sin(y), 0,
					0, 1, 0, 0,
					(float)-sin(y), 0, (float)cos(y), 0,
					0, 0, 0, 1 };
	currentModelTransformation = rotateY;
}

void Rasteriser::zRotation()
{
	zDegrees = zDegrees + 1.00;
	double z = zDegrees * 3.14159 / 180.0;

	Matrix rotateZ{ (float)cos(z), (float)-sin(z), 0, 0,
			        (float)sin(z), (float)cos(z), 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 };
	currentModelTransformation = rotateZ;
}

void Rasteriser::Translation()
{
	translationDimension += 1;
	Vertex dimensionChange = Vertex(float(translationDimension),0,0,1);
	Matrix translation{ 1, 0, 0, -(float)dimensionChange.GetX(),
						0, 1, 0, -(float)dimensionChange.GetY(),
						0, 0, 1, -(float)dimensionChange.GetZ(),
						0, 0, 0, 1 };
	currentModelTransformation = translation;
}

void Rasteriser::Update(const Bitmap& bitmap)
{
	if (newcount <= 200)
	{
		Scale();
	}

	if (newcount >= 201 && newcount <= 700)
	{
		xRotation();
	}

	if (newcount >= 701 && newcount <= 1200)
	{
		yRotation();
	}

	if (newcount >= 1201 && newcount <= 1700)
	{
		zRotation();
	}

	if (newcount >= 1701 && newcount <= 1790)
	{
		Translation();
	}

	if (newcount >= 1791)
	{
		yRotation();
	}

	newcount++;

	GeneratePerspectiveMatrix(1, ((float)(bitmap.GetWidth()) / (float)(bitmap.GetHeight())));
	GenerateViewMatrix(1, (bitmap.GetWidth()), (bitmap.GetHeight()));
}

void Rasteriser::Render(const Bitmap& bitmap)
{
	bitmap.Clear(RGB(0, 0, 0));
	
	if (newcount <= 200)
	{
		DrawString(bitmap, L"Wireframe - Scale");
		_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform
		_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix()); //Camera transform
		_model.ApplyTransformToTransformedVertices(perspectiveMatrix); //Apply perspective matrix to transformed vertices
		_model.ApplyDehomogenizeToTransformedVertices();
		_model.ApplyTransformToTransformedVertices(screenTransformation); //Apply screen transformation to transformed vertices
		DrawWireFrame(bitmap);
	}

	if (newcount >= 201 && newcount <= 700)
	{
		DrawString(bitmap, L"Wireframe - Rotation x");
		_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform
		_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix());
		_model.ApplyTransformToTransformedVertices(perspectiveMatrix);
		_model.ApplyDehomogenizeToTransformedVertices();
		_model.ApplyTransformToTransformedVertices(screenTransformation);
		DrawWireFrame(bitmap);
	}

	if (newcount >= 701 && newcount <= 1200)
	{
		DrawString(bitmap, L"Wireframe - Rotation y");
		_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform
		_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix());
		_model.ApplyTransformToTransformedVertices(perspectiveMatrix);
		_model.ApplyDehomogenizeToTransformedVertices();
		_model.ApplyTransformToTransformedVertices(screenTransformation);
		DrawWireFrame(bitmap);
	}

	if (newcount >= 1201 && newcount <= 1700)
	{
		DrawString(bitmap, L"Wireframe - Rotation z");
		_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform
		_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix());
		_model.ApplyTransformToTransformedVertices(perspectiveMatrix);
		_model.ApplyDehomogenizeToTransformedVertices();
		_model.ApplyTransformToTransformedVertices(screenTransformation);
		DrawWireFrame(bitmap);
	}

	if (newcount >= 1701 && newcount <= 1790)
	{
		DrawString(bitmap, L"Wireframe - Translation");
		_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform
		_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix());
		_model.ApplyTransformToTransformedVertices(perspectiveMatrix);
		_model.ApplyDehomogenizeToTransformedVertices();
		_model.ApplyTransformToTransformedVertices(screenTransformation);
		DrawWireFrame(bitmap);
	}

	if (newcount >= 1791 && newcount <= 2500)
	{
		DrawString(bitmap, L"Wireframe - With Backface Culling");
		_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform
		_model.Sort();
		_model.CalculateBackfaces(newCamera);
		_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix());
		_model.ApplyTransformToTransformedVertices(perspectiveMatrix);
		_model.ApplyDehomogenizeToTransformedVertices();
		_model.ApplyTransformToTransformedVertices(screenTransformation);
		DrawWireFrame(bitmap);
	}

	if (newcount >= 2501 && newcount <= 3000)
	{
		DrawString(bitmap, L"Solid GDI Shading - Ambient Lighting");
		_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform
		_model.Sort();
		_model.CalculateBackfaces(newCamera);
		_model.CalculateLightingAmbient(amibentLights);
		_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix());
		_model.ApplyTransformToTransformedVertices(perspectiveMatrix);
		_model.ApplyDehomogenizeToTransformedVertices();
		_model.ApplyTransformToTransformedVertices(screenTransformation);
		DrawSolidFlat(bitmap);
	}

	if (newcount >= 3001 && newcount <= 3500)
	{
		DrawString(bitmap, L"Solid GDI Shading - +Directional Lighting");
		_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform
		_model.Sort();
		_model.CalculateBackfaces(newCamera);
		_model.CalculateLightingAmbient(amibentLights);
		_model.CalculateLightingDirectional(directionalLights);
		_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix());
		_model.ApplyTransformToTransformedVertices(perspectiveMatrix);
		_model.ApplyDehomogenizeToTransformedVertices();
		_model.ApplyTransformToTransformedVertices(screenTransformation);
		DrawSolidFlat(bitmap);
	}

	if (newcount >= 3501 && newcount <= 4000)
	{
		DrawString(bitmap, L"Solid GDI Shading - +Point Lighting");
		_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform
		_model.Sort();
		_model.CalculateBackfaces(newCamera);
		_model.CalculateLightingAmbient(amibentLights);
		_model.CalculateLightingDirectional(directionalLights);
		_model.CalculateLightingPoint(pointLights);
		_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix());
		_model.ApplyTransformToTransformedVertices(perspectiveMatrix);
		_model.ApplyDehomogenizeToTransformedVertices();
		_model.ApplyTransformToTransformedVertices(screenTransformation);
		DrawSolidFlat(bitmap);
	}

	if (newcount >= 4001 && newcount <= 4500)
	{
		DrawString(bitmap, L"Own rasterization code");
		_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform
		_model.Sort();
		_model.CalculateBackfaces(newCamera);
		_model.CalculateLightingAmbient(amibentLights);
		_model.CalculateLightingDirectional(directionalLights);
		_model.CalculateLightingPoint(pointLights);
		_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix());
		_model.ApplyTransformToTransformedVertices(perspectiveMatrix);
		_model.ApplyDehomogenizeToTransformedVertices();
		_model.ApplyTransformToTransformedVertices(screenTransformation);
		MyDrawSolidFlat(bitmap);
	}

	if (newcount >= 4501)
	{
		DrawString(bitmap, L"Gouraud shading");
		_model.ApplyTransformToLocalVertices(currentModelTransformation); //world transform
		_model.Sort();
		_model.CalculateBackfaces(newCamera);
		//_model.CalculateNormalVectors();
		_model.CalculateLightingAmbient(amibentLights);
		_model.CalculateLightingDirectional(directionalLights);
		_model.CalculateLightingPoint(pointLights);
		_model.ApplyTransformToTransformedVertices(newCamera.getCameraMatrix());
		_model.ApplyTransformToTransformedVertices(perspectiveMatrix);
		_model.ApplyDehomogenizeToTransformedVertices();
		_model.ApplyTransformToTransformedVertices(screenTransformation);
		MyDrawSolidFlat(bitmap);
	}
}