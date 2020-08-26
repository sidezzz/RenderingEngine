#pragma once
#include "scene.h"

class Renderer
{
public:
	virtual ~Renderer() = default;
	virtual void RenderScene(Scene* scene) = 0;
	virtual bool ResizeViewport(const Vector2& size) = 0;
};