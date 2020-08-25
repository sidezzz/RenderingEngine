#pragma once
#include <list>

#include "camera.h"
#include "mesh_instance.h"

class Scene
{
public:

private:
	std::list<MeshInstance> instances_;
	std::list<std::shared_ptr<Mesh>> meshes_;
	Camera camera_;
};