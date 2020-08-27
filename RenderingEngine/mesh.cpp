
#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<Index>&& indices)
{
	vertices_ = std::move(vertices);
	indices_ = std::move(indices);
}

MeshRendererData* Mesh::GetMeshRendererData() const
{
	return renderer_data_.get();
}
void Mesh::SetMeshRendererData(std::unique_ptr<MeshRendererData> data)
{
	renderer_data_ = std::move(data);
}
const std::vector<Vertex>& Mesh::GetVertices() const
{
	return vertices_;
}
const std::vector<Index>& Mesh::GetIndices() const
{
	return indices_;
}