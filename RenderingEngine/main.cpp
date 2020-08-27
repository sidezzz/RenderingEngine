#include <iostream>
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
			SCENE.camera_.aspect_ratio_ = size.x / size.y;
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_MOUSEMOVE:
		//return 0;
		static auto last_x_pos = GET_X_LPARAM(lParam);
		static auto last_y_pos = GET_Y_LPARAM(lParam);
		auto x_pos = GET_X_LPARAM(lParam);
		auto y_pos = GET_Y_LPARAM(lParam);

		auto x_diff = x_pos - last_x_pos;
		auto y_diff = y_pos - last_y_pos;

		SCENE.camera_.transform_.rotation.yaw += x_diff * 0.1;
		SCENE.camera_.transform_.rotation.pitch += y_diff * 0.1f;
		SCENE.camera_.transform_.rotation.Clamp();

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

	auto mesh = std::make_shared<Mesh>();
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

			mesh->vertices_.emplace_back(std::move(vertex));
			mesh->indices_.push_back(mesh->indices_.size());
			++i;
		}
	}

	return mesh;
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
		Vertex{{-1,-1,0}, Vector3(1,1,1), {}, 0xFF00FFFF}, Vertex{{0,0,3}, Vector3(1,1,1), {}, 0xFF00FF00}, Vertex{{-1,1,0}, Vector3(1,1,1), {}, 0xFFFFFFFF},
		Vertex{{1,1,0}, Vector3(1,1,1), {}, 0xFF0000FF}, Vertex{{1,-1,0}, Vector3(1,1,1), {}, 0xFFFF00FF},
		Vertex{{-1,-1,0}, Vector3(1,1,1), {}, 0xFF0000FF}, Vertex{{-1,1,0}, Vector3(1,1,1), {}, 0xFF0000FF},
		Vertex{{1,1,0}, Vector3(1,1,1), {}, 0xFF0000FF}, Vertex{{1,-1,0}, Vector3(1,1,1), {}, 0xFF0000FF}
	};
	pyramide_mesh->indices_ = { 0, 1, 2, 2, 1, 3, 3, 1, 4, 4, 1, 0, 5, 6, 7, 7, 8, 5 };


	MeshInstance pyramide;
	pyramide.mesh_ = pyramide_mesh;
	pyramide.transform_.translation.x = 0.8f;

	MeshInstance car;
	car.mesh_ = BuildTestMesh("D:\\Downloads\\6e48z1kc7r40-bugatti\\bugatti\\bugatti.obj");
	car.transform_.translation.y = -10.f;
	car.transform_.scale = Vector3(10, 10, 10);

	MeshInstance weapon;
	weapon.mesh_ = BuildTestMesh("D:\\Downloads\\88yrcjq4775s-M4A4\\m4a1.obj");
	weapon.transform_.scale = Vector3(0.3, 0.3, 0.3);

	MeshInstance ironman;
	ironman.mesh_ = BuildTestMesh("D:\\Downloads\\jzb865er6v-IronMan\\IronMan\\IronMan.obj");
	ironman.transform_.scale = Vector3(0.1, 0.1, 0.1);
	ironman.transform_.translation.x = -10;

	MeshInstance drone;
	drone.mesh_ = BuildTestMesh("D:\\Downloads\\99-intergalactic_spaceship-obj\\Intergalactic_Spaceship-(Wavefront).obj");
	drone.transform_.scale = Vector3(1, 1, 1);
	drone.transform_.translation.x = 15;

	MeshInstance rungholt;
	rungholt.mesh_ = BuildTestMesh("D:\\Downloads\\rungholt\\rungholt.obj");
	rungholt.transform_.scale = Vector3(1, 1, 1);

	SCENE.instances_.push_back(drone);
	SCENE.instances_.push_back(car);
	SCENE.instances_.push_back(weapon);
	SCENE.instances_.push_back(ironman);
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
			//i.transform_.rotation.yaw += 0.1f;
			//i.transform_.translation.x -= 0.01;
		}
		//SCENE.camera_.transform_.rotation.roll += 0.1;
		//SCENE.camera_.transform_.rotation.yaw += 0.1;
		//SCENE.camera_.transform_.rotation.pitch += 0.1;
		//printf("%f\n", SCENE.camera_.transform_.rotation.Forward().x);
		//auto fwd = SCENE.camera_.transform_.rotation.Forward();
		//printf("%f %f %f\n", fwd.x, fwd.y, fwd.z);
		auto speed = 0.1f;
		if (GetAsyncKeyState(0x57) & 0x8000)
		{
			SCENE.camera_.transform_.translation += SCENE.camera_.transform_.rotation.RotateVector({ 1,0,0 }) * speed;
		}
		if (GetAsyncKeyState(0x53) & 0x8000)
		{
			SCENE.camera_.transform_.translation += SCENE.camera_.transform_.rotation.RotateVector({ -1,0,0 }) * speed;
		}
		if (GetAsyncKeyState(0x41) & 0x8000)
		{
			SCENE.camera_.transform_.translation += SCENE.camera_.transform_.rotation.RotateVector({ 0,-1,0 }) * speed;
		}
		if (GetAsyncKeyState(0x44) & 0x8000)
		{
			SCENE.camera_.transform_.translation += SCENE.camera_.transform_.rotation.RotateVector({ 0,1,0 }) * speed;
		}
		if (GetAsyncKeyState(0x51) & 0x8000)
		{
			SCENE.camera_.transform_.rotation.roll += speed;
		}
		if (GetAsyncKeyState(0x45) & 0x8000)
		{
			SCENE.camera_.transform_.rotation.roll -= speed;
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			SCENE.camera_.transform_.translation += SCENE.camera_.transform_.rotation.RotateVector({ 0,0,1 }) * speed;
		}
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			SCENE.camera_.transform_.translation += SCENE.camera_.transform_.rotation.RotateVector({ 0,0,-1 }) * speed;
		}
		RENDERER.RenderScene(&SCENE);
	}

	return 0;
}