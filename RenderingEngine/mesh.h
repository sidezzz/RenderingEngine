#pragma once
#include <vector>
#include <memory>

#include "primitives.h"

class RendererData
{
public:

};

class Mesh
{
public:

private:
	std::vector<Vertex> vertices_;
	std::vector<int> indices_;
	std::unique_ptr<RendererData> renderer_data_;
};

