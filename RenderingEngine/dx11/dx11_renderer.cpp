
#include "dx11_renderer.h"
#include "shaders/vertex_shader.h"
#include "shaders/pixel_shader.h"

struct alignas(16) Dx11ConstantBuffer
{
	Matrix4x4 model_transform;
	Matrix4x4 view_transform;
	Matrix4x4 projection_transform;
	Matrix4x4 view_projection_transform;
	Matrix4x4 model_view_projection_transform;
	alignas(16) Vector3 camera_position;
	alignas(16) ColorFloat model_color_multiplier;
};

Microsoft::WRL::ComPtr<ID3D11Device> Dx11Renderer::GetDevice() const
{
	return device_;
}
bool Dx11Renderer::Initialize(HWND hwnd)
{
	{
		DXGI_SWAP_CHAIN_DESC desc = {};
		desc.BufferCount = 2;
		desc.BufferDesc.Width = 0;
		desc.BufferDesc.Height = 0;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.RefreshRate.Numerator = 165;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow = hwnd;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		uint32_t flags = 0;

#ifndef _NDEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _NDEBUG

		const D3D_FEATURE_LEVEL feature_levels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		last_error_ = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, feature_levels, ARRAYSIZE(feature_levels),
			D3D11_SDK_VERSION, &desc, swapchain_.GetAddressOf(), device_.GetAddressOf(), nullptr, context_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}
	}
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.ScissorEnable = false;
		desc.DepthClipEnable = true;
		
		last_error_ = device_->CreateRasterizerState(&desc, solid_rasterizer_state_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}

		desc.DepthBias = -100;
		desc.DepthBiasClamp = -0.0000001f;
		desc.SlopeScaledDepthBias = 0.f;// -0.001f;
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_NONE;
		last_error_ = device_->CreateRasterizerState(&desc, wireframe_rasterizer_state_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}
	}
	{
		D3D11_BLEND_DESC desc = {};
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		last_error_ = device_->CreateBlendState(&desc, blend_state_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}
	}
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS;
		desc.StencilEnable = false;
		last_error_ = device_->CreateDepthStencilState(&desc, depth_stencil_state_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}
	}
	{
		last_error_ = device_->CreatePixelShader(G_pixel_shader, sizeof(G_pixel_shader), nullptr, pixel_shader_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}

		last_error_ = device_->CreateVertexShader(G_vertex_shader, sizeof(G_vertex_shader), nullptr, vertex_shader_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC desc[] =
		{
			{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(Vertex, uv),       D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, ambient),    D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",      1, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, diffuse),    D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",      2, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, specular),    D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PSIZE",      0, DXGI_FORMAT_R32_FLOAT,       0, offsetof(Vertex, shininess),    D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		last_error_ = device_->CreateInputLayout(desc, ARRAYSIZE(desc), G_vertex_shader, sizeof(G_vertex_shader), input_layout_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}
	}
	{
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = sizeof(Dx11ConstantBuffer);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		last_error_ = device_->CreateBuffer(&desc, nullptr, constant_buffer_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}
	}

	return CreateViewportBuffers();
}
bool Dx11Renderer::CreateViewportBuffers()
{
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buffer;
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = std::max(1.f, viewport_size_.x);
		desc.Height = std::max(1.f, viewport_size_.y);
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		last_error_ = device_->CreateTexture2D(&desc, nullptr, depth_stencil_buffer.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}

		last_error_ = device_->CreateDepthStencilView(depth_stencil_buffer.Get(), nullptr, depth_stencil_view_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}
	}

	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
		last_error_ = swapchain_->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
		if (FAILED(last_error_))
		{
			return false;
		}

		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		last_error_ = device_->CreateRenderTargetView(back_buffer.Get(), &desc, render_target_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return false;
		}
	}
	return true;
}
bool Dx11Renderer::ResizeViewport(const Vector2& size)
{
	viewport_size_ = size;
	render_target_.Reset();
	depth_stencil_view_.Reset();
	last_error_ = swapchain_->ResizeBuffers(0, size.x, size.y, DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(last_error_))
	{
		return false;
	}

	return CreateViewportBuffers();
}
void Dx11Renderer::RenderScene(Scene* scene)
{
	SetupRenderState();
	context_->ClearRenderTargetView(render_target_.Get(), scene->GetBackgroundColor().raw);

	Dx11ConstantBuffer gpu_constant_buffer;
	gpu_constant_buffer.view_transform = scene->GetCamera().GetTransform().ToMatrix().Inverse();
	gpu_constant_buffer.projection_transform = scene->GetCamera().GetPerspectiveProjection();
	gpu_constant_buffer.view_projection_transform = gpu_constant_buffer.projection_transform * gpu_constant_buffer.view_transform;
	gpu_constant_buffer.camera_position = scene->GetCamera().GetPosition();

	Mesh* last_setup_mesh = nullptr;
	ID3D11RasterizerState* last_rasterizer_state_ = nullptr;
	for (auto&& instance : scene->GetInstances())
	{
		auto mesh = instance.GetMesh();
		auto renderer_data = static_cast<Dx11MeshRendererData*>(mesh->GetMeshRendererData());
		if (!renderer_data)
		{
			if (auto new_data = CreateMeshRendererData(mesh.get()))
			{
				mesh->SetMeshRendererData(std::unique_ptr<MeshRendererData>(new_data.release()));
				renderer_data = static_cast<Dx11MeshRendererData*>(mesh->GetMeshRendererData());
			}
			else
			{
				continue;
			}
		}

		if (mesh.get() != last_setup_mesh)
		{
			unsigned int stride = sizeof(Vertex);
			unsigned int offset = 0;
			context_->IASetVertexBuffers(0, 1, renderer_data->vertex_buffer_.GetAddressOf(), &stride, &offset);
			context_->IASetIndexBuffer(renderer_data->index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
			last_setup_mesh = mesh.get();
		}

		if (instance.IsWireframe() && last_rasterizer_state_ != wireframe_rasterizer_state_.Get())
		{
			context_->RSSetState(wireframe_rasterizer_state_.Get());
			last_rasterizer_state_ = wireframe_rasterizer_state_.Get();
		}
		else if (!instance.IsWireframe() && last_rasterizer_state_ != solid_rasterizer_state_.Get())
		{
			context_->RSSetState(solid_rasterizer_state_.Get());
			last_rasterizer_state_ = solid_rasterizer_state_.Get();
		}

		gpu_constant_buffer.model_transform = instance.GetTransform().ToMatrix();
		gpu_constant_buffer.model_view_projection_transform = gpu_constant_buffer.view_projection_transform * gpu_constant_buffer.model_transform;
		gpu_constant_buffer.model_color_multiplier = instance.GetColorMultiplier();

		D3D11_MAPPED_SUBRESOURCE constant_buffer_resource;
		last_error_ = context_->Map(constant_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &constant_buffer_resource);
		if (FAILED(last_error_))
		{
			continue;
		}
		*static_cast<Dx11ConstantBuffer*>(constant_buffer_resource.pData) = gpu_constant_buffer;
		context_->Unmap(constant_buffer_.Get(), 0);

		context_->DrawIndexed(mesh->GetIndices().size(), 0, 0);
	}
}
void Dx11Renderer::Present(bool vsync)
{
	last_error_ = swapchain_->Present(vsync, 0);
}

void Dx11Renderer::SetupRenderState()
{
	context_->OMSetRenderTargets(1, render_target_.GetAddressOf(), depth_stencil_view_.Get());
	context_->IASetInputLayout(input_layout_.Get());
	context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context_->VSSetShader(vertex_shader_.Get(), nullptr, 0);
	context_->VSSetConstantBuffers(0, 1, constant_buffer_.GetAddressOf());
	context_->PSSetConstantBuffers(0, 1, constant_buffer_.GetAddressOf());
	context_->PSSetShader(pixel_shader_.Get(), nullptr, 0);
	context_->OMSetDepthStencilState(depth_stencil_state_.Get(), 0);
	context_->OMSetBlendState(blend_state_.Get(), nullptr, 0xFFFFFFFF);

	D3D11_VIEWPORT vp = {};
	vp.Width = viewport_size_.x;
	vp.Height = viewport_size_.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = vp.TopLeftY = 0;
	context_->RSSetViewports(1, &vp);
	context_->ClearDepthStencilView(depth_stencil_view_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
std::unique_ptr<Dx11MeshRendererData> Dx11Renderer::CreateMeshRendererData(const Mesh* mesh)
{
	auto renderer_data = std::make_unique<Dx11MeshRendererData>();
	{
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = mesh->GetVertices().size() * sizeof(Vertex);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = mesh->GetVertices().data();
		last_error_ = device_->CreateBuffer(&desc, &data, renderer_data->vertex_buffer_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return nullptr;
		}
	}
	{
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = mesh->GetIndices().size() * sizeof(Index);
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = mesh->GetIndices().data();
		last_error_ = device_->CreateBuffer(&desc, &data, renderer_data->index_buffer_.GetAddressOf());
		if (FAILED(last_error_))
		{
			return nullptr;
		}
	}
	return renderer_data;
}