#include "includes.h"
#include "utils/globals.h"
#include <chrono>

namespace Renderer
{
	ImFont* m_pFont = NULL;

	RenderView* pRenderer;
	bool isPlayerVisible = false;
	bool canDrawDistance = true;
	uint32_t yellowColor = 0xffffff00;
	uint32_t orangeColor = 0xffff6a00;
	uint32_t redColor = 0xffff1f1f;
	uint32_t blueColor = 0xff7ea6ff;
	uint32_t greenColor = 0xff0c9d00;
	uint32_t blackColor = 0xff000000;

	// Declare and initialize 'head' and 'foot' globally with default values
	DirectX::XMFLOAT3 head = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 foot = { 0.0f, 0.0f, 0.0f };

	// Box corners
	float boxLeft = head.x - 0.0f; // Use a default value for 'factor'
	float boxRight = head.x + 0.0f; // Use a default value for 'factor'
	float boxTop = head.y;
	float boxBottom = foot.y;

	void Initialize()
	{
		std::cout << "Init renderer" << std::endl;
		ImGuiIO& io = ImGui::GetIO();

		m_pFont = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeuib.ttf", 18.0f);
		if (!m_pFont) {
			m_pFont = io.Fonts->AddFontDefault();
			std::cout << "Failed to load Arial, using default font." << std::endl;
		}
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
			if (!IsValidPtr(LocalSoldierPrediction))
			{
				canDrawDistance = false;
			}
			else
			{
				canDrawDistance = true;
			}

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
						// Calculate player distance for box style selection
						float playerDistance = 0.0f;
						bool canDrawCornerBox = true;

						if (IsValidPtr(LocalSoldierPrediction))
						{
							playerDistance = utils::DistanceVec3(LocalSoldierPrediction->pos, PlayerSoldierPrediction->pos);
						}
						else
						{
							canDrawCornerBox = false;
						}

						float factor = (heightoffset / 5);
						DirectX::XMFLOAT3 m2 = DirectX::XMFLOAT3(head.x - factor, head.y, 0);
						DirectX::XMFLOAT3 m1 = DirectX::XMFLOAT3(head.x + factor, head.y, 0);
						DirectX::XMFLOAT3 m3 = DirectX::XMFLOAT3(foot.x - factor, foot.y, 0);
						DirectX::XMFLOAT3 m4 = DirectX::XMFLOAT3(foot.x + factor, foot.y, 0);

						const float cornerLen = 10.0f;
						

						if (playerDistance > 70.0f || !settings::ESP::cornerBox || !canDrawCornerBox)
						{
							// Full Box ESP
							RenderLine(ImVec2(m1.x, m1.y), ImVec2(m2.x, m2.y), *Chosencolor, 2);
							RenderLine(ImVec2(m2.x, m2.y), ImVec2(m3.x, m3.y), *Chosencolor, 2);
							RenderLine(ImVec2(m3.x, m3.y), ImVec2(m4.x, m4.y), *Chosencolor, 2);
							RenderLine(ImVec2(m4.x, m4.y), ImVec2(m1.x, m1.y), *Chosencolor, 2);
						}
						else
						{
							RenderLine(ImVec2(m2.x, m2.y), ImVec2(m2.x + cornerLen, m2.y), *Chosencolor, 2);
							RenderLine(ImVec2(m2.x, m2.y), ImVec2(m2.x, m2.y + cornerLen), *Chosencolor, 2);
							RenderLine(ImVec2(m1.x, m1.y), ImVec2(m1.x - cornerLen, m1.y), *Chosencolor, 2);
							RenderLine(ImVec2(m1.x, m1.y), ImVec2(m1.x, m1.y + cornerLen), *Chosencolor, 2);
							RenderLine(ImVec2(m3.x, m3.y), ImVec2(m3.x + cornerLen, m3.y), *Chosencolor, 2);
							RenderLine(ImVec2(m3.x, m3.y), ImVec2(m3.x, m3.y - cornerLen), *Chosencolor, 2);
							RenderLine(ImVec2(m4.x, m4.y), ImVec2(m4.x - cornerLen, m4.y), *Chosencolor, 2);
							RenderLine(ImVec2(m4.x, m4.y), ImVec2(m4.x, m4.y - cornerLen), *Chosencolor, 2);
						}
					}

					// HP number rendering (replace the old boxCenterY/m4 usage)
					if (pPlayer->team != pLocalPlayer->team && settings::ESP::healthNumber)
					{
						const float hpPaddingX = 8.0f;
						int hpValue = static_cast<int>(pSoldier->clientSolderHealthComponent->health);
						std::string hpText = std::to_string(hpValue) + " HP";
						float factor = (heightoffset / 5);

						float hpY = (head.y + foot.y) / 2.0f;
						float hpX = foot.x + factor + hpPaddingX;

						RenderText(hpText, ImVec2(hpX, hpY), 13.0f, *Chosencolor, false);
					}
					

					std::wstring name;
					std::string espName;
					if (pPlayer->team != pLocalPlayer->team && settings::ESP::name)
					{
						int fontsize = 14;
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

						float nameYOffset = 12.0f + (heightoffset / 10.0f);
						RenderText(espName, ImVec2(head.x, head.y - nameYOffset), fontsize, *Chosencolor, true);

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

					if (pPlayer->team != pLocalPlayer->team && settings::ESP::distance && canDrawDistance)
					{
						float distance = utils::DistanceVec3(LocalSoldierPrediction->pos, PlayerSoldierPrediction->pos);

						if (distance < 0.0f)
						{
							canDrawDistance = false;	
						}

						std::string distanceText = std::to_string(static_cast<int>(distance)) + "m";
						RenderText(distanceText, ImVec2(foot.x, foot.y + 1 + (heightoffset / 12)), 14.0f, *Chosencolor, true);
					}


				}

			}
		}

        if (globals::showScreenshotNotice && globals::canDraw && settings::ESP::fairfightScreenshot) {
            RenderText("Screenshot taken by FairFight", ImVec2(5, 5), 20.0f, 0xffffff00, false);

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

