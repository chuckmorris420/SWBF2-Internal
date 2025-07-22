#include "Includes.h"

namespace menu {

	bool isOpen = true;
	float test = 0.f;

	void init() {
		static bool no_titlebar = false;
		static bool no_border = false;
		static bool no_resize = false;
		static bool auto_resize = true;
		static bool no_move = false;
		static bool no_scrollbar = false;
		static bool no_collapse = false;
		static bool no_menu = true;
		static bool start_pos_set = false;
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 5.3f;
		style.FrameRounding = 2.3f;
		style.ScrollbarRounding = 0;

		style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.09f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.10f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.02f, 0.01f, 0.01f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
		//Accent colors
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.7f, 0.07f, 0.07f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7f, 0.07f, 0.07f, 1.00f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.5f, 0.02f, 0.02f, 1.00f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.8f, 0.4f, 0.4f, 0.8f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.7f, 0.07f, 0.07f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.6f, 0.17f, 0.17f, 1.00f);

		ImGuiWindowFlags	window_flags = 0;
		if (no_titlebar)	window_flags |= ImGuiWindowFlags_NoTitleBar;
		if (no_resize)		window_flags |= ImGuiWindowFlags_NoResize;
		if (auto_resize)	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		if (no_move)		window_flags |= ImGuiWindowFlags_NoMove;
		if (no_scrollbar)	window_flags |= ImGuiWindowFlags_NoScrollbar;
		if (no_collapse)	window_flags |= ImGuiWindowFlags_NoCollapse;
		if (!no_menu)		window_flags |= ImGuiWindowFlags_MenuBar;
		if (!start_pos_set) { ImGui::SetNextWindowPos(ImVec2(25, 25)); start_pos_set = true; }

		ImGui::GetIO().MouseDrawCursor = isOpen;
		

		if (isOpen)
		{
			ImGui::Begin("Master Keef's SWBF2 ESP", &isOpen, window_flags);
			ImGui::Text("BACKSPACE to open/close menu.");
			
			if (ImGui::BeginTabBar("Tabs"))
			{
				if (ImGui::BeginTabItem("Visuals"))
				{
					ImGui::Checkbox("Enable ESP", &Config::ESP::enabled);
					ImGui::Checkbox("Enemy Box", &Config::ESP::enemy);
					ImGui::Checkbox("Enemy Name", &Config::ESP::name);
					ImGui::Checkbox("Enemy Health", &Config::ESP::health);
					//ImGui::Checkbox("Show Distance", &Config::ESP::distance);
					ImGui::Checkbox("Enemy Dot", &Config::ESP::dot);
					ImGui::Checkbox("Flag Hero/Villian", &Config::ESP::heroCheck);
					ImGui::Checkbox("Flag Ariel/Enforcer/Infiltrator", &Config::ESP::extraUnitCheck);
					ImGui::TreePop();
				}
				if (ImGui::BeginTabItem("Config"))
				{
					ImGui::Text("Config options will be here soon.");
					ImGui::TreePop();
				}
			}
			
			ImGui::End();
		}
	}
}