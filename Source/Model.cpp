#include "Model.h"

Model::Model()
{ 
	//Directional lighting
	kd_red = 0.5f;
	kd_green = 0.5f;
	kd_blue = 0.5f;

	//Ambient lighting
	ka_red = 0.5f;
	ka_green = 0.5f;
	ka_blue = 0.5f;

	//Spot lightening
	ks_red = 0.5f;
	ks_green = 0.5f;
	ks_blue = 0.5f;
}

Model::~Model()
{
}

const vector<Polygon3D>& Model::GetPolygons()
{
	return _polygons;
}

const vector<Vertex>& Model::GetVertices()
{
	return _vertices;
}

const vector<Vertex>& Model::GetUpdatedVertices()
{
	return _updatedVertices;
}

size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

void Model::AddVertex(float x, float y, float z)
{
	Vertex vertexCollection = { (float)x,(float)y,(float)z,1};
	_vertices.push_back(vertexCollection);
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	Polygon3D polygonCollection = {i0, i1, i2};
	_polygons.push_back(polygonCollection);
}

void Model::ApplyTransformToLocalVertices(const Matrix &transform)
{
	_updatedVertices.clear();
	for (int i = 0; i < GetVertexCount(); i++)
	{
		Vertex transformationMatrix = transform * _vertices[i];
		_updatedVertices.push_back(transformationMatrix);
	}
}

void Model::ApplyTransformToTransformedVertices(const Matrix &transform)
{
	for (int i = 0; i < GetVertexCount(); i++)
	{
		_updatedVertices[i] = transform * _updatedVertices[i];
	}
}

void Model::ApplyDehomogenizeToTransformedVertices()
{
	for (int i = 0; i < GetVertexCount(); i++)
	{
		_updatedVertices[i].dehomogenize();
	}
}

void Model::CalculateBackfaces(Camera cam)
{
	for (int i = 0; i < GetPolygonCount(); i++)
	{
		Vertex vertex0 = GetUpdatedVertices()[GetPolygons()[i].GetIndex(0)];
		Vertex vertex1 = GetUpdatedVertices()[GetPolygons()[i].GetIndex(1)];
		Vertex vertex2 = GetUpdatedVertices()[GetPolygons()[i].GetIndex(2)];
		Vector3D vectorA = vertex0 - vertex1;
		Vector3D vectorB = vertex0 - vertex2;
		_normalVector = Vector3D::VectorCrossProduct(vectorB, vectorA);
		Vector3D eyeVector = vertex0 - cam.GetCameraPosition();
		float dotProductResult = Vector3D::DotProduct(eyeVector, _normalVector);
		if (dotProductResult < 0)
		{ 
			_polygons.at(i).SetPolygonCulled(true);
		}
		else
		{
			_polygons.at(i).SetPolygonCulled(false);
		}
		_polygons.at(i).setNormalVector(_normalVector);
	}
}

void Model::Sort(void)
{
	//Sort algorithm, sorts polygons by the averege z from the 3 different vertices
	for (int i = 0; i < GetPolygonCount(); i++)
	{
		Vertex vertex0 = GetUpdatedVertices()[GetPolygons()[i].GetIndex(0)];
		Vertex vertex1 = GetUpdatedVertices()[GetPolygons()[i].GetIndex(1)];
		Vertex vertex2 = GetUpdatedVertices()[GetPolygons()[i].GetIndex(2)];

		float avgZ = ((vertex0.GetZ() + vertex1.GetZ() + vertex2.GetZ()) /3.0f);
		_polygons.at(i).setPolygonZ(avgZ);
	}
	//Sort <algorithm> library method
	sort(_polygons.begin(), _polygons.end(), ascendingSort);
}

//Bool expression used for the sort method, compared z coordinate values
bool Model::ascendingSort(const Polygon3D& lhs, const Polygon3D& rhs)
{
	return (lhs.getPolygonZ() > rhs.getPolygonZ());
}

void Model::CalculateLightingDirectional(vector<DirectionalLighting> directionalLights)
{
	float total_red = 0.0f;
	float total_green = 0.0f;
	float total_blue = 0.0f;

	for (int i = 0; i < GetPolygonCount(); i++)
	{
		//Assigns rgb values from ambient lighting to local rgb values
		COLORREF colours = _polygons.at(i).getColourProperty();
		RGBTRIPLE rgb;
		total_red = rgb.rgbtRed = GetRValue(colours);
		total_green = rgb.rgbtGreen = GetGValue(colours);
		total_blue = rgb.rgbtBlue = GetBValue(colours);

		//For each directional lighting in collection
		for (int j = 0; j < directionalLights.size(); j++)
		{
			float temp_red = (float)directionalLights.at(j).getRedIntensity();
			float temp_green = (float)directionalLights.at(j).getGreenIntensity();
			float temp_blue = (float)directionalLights.at(j).getBlueIntensity();

			temp_red = temp_red * kd_red;
			temp_green = temp_green * kd_green;
			temp_blue = temp_blue * kd_green;

			//calculate dot product of the normalised vector to the light source and the polygons normal vector
			Vector3D normalisedVector = directionalLights.at(j).getLightDirection();
			normalisedVector.normalise();
			
			Vector3D normalVectorNormalise = _polygons.at(i).getNormalVector();
			//Vector3D normalVectorNormalise = normalVertexVector.at(i).GetNormalVector();

			//Calculate dot product, detects if model is within 'view' of the light source, within the lights directional
			normalVectorNormalise.normalise();
			float dotProductResult = Vector3D::DotProduct(normalisedVector, normalVectorNormalise);

			//Dot product, find area seen by the directional lighting, if dot product is more then zero it can 'see' the surface
			if (dotProductResult > 0)
			{
				/* For the polygons that can be seen by the light source, these polygons rgb colours are changed to 
				compensate for the directional light source */
				temp_red = (temp_red * dotProductResult);
				temp_green = (temp_green * dotProductResult);
				temp_blue = (temp_blue * dotProductResult);

				total_red = total_red + temp_red;
				total_green = total_green + temp_green;
				total_blue = total_blue + temp_blue;
			}

			//Clamp values, makes sure they're within range 0-255
			if (total_red > 255)
			{
				total_red = (float)255;
			}

			if (total_green > 255)
			{
				total_green = (float)255;
			}

			if (total_blue > 255)
			{
				total_blue = (float)255;
			}

			//Resets RGB values to include directional lighting
			_polygons.at(i).setColourProperty(RGB((int)total_red, (int)total_green, (int)total_blue));
		}
	}

}

void Model::CalculateLightingAmbient(AmbientLighting ambientLights)
{
	float total_red = 0.0f;
	float total_green = 0.0f;
	float total_blue = 0.0f;

	for (int i = 0; i < GetPolygonCount(); i++)
	{
		//Applys ambient lighting to all polygons that are not backfaces culled
		float temp_red = 0.0f;
		float temp_green = 0.0f;
		float temp_blue = 0.0f;
		
		//Gets ambient light intensities from class values
		temp_red = (float)ambientLights.getRedIntensity();
		temp_green = (float)ambientLights.getGreenIntensity();
		temp_blue = (float)ambientLights.getBlueIntensity();

	    total_red = temp_red * ka_red;
		total_green = temp_green * ka_green;
		total_blue = temp_blue * ka_blue;

		_polygons.at(i).setColourProperty(RGB((int)total_red, (int)total_green, (int)total_blue));
	}
}

void Model::CalculateLightingPoint(vector<PointLighting> pointLights)
{
	float total_red = 0.0f;
	float total_green = 0.0f;
	float total_blue = 0.0f;

	for (int i = 0; i < GetPolygonCount(); i++)
	{
		//Assigns rgb values from ambient lighting to local rgb values
		COLORREF colours = _polygons.at(i).getColourProperty();
		RGBTRIPLE rgb;
		total_red = rgb.rgbtRed = GetRValue(colours);
		total_green = rgb.rgbtGreen = GetGValue(colours);
		total_blue = rgb.rgbtBlue = GetBValue(colours);

		//For each point light
		for (int j = 0; j < pointLights.size(); j++)
		{
			//Collects light intensity from class object
			float temp_red = (float)pointLights.at(j).getRedIntensity();
			float temp_green = (float)pointLights.at(j).getGreenIntensity();
			float temp_blue = (float)pointLights.at(j).getBlueIntensity();

			temp_red = temp_red * ks_red;
		    temp_green = temp_green * ks_green;
			temp_blue = temp_blue * ks_blue;

			//Uses one vector from each polygon to access light direction, will be used to calculate distance
			Vertex vertex0 = GetUpdatedVertices()[GetPolygons()[i].GetIndex(0)];

			//Calculate distance, the length between the vector and the light source
			Vector3D vectorToLight = { (vertex0.GetX() - pointLights.at(j).getLightDirection().GetX()),
									   (vertex0.GetY() - pointLights.at(j).getLightDirection().GetY()),
									   (vertex0.GetZ() - pointLights.at(j).getLightDirection().GetZ())};
			
			//Square root result to find the distance
			float distance = sqrtf((vectorToLight.GetVectorX() * 2) + (vectorToLight.GetVectorY() * 2) + (vectorToLight.GetVectorZ() * 2));
			vectorToLight.normalise();
			//Calculate attenuation using attenuation coefficients
			float attenuation = ((1 / ((pointLights.at(j).getACoefficient() + pointLights.at(j).getBCoefficient()) * (distance + pointLights.at(j).getCCoefficient()) * (distance * 2)))*100);

			Vector3D normalVectorNormalise = _polygons.at(i).getNormalVector();
			normalVectorNormalise.normalise();

			//Dot product, find area seen by the directional lighting, if dot product is more then zero it can 'see' the surface
			float dotProductResult = Vector3D::DotProduct(vectorToLight, normalVectorNormalise);

			if (dotProductResult > 0)
			{
				temp_red = (temp_red * attenuation);
				temp_green = (temp_green * attenuation);
				temp_blue = (temp_blue * attenuation);

				total_red = total_red + temp_red;
				total_green = total_green + temp_green;
				total_blue = total_blue + temp_blue;
			}

			//Clamp values
			if (total_red > 255)
			{
				total_red = (float)255;
			}

			if (total_green > 255)
			{
				total_green = (float)255;
			}

			if (total_blue > 255)
			{
				total_blue = (float)255;
			}

			//Add colours back to each polygon
			_polygons.at(i).setColourProperty(RGB((int)total_red, (int)total_green, (int)total_blue));
		}
	}
}

//Method to generate normal vectors for each vertex
void Model::CalculateNormalVectors()
{
	//for (int i = 0; i < GetVertexCount(); i++)
	//{
	//	normalVertexVector.at(i).SetNormalVector({ 0,0,0 });
	//	normalVertexVector.at(i).SetVertexCount(0);
	//}

	//for (int i = 0; i < GetPolygonCount(); i++)
	//{
	//	//normalVertexVector.at(i).SetNormalVector((normalVertexVector.at(i).GetNormalVector() + _polygons.at(i).getNormalVector()));
	//	//normalVertexVector.at(i).SetVertexCount(normalVertexVector.at(i).GetVertexCount() + 1);

	//	normalVertexVector[GetPolygons()[i].GetIndex(0)].SetNormalVector((normalVertexVector.at(i).GetNormalVector() + _polygons.at(i).getNormalVector()));
	//	normalVertexVector[GetPolygons()[i].GetIndex(0)].SetVertexCount(normalVertexVector.at(i).GetVertexCount()+1);

	//	normalVertexVector[GetPolygons()[i].GetIndex(1)].SetNormalVector((normalVertexVector.at(i).GetNormalVector() + _polygons.at(i).getNormalVector()));
	//	normalVertexVector[GetPolygons()[i].GetIndex(1)].SetVertexCount(normalVertexVector.at(i).GetVertexCount() + 1);

	//	normalVertexVector[GetPolygons()[i].GetIndex(2)].SetNormalVector((normalVertexVector.at(i).GetNormalVector() + _polygons.at(i).getNormalVector()));
	//	normalVertexVector[GetPolygons()[i].GetIndex(2)].SetVertexCount(normalVertexVector.at(i).GetVertexCount() + 1);
	//}

	//for (int i = 0; i < GetVertexCount(); i++)
	//{
	//	//normalVertexVector.at(i).SetNormalVector(Vector3D(normalVertexVector.at(i).GetNormalVector().GetVectorX() / normalVertexVector.at(i).GetVertexCount(), normalVertexVector.at(i).GetNormalVector().GetVectorY() / normalVertexVector.at(i).GetVertexCount(), normalVertexVector.at(i).GetNormalVector().GetVectorZ() / normalVertexVector.at(i).GetVertexCount()));
	//	//normalVertexVector.at(i).GetNormalVector().normalise();

	//	normalVertexVector[GetPolygons()[i].GetIndex(0)].SetNormalVector(Vector3D(normalVertexVector[GetPolygons()[i].GetIndex(0)].GetNormalVector().GetVectorX() / normalVertexVector[GetPolygons()[i].GetIndex(0)].GetVertexCount(), normalVertexVector[GetPolygons()[i].GetIndex(0)].GetNormalVector().GetVectorY() / normalVertexVector[GetPolygons()[i].GetIndex(0)].GetVertexCount(), normalVertexVector[GetPolygons()[i].GetIndex(0)].GetNormalVector().GetVectorZ() / normalVertexVector[GetPolygons()[i].GetIndex(0)].GetVertexCount()));
	//	normalVertexVector[GetPolygons()[i].GetIndex(1)].SetNormalVector(Vector3D(normalVertexVector[GetPolygons()[i].GetIndex(1)].GetNormalVector().GetVectorX() / normalVertexVector[GetPolygons()[i].GetIndex(1)].GetVertexCount(), normalVertexVector[GetPolygons()[i].GetIndex(1)].GetNormalVector().GetVectorY() / normalVertexVector[GetPolygons()[i].GetIndex(1)].GetVertexCount(), normalVertexVector[GetPolygons()[i].GetIndex(1)].GetNormalVector().GetVectorZ() / normalVertexVector[GetPolygons()[i].GetIndex(1)].GetVertexCount()));
	//	normalVertexVector[GetPolygons()[i].GetIndex(2)].SetNormalVector(Vector3D(normalVertexVector[GetPolygons()[i].GetIndex(2)].GetNormalVector().GetVectorX() / normalVertexVector[GetPolygons()[i].GetIndex(2)].GetVertexCount(), normalVertexVector[GetPolygons()[i].GetIndex(2)].GetNormalVector().GetVectorY() / normalVertexVector[GetPolygons()[i].GetIndex(2)].GetVertexCount(), normalVertexVector[GetPolygons()[i].GetIndex(2)].GetNormalVector().GetVectorZ() / normalVertexVector[GetPolygons()[i].GetIndex(2)].GetVertexCount()));

	//	normalVertexVector[GetPolygons()[i].GetIndex(0)].GetNormalVector().normalise();
	//	normalVertexVector[GetPolygons()[i].GetIndex(1)].GetNormalVector().normalise();
	//	normalVertexVector[GetPolygons()[i].GetIndex(2)].GetNormalVector().normalise();
	//}

	for (Vertex& vertex : _normalVertexVector)
	{
		vertex.SetNormalVector(Vector3D(0, 0, 0));
		vertex.SetVertexCount(0);
	}

	for (Polygon3D& polygon : _polygons)
	{
		for (int i = 0; i < 3; i++)
		{
			Vector3D normalVertexVector = _normalVertexVector.at(polygon.GetIndex(i)).GetNormalVector();
			normalVertexVector = normalVertexVector + polygon.getNormalVector();
			_normalVertexVector.at(polygon.GetIndex(i)).SetNormalVector(normalVertexVector);
			_normalVertexVector.at(polygon.GetIndex(i)).SetVertexCount(_normalVertexVector.at(polygon.GetIndex(i)).GetVertexCount() + 1);
		}
	}

	for (Vertex& vertex : _normalVertexVector)
	{
		Vector3D tempVector = vertex.GetNormalVector();
		tempVector.SetVectorX(tempVector.GetVectorX() / vertex.GetVertexCount());
		tempVector.SetVectorY(tempVector.GetVectorY() / vertex.GetVertexCount());
		tempVector.SetVectorZ(tempVector.GetVectorZ() / vertex.GetVertexCount());

		tempVector.normalise();
		vertex.SetNormalVector(tempVector);
	}
}
