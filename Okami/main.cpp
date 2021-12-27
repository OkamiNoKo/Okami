#include <windows.h>
#include <d2d1.h>
#pragma comment(lib,"d2d1.lib")

#include <iostream>
#include <fstream>
#include <string>



LRESULT _stdcall WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam);

HWND winInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "class";

	RegisterClass(&wc);
	HWND hWnd = CreateWindow(
		"class",
		"Direct2D init",
		WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720,
		NULL, NULL, hInstance, NULL);
	return hWnd;
}

struct Player
{
	int x;
	int y;
	int width;
	int hight;
};

struct Box
{
	int width;
	int hight;
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = winInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	Player P;
	P.x = 0; P.y = 240; P.width = 128; P.hight = 72;


	ID2D1Factory* factory = NULL;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

	ID2D1HwndRenderTarget* rt = NULL;
	factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hWnd, D2D1::SizeU(1280, 720))
		, &rt);

	ID2D1SolidColorBrush ** brushes = new ID2D1SolidColorBrush*[32];

	for (int i = 0; i < 27; i++)
	{
		rt->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF((float)((i / 9) % 3) / 3.5, (float)((i / 3) % 3) / 3.5, (float)(i % 3) / 3.5)),
			&brushes[i]);
	}
	rt->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0.8f ,0.8f ,0.8f )),
		&brushes[27]);


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;

	Box box;

	int **Level = NULL;
	std::string line;
	std::ifstream in("materials/w1l1.txt"); // �������� ���� ��� ������
	int c = 0;
	if (in.is_open())
	{
		while (getline(in, line))
		{
			if (c < 2)
			{
				int S = 0;
				for (int i = 0; i < line.length(); i++)
				{
					S = S * 10 + (int)line[i] - (int)'0';
				}
				if (c == 0)
				{
					box.width = S;
				}
				else
				{
					box.hight = S;
					Level = new int*[box.hight];
					for (int i = 0; i < box.hight; i++)
					{
						Level[i] = new int[box.width];
					}
				}
			}
			else
			{
				int S = 0;
				for (int i = 0; i < line.length(); i++)
				{
					if (line[i] == ',')
					{
						Level[(c - 2) / box.width][(c - 2) % box.width] = S;
						S = 0;
						c++;
					}
					else
					{
						S += (int)line[i] - (int)'0';
					}
				}
			}
			c++;
		}
	}
	in.close();

	bool flag = true;
	while (flag)
	{   

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		rt->BeginDraw();
		rt->Clear(D2D1::ColorF(D2D1::ColorF::White));
		
		for (int i = 0; i < 720; i++)
		{
			for (int j = 0; j < 1280; j++)
			{
				Level[i][j] = 0;
				
				rt->DrawRectangle(
					D2D1::RectF(j, i, j + 1, i + 1),
					brushes[Level[i][j]], 100.0f, NULL);
			}
		}
		rt->EndDraw();
	}
	if (brushes[0] != NULL)
		brushes[0]->Release();
	if (rt != NULL)
		rt->Release();
	if (factory != NULL)
		factory->Release();

	return 0;
}

LRESULT _stdcall WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}