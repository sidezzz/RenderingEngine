#include <iostream>
#include <chrono>
#include <functional>
#include <future>
#include <Windows.h>
#include <windowsx.h>

#include "primitives.h"
#include "dx11/dx11_renderer.h"
#include "3rd_party/tiny_obj_loader.h"
#include "3rd_party/imgui.h"
#include "3rd_party/imgui_impl_win32.h"
#include "3rd_party/imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Scene SCENE;
Dx11Renderer RENDERER;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	switch (msg)
	{
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			auto size = Vector2{ (float)LOWORD(lParam), (float)HIWORD(lParam) };
			RENDERER.ResizeViewport(size);
			SCENE.GetCamera().SetAspectRatio(size.x / size.y);
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	};

	bool imgui_mouse = false;
	bool imgui_keyboard = false;
	if (ImGui::GetCurrentContext())
	{
		auto& io = ImGui::GetIO();
		imgui_mouse = io.WantCaptureMouse;
		imgui_keyboard = io.WantCaptureKeyboard;
	}

	if (!imgui_mouse)
	{
		switch (msg)
		{
		case WM_MOUSEMOVE:
		{
			static auto last_x_pos = GET_X_LPARAM(lParam);
			static auto last_y_pos = GET_Y_LPARAM(lParam);
			auto x_pos = GET_X_LPARAM(lParam);
			auto y_pos = GET_Y_LPARAM(lParam);

			auto x_diff = x_pos - last_x_pos;
			auto y_diff = y_pos - last_y_pos;

			auto cam_rot = SCENE.GetCamera().GetRotation();
			cam_rot.yaw += x_diff * 0.1;
			cam_rot.pitch += y_diff * 0.1f;
			SCENE.GetCamera().SetRotation(cam_rot.Clamp());

			last_x_pos = x_pos;
			last_y_pos = y_pos;
			if (GetActiveWindow() == hWnd)
			{
				RECT rect;
				GetClientRect(hWnd, &rect);

				POINT ul;
				ul.x = rect.left;
				ul.y = rect.top;

				POINT lr;
				lr.x = rect.right;
				lr.y = rect.bottom;

				MapWindowPoints(hWnd, nullptr, &ul, 1);
				MapWindowPoints(hWnd, nullptr, &lr, 1);

				SetCursorPos(ul.x + (lr.x - ul.x) / 2, ul.y + (lr.y - ul.y) / 2);

				last_x_pos = (lr.x - ul.x) / 2;
				last_y_pos = (lr.y - ul.y) / 2;
			}
			return 0;
		}
		case WM_MOUSEWHEEL:
			SCENE.GetCamera().SetFov(SCENE.GetCamera().GetFov() - (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
			return 0;
		}
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

int main()
{
	/*auto pyramide_mesh = std::make_shared<Mesh>();
	pyramide_mesh->vertices_ = 
	{ 
		Vertex{{-1,-1,0}, Vector3(1,1,1), {}, 0xFF00FFFF}, Vertex{{0,0,3}, Vector3(1,1,1), {}, 0xFF00FF00}, Vertex{{-1,1,0}, Vector3(1,1,1), {}, 0xFFFFFFFF},
		Vertex{{1,1,0}, Vector3(1,1,1), {}, 0xFF0000FF}, Vertex{{1,-1,0}, Vector3(1,1,1), {}, 0xFFFF00FF},
		Vertex{{-1,-1,0}, Vector3(1,1,1), {}, 0xFF0000FF}, Vertex{{-1,1,0}, Vector3(1,1,1), {}, 0xFF0000FF},
		Vertex{{1,1,0}, Vector3(1,1,1), {}, 0xFF0000FF}, Vertex{{1,-1,0}, Vector3(1,1,1), {}, 0xFF0000FF}
	};
	pyramide_mesh->indices_ = { 0, 1, 2, 2, 1, 3, 3, 1, 4, 4, 1, 0, 5, 6, 7, 7, 8, 5 };


	MeshInstance pyramide;
	pyramide.mesh_ = pyramide_mesh;
	pyramide.transform_.translation.x = 0.8f;*/

	auto loader = SCENE.SetMeshLoader<ObjSceneMeshLoader>();

	loader->LoadMeshAsync("D:\\Downloads\\jzb865er6v-IronMan\\IronMan\\IronMan.obj", [](auto mesh)
	{
		MeshInstance ironman(mesh);
		ironman.SetPosition({ 0, 50, 10 });

		MeshInstance wire_ironman = ironman;
		wire_ironman.SetPosition(wire_ironman.GetPosition() + Vector3(80, 0, 0));
		wire_ironman.SetRotation({ 180, 0, 0 });
		wire_ironman.SetWireframe(true);

		return std::vector{ ironman, wire_ironman };
	});

	loader->LoadMeshAsync("D:\\Downloads\\rungholt\\rungholt.obj", [](auto mesh)
	{
		MeshInstance city(mesh);
		//MeshInstance wire_city = city;
		//wire_city.SetWireframe(true);
		//wire_city.SetColorMultiplier({ 0.f, 0.f, 0.f, 1.f });

		return std::vector{ city };// , wire_city };
	});

	loader->LoadMeshAsync("D:\\Downloads\\6e48z1kc7r40-bugatti\\bugatti\\bugatti.obj", [](auto mesh)
	{
		MeshInstance car(mesh);
		car.SetScale({ 10, 10, 10 });
		car.SetPosition({ 0, 0, 4 });

		return std::vector{ car };
	});

	loader->LoadMeshAsync("D:\\Downloads\\BeachBabeWidowmaker.obj", [](auto mesh)
	{
		MeshInstance widow(mesh);
		widow.SetPosition({ 0, 180, 10 });
		widow.SetScale({ 100, 100, 100 });
		widow.SetColorMultiplier({ 1.f,1.f,1.f, 0.5f });

		return std::vector{ widow };
	});

	loader->LoadMeshAsync("D:\\Downloads\\Viper_Lewd_Rigged_PUBLIC_1.1.obj", [](auto mesh)
	{
		MeshInstance viper(mesh);
		viper.SetPosition({ 140, 180, 10 });
		viper.SetRotation({ 180,0,0 });
		viper.SetScale({ 100, 100, 100 });

		/*MeshInstance wire_viper = viper;
		wire_viper.SetWireframe(true);
		wire_viper.SetColorMultiplier({ 0.f, 0.f, 0.f, 1.f });*/

		return std::vector{ viper };// , wire_viper };
	});

	loader->LoadMeshAsync("D:\\Downloads\\88yrcjq4775s-M4A4\\m4a1.obj", [](auto mesh)
	{
		MeshInstance weapon(mesh);
		weapon.SetPosition({ -40, -67, 185 });
		weapon.SetScale({ 3, 3, 3 });

		MeshInstance weapon_polygon_highlight = weapon;
		weapon_polygon_highlight.SetWireframe(true);
		weapon_polygon_highlight.SetColorMultiplier({ 0.f, 0.f, 0.f, 1.f });

		return std::vector{ weapon, weapon_polygon_highlight };
	});


	WNDCLASSEXA wc = { sizeof(WNDCLASSEXA), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "RenderingEngine", NULL };
	RegisterClassExA(&wc);
	HWND hwnd = CreateWindowA(wc.lpszClassName, "Rendering Engine", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

	ImGui::CreateContext();
	if (!ImGui_ImplWin32_Init(hwnd) || !RENDERER.Initialize(hwnd) || !ImGui_ImplDX11_Init(RENDERER.GetDevice().Get()))
	{
		UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	std::chrono::high_resolution_clock clock;
	auto last_frame_time = clock.now();

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

		auto current_frame_time = clock.now();

		auto elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(current_frame_time - last_frame_time);

		auto speed = 40.f * static_cast<float>(elapsed.count());
		auto& camera = SCENE.GetCamera();
		if (GetAsyncKeyState(0x57) & 0x8000)
		{
			camera.SetPosition(camera.GetPosition() + camera.GetRotation().RotateVector({ 1,0,0 }) * speed);
		}
		if (GetAsyncKeyState(0x53) & 0x8000)
		{
			camera.SetPosition(camera.GetPosition() + camera.GetRotation().RotateVector({ -1,0,0 }) * speed);
		}
		if (GetAsyncKeyState(0x41) & 0x8000)
		{
			camera.SetPosition(camera.GetPosition() + camera.GetRotation().RotateVector({ 0,-1,0 }) * speed);
		}
		if (GetAsyncKeyState(0x44) & 0x8000)
		{
			camera.SetPosition(camera.GetPosition() + camera.GetRotation().RotateVector({ 0,1,0 }) * speed);
		}
		if (GetAsyncKeyState(0x51) & 0x8000)
		{
			camera.SetRotation(camera.GetRotation() + Rotator(0, 0, speed));
		}
		if (GetAsyncKeyState(0x45) & 0x8000)
		{
			camera.SetRotation(camera.GetRotation() + Rotator(0, 0, -speed));
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			camera.SetPosition(camera.GetPosition() + camera.GetRotation().RotateVector({ 0,0,1 }) * speed);
		}
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			camera.SetPosition(camera.GetPosition() + camera.GetRotation().RotateVector({ 0,0,-1 }) * speed);
		}

		SCENE.Tick();
		RENDERER.RenderScene(&SCENE);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (ImGui::Begin("sasd"))
		{
			
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		RENDERER.Present(true);
		last_frame_time = current_frame_time;
	}

	return 0;
}