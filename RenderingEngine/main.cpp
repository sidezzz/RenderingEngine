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



class CameraController
{
public:
	CameraController(Camera& camera)
		: camera_(camera) {}
	void HandleMouseMove(const Vector2& delta)
	{
		auto cam_rot = camera_.GetRotation();
		auto fov_scale = camera_.GetFov() / 75.f;
		cam_rot.yaw += fov_scale * delta.x * mouse_sensetivity_.x;
		cam_rot.pitch -= fov_scale * delta.y * mouse_sensetivity_.y;
		camera_.SetRotation(cam_rot.Clamp());
	}
	void HandleMouseWheel(float delta)
	{
		camera_.SetFov(camera_.GetFov() * (1.f + 0.05f * delta));
	}
	void HandleKeyDown(int key_code)
	{
		for (auto& key : key_map_)
		{
			if (key.key_code == key_code)
			{
				if (!key.active)
				{
					movement_vector_ += key.movement;
					key.active = true;
				}
				break;
			}
		}
	}
	void HandleKeyUp(int key_code)
	{
		for (auto& key : key_map_)
		{
			if (key.key_code == key_code)
			{
				if (key.active)
				{
					movement_vector_ -= key.movement;
					key.active = false;
				}
				break;
			}
		}
	}
	void Tick(float time_delta)
	{
		camera_.SetPosition(camera_.GetPosition() + camera_.GetRotation().RotateVector(movement_vector_ * movement_scale_) * time_delta);
	}

private:
	struct Key
	{
		int key_code;
		Vector3 movement;
		bool active;
	};

	Camera& camera_;
	Vector2 mouse_sensetivity_ = { 0.1f, 0.1f };
	Vector3 movement_vector_;
	float movement_scale_ = 40.f;
	Key key_map_[6] =
	{
		{0x57, { 1.f, 0, 0}, false},
		{0x53, {-1.f, 0, 0}, false},
		{0x44, {0, 1.f, 0}, false},
		{0x41, {0, -1.f, 0}, false},
		{VK_SPACE, {0, 0, 1.f}, false},
		{VK_CONTROL, {0, 0, -1.f}, false}
	};
};

Scene SCENE;
CameraController CONTROLLER(SCENE.GetCamera());
Dx11Renderer RENDERER;
bool MENU_ACTIVE = true;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

	if (msg == WM_KEYDOWN && wParam == VK_ESCAPE)
	{
		MENU_ACTIVE = !MENU_ACTIVE;
		return 0;
	}

	if (!MENU_ACTIVE)
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

			CONTROLLER.HandleMouseMove({ static_cast<float>(x_diff), static_cast<float>(y_diff) });

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
		case WM_KEYDOWN:
			CONTROLLER.HandleKeyDown(wParam);
			return 0;
		case WM_KEYUP:
			CONTROLLER.HandleKeyUp(wParam);
			return 0;
		case WM_MOUSEWHEEL:
			CONTROLLER.HandleMouseWheel(-(float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
			return 0;
		}
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

int main()
{
	auto loader = SCENE.SetMeshLoader<ObjSceneMeshLoader>();

	loader->LoadMeshAsync("D:\\Downloads\\jzb865er6v-IronMan\\IronMan\\IronMan.obj", [](auto mesh)
	{
		MeshInstance ironman(mesh);
		ironman.SetPosition({ 0, 50, 10 });
		ironman.SetRotation({ 0, 30, 0 });

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

		CONTROLLER.Tick(elapsed.count());
		SCENE.Tick();
		RENDERER.RenderScene(&SCENE);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		if (MENU_ACTIVE)
		{
			if (ImGui::Begin("Menu"))
			{
				ImGui::Text("FOV: %f", SCENE.GetCamera().GetFov());
				auto rotation = SCENE.GetCamera().GetRotation();
				ImGui::Text("Rotation: %f %f", rotation.yaw, rotation.pitch);

				if (ImGui::CollapsingHeader("Instances"))
				{
					int i = 0;
					for (auto& instance : SCENE.GetInstances())
					{
						if (ImGui::TreeNode(&instance, "%d", i))
						{
							auto wireframe = instance.IsWireframe();
							ImGui::Checkbox("Wireframe", &wireframe);
							instance.SetWireframe(wireframe);
							if (ImGui::TreeNode("Translation"))
							{
								auto position = instance.GetPosition();
								ImGui::SliderFloat("X", &position.x, -1000, 1000);
								ImGui::SliderFloat("Y", &position.y, -1000, 1000);
								ImGui::SliderFloat("Z", &position.z, -1000, 1000);
								instance.SetPosition(position);
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Rotation"))
							{
								auto rad_rotator = DegreeToRad(instance.GetRotation());
								ImGui::SliderAngle("Yaw", &rad_rotator.yaw, 0, 360);
								ImGui::SliderAngle("Pitch", &rad_rotator.pitch, 0, 360);
								ImGui::SliderAngle("Roll", &rad_rotator.roll, 0, 360);
								instance.SetRotation(RadToDegree(rad_rotator));
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Scale"))
							{
								auto scale = instance.GetScale();
								static bool lock = false;
								ImGui::Checkbox("Lock axis", &lock);
								if (lock)
								{
									ImGui::SliderFloat("Axis", &scale.x, 0, 200);
									scale.y = scale.x;
									scale.z = scale.x;
								}
								else
								{
									ImGui::SliderFloat("X", &scale.x, 0, 200);
									ImGui::SliderFloat("Y", &scale.y, 0, 200);
									ImGui::SliderFloat("Z", &scale.z, 0, 200);
								}
								instance.SetScale(scale);
								ImGui::TreePop();
							}
							ImGui::TreePop();
						}
						++i;
					}
				}
			}
			ImGui::End();
		}

		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		RENDERER.Present(true);
		last_frame_time = current_frame_time;
	}

	return 0;
}