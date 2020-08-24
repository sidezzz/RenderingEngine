#pragma once
#include <vector>
#include <memory>

#include "primitives.h"

class Mesh
{
public:

private:
	std::vector<Vertex> vertices_;
	std::vector<int> indices_;
};

