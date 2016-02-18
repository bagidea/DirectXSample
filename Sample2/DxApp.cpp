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
	_device = nullptr;
	_context = nullptr;
	_swap = nullptr;
	_renderer = nullptr;
}

DxApp::~DxApp()
{
	if(_context)
		_context->ClearState();

	Memory::safeRelease(_renderer);
	Memory::safeRelease(_swap);
	Memory::safeRelease(_context);
	Memory::safeRelease(_device);

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

	if(!initDx(screenWidth, screenHeight))
		return false;

	return true;
}

bool  DxApp::initDx(int screenWidth, int screenHeight)
{
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driver[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT numDriverType = ARRAYSIZE(driver);

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};

	UINT numFeatureLevel = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferCount = 1;
	sd.BufferDesc.Width = screenWidth;
	sd.BufferDesc.Height = screenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hwnd;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Windowed = true;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT result;

	for(int i = 0; i < numDriverType; i++)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, driver[i], NULL, createDeviceFlags, featureLevels, numFeatureLevel, D3D11_SDK_VERSION, &sd, &_swap, &_device, &_level, &_context);

		if(SUCCEEDED(result))
		{
			_type = driver[i];
			break;
		}
	}

	if(FAILED(result))
	{
		OutputDebugString("Failed to create device and swap chain");
		return false;
	}

	ID3D11Texture2D* backBufferTex;
	_swap->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTex));
	_device->CreateRenderTargetView(backBufferTex, nullptr, &_renderer);

	_context->OMSetRenderTargets(1, &_renderer, nullptr);

	_viewport.Width = static_cast<float>(screenWidth);
	_viewport.Height = static_cast<float>(screenHeight);
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;

	_context->RSSetViewports(1, &_viewport);

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
			_context->ClearRenderTargetView(_renderer, DirectX::Colors::CornflowerBlue);
			_swap->Present(0, 0);
		}
	}
}