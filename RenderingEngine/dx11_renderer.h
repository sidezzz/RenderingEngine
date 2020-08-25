#pragma once
#include "renderer.h"
#include "mesh.h"



class Dx11Renderer : public Renderer
{
public:
	void RenderScene(Scene* scene) override;
};