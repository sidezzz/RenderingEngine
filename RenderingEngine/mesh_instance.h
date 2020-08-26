#pragma once
#include <memory>

#include "mesh.h"

class MeshInstance
{
public:
	std::shared_ptr<Mesh> GetMesh() const;
	Transform GetTransform() const;

private:
	Transform transform_;
	std::shared_ptr<Mesh> mesh_;
};