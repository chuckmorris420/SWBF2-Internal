#pragma once

// Credits: RangeMachine - https://www.unknowncheats.me/forum/members/1058523.html
namespace Renderer
{
	extern ImFont* m_pFont;
	extern void Initialize();

	extern void BeginScene();
	extern void DrawScene();
	extern void EndScene();

	extern float RenderText(const std::string& text, const ImVec2& position, float size, uint32_t color, bool center = false);
	extern void RenderLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness = 1.0f);
	extern void RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness = 1.0f, uint32_t segments = 16);
	extern void RenderCircleFilled(const ImVec2& position, float radius, uint32_t color, uint32_t segments = 16);
	extern void RenderRect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All, float thickness = 1.0f);
	extern void RenderRectFilled(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All);
	extern void RenderImage(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, uint32_t color);
	extern void RenderImageRounded(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, uint32_t color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All);
	void DrawLine(float x1, float y1, float x2, float y2, float thicc, float r, float g, float b);
}

// Declare the ImColorToU32 function
inline uint32_t ImColorToU32(const ImColor& color)
{
    int r = int(color.Value.x * 255.0f);
    int g = int(color.Value.y * 255.0f);
    int b = int(color.Value.z * 255.0f);
    int a = int(color.Value.w * 255.0f);
    return (a << 24) | (r << 16) | (g << 8) | b;
}


