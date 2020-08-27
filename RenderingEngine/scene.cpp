#include "scene.h"

const std::list<MeshInstance>& Scene::GetInstances() const
{
	return instances_;
}
const Camera& Scene::GetCamera() const
{
	return camera_;
}
Camera& Scene::GetCamera()
{
	return camera_;
}