#include <iostream>
#include <chrono>
#include <Windows.h>
#include <windowsx.h>

#include "primitives.h"
#include "dx11/dx11_renderer.h"
#include "3rd_party/tiny_obj_loader.h"

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
			SCENE.GetCamera().SetAspectRatio(size.x / size.y);
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_MOUSEMOVE:
	{
		//return 0;
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
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

std::shared_ptr<Mesh> BuildTestMesh(std::string file_name)
{
	std::string inputfile = file_name;// "IronMan.obj";//"Intergalactic_Spaceship-(Wavefront).obj";
	std::string material_dir;
	auto dir_end = inputfile.rfind('\\');
	if (dir_end != std::string::npos)
	{
		material_dir = std::string(inputfile.begin(), inputfile.begin() + dir_end);
	}
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), material_dir.empty() ? nullptr : material_dir.c_str(), true);

	if (!warn.empty()) {
		std::cout << "WARN " << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << "ERR " << err << std::endl;
	}

	std::vector<Vertex> converted_vertices;
	std::vector<Index> converted_indices;
	for (auto& shape : shapes)
	{
		int i = 0;
		for (auto& idx : shape.mesh.indices)
		{
			Vertex vertex;
			// axis are inverted due to different coordinate system
			vertex.position.y = attrib.vertices[3 * idx.vertex_index + 0];
			vertex.position.z = attrib.vertices[3 * idx.vertex_index + 1];
			vertex.position.x = attrib.vertices[3 * idx.vertex_index + 2];

			vertex.normal.y = attrib.normals[3 * idx.normal_index + 0];
			vertex.normal.z = attrib.normals[3 * idx.normal_index + 1];
			vertex.normal.x = attrib.normals[3 * idx.normal_index + 2];

			ColorFloat color(1.f, 1.f, 1.f, 1.f);
			auto face_id = i / 3;
			auto material_id = shape.mesh.material_ids[face_id];
			if (material_id < materials.size())
			{
				color.r = materials[material_id].diffuse[0];
				color.g = materials[material_id].diffuse[1];
				color.b = materials[material_id].diffuse[2];
			}
			vertex.color = ColorInt(color);
			/*ColorFloat color;
			color.r = attrib.colors[3 * idx.vertex_index + 0];
			color.g = attrib.colors[3 * idx.vertex_index + 1];
			color.b = attrib.colors[3 * idx.vertex_index + 2];
			color.a = 1.f;
			vertex.color = ColorInt(color);*/

			converted_vertices.emplace_back(std::move(vertex));
			converted_indices.push_back(converted_indices.size());
			++i;
		}
	}

	return std::make_shared<Mesh>(std::move(converted_vertices), std::move(converted_indices));
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

	MeshInstance car(BuildTestMesh("D:\\Downloads\\6e48z1kc7r40-bugatti\\bugatti\\bugatti.obj"));
	car.SetScale({ 10, 10, 10 });
	car.SetPosition({ 0, 0, 4 });

	MeshInstance weapon(BuildTestMesh("D:\\Downloads\\88yrcjq4775s-M4A4\\m4a1.obj"));
	weapon.SetPosition({ -40, -67, 185 });
	weapon.SetScale({ 3, 3, 3 });

	MeshInstance ironman(BuildTestMesh("D:\\Downloads\\jzb865er6v-IronMan\\IronMan\\IronMan.obj"));
	ironman.SetPosition({ 0, 50, 10 });

	MeshInstance wire_ironman = ironman;
	wire_ironman.SetPosition(wire_ironman.GetPosition() + Vector3(80, 0, 0));
	wire_ironman.SetRotation({ 180, 0, 0 });
	wire_ironman.SetWireframe(true);

	MeshInstance drone(BuildTestMesh("D:\\Downloads\\99-intergalactic_spaceship-obj\\Intergalactic_Spaceship-(Wavefront).obj"));

	MeshInstance rungholt(BuildTestMesh("D:\\Downloads\\rungholt\\rungholt.obj"));

	SCENE.instances_.push_back(drone);
	SCENE.instances_.push_back(car);
	SCENE.instances_.push_back(weapon);
	SCENE.instances_.push_back(ironman);
	SCENE.instances_.push_back(wire_ironman);
	SCENE.instances_.push_back(rungholt);


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

		auto speed = 10.f * static_cast<float>(elapsed.count());
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

		RENDERER.RenderScene(&SCENE);
		last_frame_time = current_frame_time;
	}

	return 0;
}