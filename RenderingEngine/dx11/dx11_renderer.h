#pragma once
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#include <wrl.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "../renderer.h"
#include "../mesh.h"

class Dx11MeshRendererData : public MeshRendererData
{
public:

private:
	friend class Dx11Renderer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer_;
	Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer_;
};

class Dx11Renderer : public Renderer
{
public:
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const;
	bool Initialize(HWND hwnd);
	void RenderScene(Scene* scene) override;
	void Present(bool vsync);
	bool ResizeViewport(const Vector2& size) override;

private:
	void SetupRenderState();
	std::unique_ptr<Dx11MeshRendererData> CreateMeshRendererData(const Mesh* mesh);
	bool CreateViewportBuffers();

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain_;
	Microsoft::WRL::ComPtr<ID3D11Device> device_;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> solid_rasterizer_state_;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> wireframe_rasterizer_state_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view_;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state_;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout_;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader_;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader_;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer_;

	Vector2 viewport_size_;
	uint32_t last_error_ = 0;
};