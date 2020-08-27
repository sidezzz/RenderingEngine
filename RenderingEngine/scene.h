#pragma once
#include <list>

#include "camera.h"
#include "mesh_instance.h"

class Scene
{
public:
	const std::list<MeshInstance>& GetInstances() const;
	const Camera& GetCamera() const;
	Camera& GetCamera();

//private:
	std::list<MeshInstance> instances_;
private:
	Camera camera_;
};