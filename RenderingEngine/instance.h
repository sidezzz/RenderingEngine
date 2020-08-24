#pragma once
#include <memory>

#include "mesh.h"

class Instance
{
public:

private:
	Transform transform_;
	std::shared_ptr<Mesh> mesh_;
};