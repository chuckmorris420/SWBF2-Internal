#pragma once
#include <windows.h>
#include <cstdio>
#include <iostream>
#include <sstream>      // std::stringstream
#include <algorithm>
#include <DirectXMath.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "classes.h"
#include <nlohmann/json.hpp>
#include <fstream>

#include "hooks.h"
#include "D3dx9math.h"
#include "d3d11/d3d11_impl.h"
#include <d2d1.h>
#include "input.h"
#include "renderer.h"
#include <string>
#include <vector>
#include <filesystem>
#include <array>

#include "settings.h"
#include "utils.h"
#include "Renderer.h"
#include "globals.h"

#include "menu/menu.h"


typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;
