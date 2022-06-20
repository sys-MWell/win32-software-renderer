#pragma once
#include <iostream>
#include <vector>
#include "Vertex.h"
#include "Camera.h"
#include "Matrix.h"
#include "Polygon3D.h"
#include "DirectionalLighting.h"
#include "AmbientLighting.h"
#include "PointLighting.h"
#include "Model.h"
#include "Vector3D.h"
#include <algorithm>

using namespace std;

class Model
{
public:
	Model();
	~Model();

	//Accessors
	const vector<Polygon3D>& GetPolygons();
	const vector<Vertex>& GetVertices();
	const vector<Vertex>& GetUpdatedVertices();
	size_t GetPolygonCount() const;
	size_t GetVertexCount() const;
	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2);
	void ApplyTransformToLocalVertices(const Matrix& transform);
	void ApplyTransformToTransformedVertices(const Matrix& transform);
	void ApplyDehomogenizeToTransformedVertices();
	void CalculateBackfaces(Camera cam);
	void Sort(void);
	static bool ascendingSort(const Polygon3D& lhs, const Polygon3D& rhs);

	void CalculateLightingDirectional(vector<DirectionalLighting> directionalLights);

	void CalculateLightingAmbient(AmbientLighting ambientLights);

	void CalculateLightingPoint(vector<PointLighting> pointLights);

	void CalculateNormalVectors();

private:
	Vector3D _normalVector;
	vector<Vertex> _normalVertexVector;

	vector<Polygon3D> _polygons;
	vector<Vertex> _vertices;
	vector<Vertex> _updatedVertices;

	//Directional lighting
	float kd_red;
	float kd_green;
	float kd_blue;

	//Ambient lighting
	float ka_red;
	float ka_green;
	float ka_blue;

	//Spot lightening
	float ks_red;
	float ks_green;
	float ks_blue;
};