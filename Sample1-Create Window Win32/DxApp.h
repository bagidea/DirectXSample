#ifndef DX_APP_H
#define DX_APP_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

class DxApp
{
public:
	DxApp();
	~DxApp();

	bool init(char* Title, int screenWidth, int screenHeight);
	void start();
private:
	char* _appName;
	HINSTANCE _instance;
	HWND _hwnd;
};

#endif