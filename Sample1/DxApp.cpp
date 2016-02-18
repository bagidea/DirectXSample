#include "DxApp.h"

#include <iostream>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch(msg)
	{
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
		}break;
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			DestroyWindow(hwnd);
		}break;
	default:
		{
			return DefWindowProc(hwnd, msg, wp, lp);
		}
	}
}

DxApp::DxApp()
{
}

DxApp::~DxApp()
{
	UnregisterClass(_appName, _instance);
	_instance = NULL;
}

bool DxApp::init(char* Title, int screenWidth, int screenHeight)
{
	WNDCLASSEX wc;

	_appName = Title;
	_instance = GetModuleHandle(NULL);

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _instance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _appName;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Register Class Ex Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;

	int _screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int _screenHeight = GetSystemMetrics(SM_CYSCREEN);

	_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		Title,
		Title,
		nStyle,
		//CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight,
		(_screenWidth/2)-(screenWidth/2), (_screenHeight/2)-(screenHeight/2), screenWidth, screenHeight,
		NULL, NULL, _instance, NULL
	);

	if(_hwnd == NULL)
	{
		MessageBox(NULL, "Create Window Ex Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	ShowWindow(_hwnd, SW_SHOW);
	SetForegroundWindow(_hwnd);
	SetFocus(_hwnd);

	return true;
}

void DxApp::start()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			//Update
		}
	}
}