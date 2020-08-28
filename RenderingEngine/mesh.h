#pragma once
#include <vector>
#include <memory>

#include "primitives.h"

class MeshRendererData
{
public:
	virtual ~MeshRendererData() = default;
};

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv; // currently unused

	// per-vertex material
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	float shininess;
};

using Index = int;

class Mesh
{
public:
	Mesh(std::vector<Vertex>&& vertices, std::vector<Index>&& indices);

	MeshRendererData* GetMeshRendererData() const;
	void SetMeshRendererData(std::unique_ptr<MeshRendererData> data);
	const std::vector<Vertex>& GetVertices() const;
	const std::vector<Index>& GetIndices() const;

private:
	std::vector<Vertex> vertices_;
	std::vector<Index> indices_;
	std::unique_ptr<MeshRendererData> renderer_data_;
};

