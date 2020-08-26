#pragma once
#include <vector>
#include <memory>

#include "primitives.h"

class MeshRendererData
{
public:
	virtual ~MeshRendererData() = default;
};

//struct HomogenousVector3 : Vector3
//{
//	float w = 1.f;
//};

struct Vertex
{
	Vector3 position;
	Vector3 normal; // currently unused
	Vector2 uv; // currently unused
	ColorInt color;
};

using Index = int;

class Mesh
{
public:
	MeshRendererData* GetMeshRendererData() const;
	void SetMeshRendererData(std::unique_ptr<MeshRendererData> data);
	const std::vector<Vertex>& GetVertices() const;
	const std::vector<Index>& GetIndices() const;

//private:
	std::vector<Vertex> vertices_;
	std::vector<Index> indices_;
	std::unique_ptr<MeshRendererData> renderer_data_;
};

