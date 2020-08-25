#pragma once
#include "scene.h"

class Renderer
{
public:
	virtual void RenderScene(Scene* scene) = 0;
};