#include "includes.h"
#include <chrono>

namespace globals {
	HMODULE mainModule;
	HWND mainWindow;
	int uninjectKey = VK_F12;
	int openMenuKey = VK_BACK;
	RECT wRect;
	bool canDraw = true;
	bool showScreenshotNotice = true;
	std::chrono::steady_clock::time_point lastScreenshotTime = std::chrono::steady_clock::now();
}
