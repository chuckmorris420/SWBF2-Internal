#include "includes.h"
#include "utils/globals.h"
#include <chrono>

namespace Renderer
{
	ImFont* m_pFont = NULL;

	RenderView* pRenderer;
	bool isPlayerVisible = false;

	uint32_t yellowColor = 0xffffff00;
	uint32_t orangeColor = 0xffff6a00;
	uint32_t redColor = 0xffff1f1f;
	uint32_t blueColor = 0xff7ea6ff;
	uint32_t greenColor = 0xff0c9d00;
	uint32_t blackColor = 0xff000000;

	void Initialize()
	{
		std::cout << "Init renderer" << std::endl;
		// Shitty stuff
		ImGuiIO& io = ImGui::GetIO();

		// TODO ? custom font
		io.Fonts->AddFontDefault();
		m_pFont = io.Fonts->AddFontDefault();
	}

	void BeginScene()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Begin("##Backbuffer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
	}

	void DrawScene()
	{
		// Color example: 0xfff54266

		if (!IsValidPtr(pRenderView)) return ;

		if (!IsValidPtr(pLocalPlayer)) return ;

		if (!IsValidPtr(pNameClass)) return;

		for (auto pPlayer : pClientArray->clients)
		{
			if (!IsValidPtr(pPlayer)) continue;

			if (pPlayer == pLocalPlayer) continue;

			ClientSoldier* pLocalSoldier = pLocalPlayer->controlledControllable;
			if (!IsValidPtr(pLocalSoldier)) continue;

			ClientSoldier* pSoldier = pPlayer->controlledControllable;
			if (!IsValidPtr(pSoldier)) continue;

			if (!IsValidPtr(pSoldier->clientSolderHealthComponent)) continue;
			if (pSoldier->clientSolderHealthComponent->health <= 0) continue;

			ClientSoldierPrediction* LocalSoldierPrediction = pLocalSoldier->clientSoldierPrediction;
			if (!IsValidPtr(LocalSoldierPrediction)) continue;

			ClientSoldierPrediction* PlayerSoldierPrediction = pSoldier->clientSoldierPrediction;
			if (!IsValidPtr(PlayerSoldierPrediction)) continue;

			uint32_t* Chosencolor;
			uint32_t occludedColor = ImColorToU32(settings::ESP::enemyOccludedColor);
			uint32_t visibleColor = ImColorToU32(settings::ESP::enemyVisibleColor);
			uint32_t heroOccludedColor = ImColorToU32(settings::ESP::heroOccludedColor);
			uint32_t heroVisibleColor = ImColorToU32(settings::ESP::heroVisibleColor);
			uint32_t extraUnitOccludedColor = ImColorToU32(settings::ESP::extraUnitOccludedColor);
			uint32_t extraUnitVisibleColor = ImColorToU32(settings::ESP::extraUnitVisibleColor);

			if (pPlayer->team == pLocalPlayer->team) { Chosencolor = &blueColor; }
			else
			{
				if (pSoldier->clientSolderHealthComponent->m_flMaxHealth > 549 && settings::ESP::heroCheck)
				{
					pSoldier->occluded ? Chosencolor = &heroOccludedColor : Chosencolor = &heroVisibleColor;
				}
				else if (pSoldier->clientSolderHealthComponent->m_flMaxHealth > 200 && settings::ESP::extraUnitCheck)
				{
					pSoldier->occluded ? Chosencolor = &extraUnitOccludedColor : Chosencolor = &extraUnitVisibleColor;
				}
				else
				{
					pSoldier->occluded ? Chosencolor = &occludedColor : Chosencolor = &visibleColor;
				}
			}

			DirectX::XMFLOAT3 head = PlayerSoldierPrediction->pos;
			head.y += pSoldier->heightoffset + .25;
			DirectX::XMFLOAT3 foot = PlayerSoldierPrediction->pos;

			if (utils::WorldToScreen(foot) && utils::WorldToScreen(head))
			{

				float heightoffset = utils::DistanceVec3(foot, head);
				if (settings::ESP::enabled) {
					if (pPlayer->team != pLocalPlayer->team && settings::ESP::enemy)
					{
						float factor = (heightoffset / 5);

						DirectX::XMFLOAT3 m2 = DirectX::XMFLOAT3(head.x - factor, head.y, 0);
						DirectX::XMFLOAT3 m1 = DirectX::XMFLOAT3(head.x + factor, head.y, 0);
						DirectX::XMFLOAT3 m3 = DirectX::XMFLOAT3(foot.x - factor, foot.y, 0);
						DirectX::XMFLOAT3 m4 = DirectX::XMFLOAT3(foot.x + factor, foot.y, 0);


						RenderLine(ImVec2(m1.x, m1.y), ImVec2(m2.x, m2.y), *Chosencolor, 2);
						RenderLine(ImVec2(m2.x, m2.y), ImVec2(m3.x, m3.y), *Chosencolor, 2);
						RenderLine(ImVec2(m3.x, m3.y), ImVec2(m4.x, m4.y), *Chosencolor, 2);
						RenderLine(ImVec2(m4.x, m4.y), ImVec2(m1.x, m1.y), *Chosencolor, 2);
					}

					std::wstring name;
					std::string espName;
					if (pPlayer->team != pLocalPlayer->team && settings::ESP::name)
					{
						int fontsize = 12;
						try {
							std::wstring tmpname = std::wstring(&pPlayer->NameClass->name[0], &pPlayer->NameClass->name[20]);
							auto czech = wcstok(&tmpname[0], L" ");
							if (czech != nullptr) { name = czech; }	//nice meme
						}
						catch (const std::exception& exc) {}
						float offset = name.length() * fontsize / 5;
						size_t size;
						espName.resize(name.length());
						wcstombs_s(&size, &espName[0], espName.size() + 1, name.c_str(), name.size());

						RenderText(espName, ImVec2(head.x, (head.y - 6) - (heightoffset / 4)), fontsize, *Chosencolor, true);

					}

					if (pPlayer->team != pLocalPlayer->team && settings::ESP::health)
					{
						float factor = (heightoffset / 5);
						DirectX::XMFLOAT3 m2 = DirectX::XMFLOAT3(head.x - factor, head.y, 0);
						DirectX::XMFLOAT3 m3 = DirectX::XMFLOAT3(foot.x - factor, foot.y, 0);

						float perc = (pSoldier->clientSolderHealthComponent->health / pSoldier->clientSolderHealthComponent->m_flMaxHealth);

						float thicc = heightoffset / 8;
						if (thicc < 4) thicc = 4;
						if (thicc > 6) thicc = 6;
						D3DXCOLOR HealthColor = D3DXCOLOR(1 - perc, perc, 0, 1);

						/*RenderRect(ImVec2(m2.x - 2, m2.y), ImVec2(m3.x - 2, m3.y), blackColor, 0, 0, thicc);
						RenderRect(ImVec2(m2.x-2, m2.y + ((m3.y - m2.y) * (1 - perc))), ImVec2(m3.x-2, m3.y), HealthColor, 0, 0, thicc);*/

						/*RenderRectFilled(ImVec2(m2.x + factor / 2, m2.y - 2), ImVec2(m3.x + factor / 2, m3.y + 2), blackColor, 0, 0);
						RenderRectFilled(ImVec2(m2.x + factor / 2, m2.y + ((m3.y - m2.y) * (1 - perc))), ImVec2(m3.x + factor / 2, m3.y), HealthColor, 0, 0);*/

						/*RenderRect(ImVec2(m2.x -2, m2.y - 2), ImVec2(m3.x + thicc / 2, m3.y), blackColor, 0, 0, thicc);
						RenderRect(ImVec2(m2.x -2, m2.y + ((m3.y - m2.y) * (1 - perc))), ImVec2(m3.x + thicc / 2, m3.y), HealthColor, 0, 0, thicc);*/

						RenderLine(ImVec2(m2.x - thicc, m2.y), ImVec2(m3.x - thicc, m3.y), blackColor, thicc - 2);
						RenderLine(ImVec2(m2.x - thicc, m2.y + ((m3.y - m2.y) * (1 - perc))), ImVec2(m3.x - thicc, m3.y), HealthColor, thicc - 2);
					}

					if (pPlayer->team != pLocalPlayer->team && settings::ESP::dot)
					{
						float factor = (heightoffset / 5);
						float dotRadius = heightoffset / 6.0f;
						if (dotRadius < 2.0f) dotRadius = 2.0f;
						if (dotRadius > 10.0f) dotRadius = 10.0f;
						RenderCircle(ImVec2(head.x, head.y + 6 + factor), dotRadius, *Chosencolor, 1, 16);
					}

				}

			}
		}

		// Show screenshot notice if needed
        if (globals::showScreenshotNotice && globals::canDraw && settings::ESP::fairfightScreenshot) {
            RenderText("Screenshot taken by FairFight", ImVec2(5, 5), 20.0f, 0xffffff00, false);

			// Remove after time has passed
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - globals::lastScreenshotTime).count() >= 20) {
                globals::showScreenshotNotice = false;
            }
        }
	}

	void EndScene()
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->PushClipRectFullScreen();

		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
	}

	float RenderText(const std::string& text, const ImVec2& position, float size, uint32_t color, bool center)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;

		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int i = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = m_pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

			if (center)
			{
				window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

				window->DrawList->AddText(m_pFont, size, { position.x - textSize.x / 2.0f, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
			}
			else
			{
				window->DrawList->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
				window->DrawList->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

				window->DrawList->AddText(m_pFont, size, { position.x, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
			}

			y = position.y + textSize.y * (i + 1);
			i++;
		}

		return y;
	}

	void RenderLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;

		window->DrawList->AddLine(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), thickness);
	}

	void RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness, uint32_t segments)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;

		window->DrawList->AddCircle(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments, thickness);
	}

	void RenderCircleFilled(const ImVec2& position, float radius, uint32_t color, uint32_t segments)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;

		window->DrawList->AddCircleFilled(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments);
	}

	void RenderRect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags, float thickness)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xFF;
		float r = (color >> 16) & 0xFF;
		float g = (color >> 8) & 0xFF;
		float b = (color) & 0xFF;

		window->DrawList->AddRect(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags, thickness);
	}

	void RenderRectFilled(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xFF;
		float r = (color >> 16) & 0xFF;
		float g = (color >> 8) & 0xFF;
		float b = (color) & 0xFF;

		window->DrawList->AddRectFilled(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
	}

	void RenderImage(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, uint32_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xFF;
		float r = (color >> 16) & 0xFF;
		float g = (color >> 8) & 0xFF;
		float b = (color) & 0xFF;

		window->DrawList->AddImage(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }));
	}

	void RenderImageRounded(ID3D11ShaderResourceView* pTexture, const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (color >> 24) & 0xFF;
		float r = (color >> 16) & 0xFF;
		float g = (color >> 8) & 0xFF;
		float b = (color) & 0xFF;

		window->DrawList->AddImageRounded(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
	}
}

