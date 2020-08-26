#include <iostream>
#include <Windows.h>

#include "primitives.h"
#include "dx11/dx11_renderer.h"

void PrintMatrix(const Matrix4x4& m)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			printf("%.3f ", m.m[i][j]);
		}
		printf("\n");
	}
}

Scene SCENE;
Dx11Renderer RENDERER;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			auto size = Vector2{ (float)LOWORD(lParam), (float)HIWORD(lParam) };
			RENDERER.ResizeViewport(size);
			SCENE.camera_.aspect_ratio_ = size.x / size.y;
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

int main()
{
	Transform t;
	//t.translation.x = 10.f;
	//t.translation.y = -2;
	//t.rotation.yaw = 0.f;
	//t.scale = Vector3(0.25, 0.25, 0.25);
	t.rotation.roll = 90.f;
	//t.rotation.pitch = 90.f;
	PrintMatrix(t.ToMatrix());

	auto pyramide_mesh = std::make_shared<Mesh>();
	pyramide_mesh->vertices_ = 
	{ 
		Vertex{{-1,1,0}, {}, {}, 0xFFFFFF00}, Vertex{{0,0,3}, {}, {}, 0xFF00FF00}, Vertex{{-1,-1,0}, {}, {}, 0xFF00FFFF},
		Vertex{{1,-1,0}, {}, {}, 0xFF0000FF}, Vertex{{1,1,0}, {}, {}, 0xFFFF0000},
		Vertex{{-1,1,0}, {}, {}, 0xFF000000}, Vertex{{-1,-1,0}, {}, {}, 0xFF000000}, Vertex{{1,-1,0}, {}, {}, 0xFF000000}, Vertex{{1,1,0}, {}, {}, 0xFF000000}
	};
	pyramide_mesh->indices_ = { 0, 1, 2, 2, 1, 3, 3, 1, 4, 4, 1, 0, 5, 6, 7, 7, 8, 5 };

	MeshInstance instance;
	instance.mesh_ = pyramide_mesh;
	instance.transform_.translation.x = 10.f;

	MeshInstance instance2;
	instance2.mesh_ = pyramide_mesh;
	instance2.transform_.translation.x = -10.f;

	MeshInstance instance3;
	instance3.mesh_ = pyramide_mesh;
	instance3.transform_.translation.y = 10.f;

	MeshInstance instance4;
	instance4.mesh_ = pyramide_mesh;
	instance4.transform_.translation.y = -10.f;

	MeshInstance instance5;
	instance5.mesh_ = pyramide_mesh;
	instance5.transform_.translation.z = 10.f;

	MeshInstance instance6;
	instance6.mesh_ = pyramide_mesh;
	instance6.transform_.translation.z = -10.f;

	SCENE.meshes_.push_back(pyramide_mesh);
	SCENE.instances_.push_back(instance);
	//SCENE.instances_.push_back(instance2);
	//SCENE.instances_.push_back(instance3);
	//SCENE.instances_.push_back(instance4);
	//SCENE.instances_.push_back(instance5);
	//SCENE.instances_.push_back(instance6);

	//SCENE.camera_.transform_.scale.z = -1.f;


	WNDCLASSEXA wc = { sizeof(WNDCLASSEXA), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "RenderingEngine", NULL };
	RegisterClassExA(&wc);
	HWND hwnd = CreateWindowA(wc.lpszClassName, "Rendering Engine", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

	if (!RENDERER.Initialize(hwnd))
	{
		UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessageA(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
			continue;
		}
		for (auto&& i : SCENE.instances_)
		{
			//i.transform_.rotation.pitch += 1.f;
			//i.transform_.rotation.roll += 1.f;
			i.transform_.rotation.yaw += 1.f;
			//i.transform_.translation.x -= 0.002;
		}
		//SCENE.camera_.transform_.rotation.roll += 0.1;
		//SCENE.camera_.transform_.rotation.yaw += 0.1;

		RENDERER.RenderScene(&SCENE);
	}

	return 0;
}