#pragma once
#include <vector>

#include "camera.h"
#include "instance.h"

class Scene
{
public:

private:
	std::vector<Instance> instances_;
	Camera camera_;
};