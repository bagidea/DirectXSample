#ifndef DX_APP_H
#define DX_APP_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <d3d11.h>

#include "DxUtil.h"

class DxApp
{
public:
	DxApp();
	~DxApp();

	bool init(char* Title, int screenWidth, int screenHeight);
	void start();
private:
	bool initDx(int screenWidth, int screenHeight);

	char* _appName;
	HINSTANCE _instance;
	HWND _hwnd;

	ID3D11Device* _device;
	ID3D11DeviceContext* _context;
	IDXGISwapChain* _swap;
	ID3D11RenderTargetView* _renderer;
	D3D_DRIVER_TYPE _type;
	D3D_FEATURE_LEVEL _level;
	D3D11_VIEWPORT _viewport;
};

#endif