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
			RENDERER.ResizeViewport(Vector2{ (float)LOWORD(lParam), (float)HIWORD(lParam) });
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

		RENDERER.RenderScene(&SCENE);
	}

	return 0;
}