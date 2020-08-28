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
	ColorFloat GetColorMultiplier() const;
	void SetWireframe(bool wireframe);
	void SetColorMultiplier(const ColorFloat& color);

private:
	std::shared_ptr<Mesh> mesh_;
	ColorFloat color_multiplier_ = ColorFloat(1.f, 1.f, 1.f, 1.f);
	bool wireframe_ = false;
};