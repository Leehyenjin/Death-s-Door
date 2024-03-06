#pragma once

#include <Process.h>

#include "GameInstance.h"

namespace Client
{
	static const unsigned int g_iWinSizeX = 1024;
	static const unsigned int g_iWinSizeY = 600;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
extern bool g_bFullScreen;
extern bool g_bNeedResizeSwapChain;

using namespace Client;




