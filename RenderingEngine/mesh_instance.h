#pragma once
#include <memory>

#include "scene_object.h"
#include "mesh.h"

class MeshInstance : public SceneObject
{
public:
	MeshInstance(std::shared_ptr<Mesh> mesh);

	std::shared_ptr<Mesh> GetMesh() const;
	bool IsWireframe() const;
	void SetWireframe(bool wireframe);

private:
	std::shared_ptr<Mesh> mesh_;
	bool wireframe_ = false;
};