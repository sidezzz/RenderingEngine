
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
void MeshInstance::SetWireframe(bool wireframe)
{
	wireframe_ = wireframe;
}
