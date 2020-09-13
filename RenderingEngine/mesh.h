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

	bool operator==(const Vertex& rhs) const
	{
		return memcmp(this, &rhs, sizeof(Vertex)) == 0;
	}
};

namespace std
{
	template<>
	struct hash<Vertex>
	{
		size_t operator()(const Vertex& value) const
		{
			size_t result = 0;
			auto begin = reinterpret_cast<const float*>(&value);
			auto end = begin + sizeof(Vertex) / sizeof(float);
			for (auto itr = begin; itr != end; ++itr)
			{
				result ^= hash<float>()(*itr);
			}
			return result;
		}
	};
}

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

