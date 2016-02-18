#include "DxApp.h"

int WINAPI WinMain(HINSTANCE hi, HINSTANCE phi, LPSTR lp, int nsc)
{
	DxApp* app = new DxApp();

	if(app->init("Sample1 Window", 800, 600))
		app->start();

	delete app;

	return 0;
}