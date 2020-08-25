#pragma once
#include <memory>

#include "mesh.h"

class MeshInstance
{
public:

private:
	Transform transform_;
	std::shared_ptr<Mesh> mesh_;
};