
#include "dx11_renderer.h"
#include "shaders/vertex_shader.h"
#include "shaders/pixel_shader.h"

struct Dx11ConstantBuffer
{
	Matrix4x4 model_transform;
	Matrix4x4 view_transform;
	Matrix4x4 projection_transform;
};

bool Dx11Renderer::Initialize(HWND hwnd)
{
	{
		DXGI_SWAP_CHAIN_DESC desc = {};
		desc.BufferCount = 2;
		desc.BufferDesc.Width = 0;
		desc.BufferDesc.Height = 0;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow = hwnd;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

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
		desc.CullMode = D3D11_CULL_NONE;
		desc.ScissorEnable = false;
		desc.DepthClipEnable = true;
		last_error_ = device_->CreateRasterizerState(&desc, rasterizer_state_.GetAddressOf());
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
			{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			//{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(Vertex, uv),       D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, offsetof(Vertex, color),    D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
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
	Dx11ConstantBuffer cpu_constant_buffer;
	auto camera_transform = scene->GetCamera().GetTransform();
	camera_transform.rotation.pitch += 90.f;
	camera_transform.rotation.yaw += 90.f;
	camera_transform.rotation.Clamp();
	cpu_constant_buffer.view_transform = camera_transform.ToMatrix().Inverse();
	auto& projection = cpu_constant_buffer.projection_transform;

	auto far_dist = 1000.f;
	auto near_dist = 0.1f;//(viewport_size_.x * 0.5f) / std::tan(0.5f * DegreeToRad(scene->GetCamera().GetFov()));

	auto range_inv = 1.f / (near_dist - far_dist);
	projection.m[1][1] = 1.f / std::tan(0.5f * DegreeToRad(scene->GetCamera().GetFov()));
	projection.m[0][0] = projection.m[1][1] / scene->GetCamera().GetAspectRatio();
	projection.m[2][2] = (near_dist + far_dist) * range_inv;
	projection.m[3][2] = (near_dist * far_dist) * range_inv;
	projection.m[2][3] = -1.f;
	projection.m[3][3] = 0.f;
	/*projection.m[1][1] = 1.f / std::tan(0.5f * DegreeToRad(scene->GetCamera().GetFov()));
	projection.m[0][0] = projection.m[1][1] / scene->GetCamera().GetAspectRatio();
	projection.m[2][2] = (far_dist) / (far_dist - near_dist);
	projection.m[3][2] = -(near_dist * far_dist) / (far_dist - near_dist);
	projection.m[2][3] = 1.f;
	projection.m[3][3] = 0.f;*/

	/*projection.m[1][1] = 1.f / std::tan(0.5f * DegreeToRad(scene->GetCamera().GetFov()));
	projection.m[0][0] = projection.m[1][1] / scene->GetCamera().GetAspectRatio();
	projection.m[2][2] = (far_dist) / (near_dist - far_dist);
	projection.m[3][2] = (far_dist * near_dist) / (near_dist - far_dist);
	projection.m[2][3] = -1.f;
	projection.m[3][3] = 0.f;*/

	//auto far_dist = std::numeric_limits<float>::infinity();
	//auto near_dist = (viewport_size_.x * 0.5f) / std::tan(0.5f * DegreeToRad(scene->GetCamera().GetFov()));
	/*auto frustrum_depth = far_dist - near_dist;
	auto one_over_depth = 1.f / frustrum_depth;

	projection.m[1][1] = 1.f / std::tan(0.5f * DegreeToRad(scene->GetCamera().GetFov()));
	projection.m[0][0] = 1.f * projection.m[1][1] / scene->GetCamera().GetAspectRatio();
	projection.m[2][2] = far_dist * one_over_depth;
	projection.m[3][2] = (-far_dist * near_dist) * one_over_depth;
	projection.m[2][3] = 1.f;
	projection.m[3][3] = 0.f;*/

	Mesh* last_setup_mesh = nullptr;
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

		cpu_constant_buffer.model_transform = instance.GetTransform().ToMatrix();

		D3D11_MAPPED_SUBRESOURCE constant_buffer_resource;
		last_error_ = context_->Map(constant_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &constant_buffer_resource);
		if (FAILED(last_error_))
		{
			continue;
		}
		auto gpu_constant_buffer = static_cast<Dx11ConstantBuffer*>(constant_buffer_resource.pData);
		*gpu_constant_buffer = cpu_constant_buffer;
		context_->Unmap(constant_buffer_.Get(), 0);

		context_->DrawIndexed(mesh->GetIndices().size(), 0, 0);
	}

	last_error_ = swapchain_->Present(1, 0);
}

void Dx11Renderer::SetupRenderState()
{
	context_->OMSetRenderTargets(1, render_target_.GetAddressOf(), depth_stencil_view_.Get());
	context_->IASetInputLayout(input_layout_.Get());
	context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context_->VSSetShader(vertex_shader_.Get(), nullptr, 0);
	context_->VSSetConstantBuffers(0, 1, constant_buffer_.GetAddressOf());
	context_->PSSetShader(pixel_shader_.Get(), nullptr, 0);

	context_->OMSetDepthStencilState(depth_stencil_state_.Get(), 0);
	D3D11_VIEWPORT vp = {};
	vp.Width = viewport_size_.x;
	vp.Height = viewport_size_.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = vp.TopLeftY = 0;
	context_->RSSetViewports(1, &vp);
	context_->RSSetState(rasterizer_state_.Get());
	ColorFloat background;
	background.r = 0.3f;
	background.g = 0.3f;
	background.b = 0.3f;
	background.a = 1.f;
	context_->ClearRenderTargetView(render_target_.Get(), background.raw);
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