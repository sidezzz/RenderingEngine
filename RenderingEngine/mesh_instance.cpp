
#include "mesh_instance.h"

std::shared_ptr<Mesh> MeshInstance::GetMesh() const
{
	return mesh_;
}
Transform MeshInstance::GetTransform() const
{
	return transform_;
}
