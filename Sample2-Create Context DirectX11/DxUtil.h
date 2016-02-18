#pragma once

#include <d3d11.h>
#include <DirectXColors.h>

namespace Memory
{
	template <class T> void safeDelete(T& t)
	{
		if(t)
		{
			delete t;
			t = nullptr;
		}
	}

	template <class T> void safeDeleteArr(T& t)
	{
		if(t)
		{
			delete[] t;
			t = nullptr;
		}
	}

	template <class T> void safeRelease(T& t)
	{
		if(t)
		{
			t->Release();
			t =nullptr;
		}
	}
}