
#include "mesh_instance.h"

MeshInstance::MeshInstance(std::shared_ptr<Mesh> mesh)
{
	mesh_ = mesh;
}

std::shared_ptr<Mesh> MeshInstance::GetMesh() const
{
	return mesh_;
}
bool MeshInstance::IsWireframe() const
{
	return wireframe_;
}
ColorFloat MeshInstance::GetColorMultiplier() const
{
	return color_multiplier_;
}
void MeshInstance::SetWireframe(bool wireframe)
{
	wireframe_ = wireframe;
}
void MeshInstance::SetColorMultiplier(const ColorFloat& color)
{
	color_multiplier_ = color;
}
