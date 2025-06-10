#pragma once

#include <chrono>

namespace globals
{
	extern HMODULE mainModule;
	extern HWND mainWindow;
	extern int uninjectKey;
	extern int openMenuKey;
	extern RECT wRect;
	extern bool canDraw;
	extern bool showScreenshotNotice;
	extern std::chrono::steady_clock::time_point lastScreenshotTime;
};

