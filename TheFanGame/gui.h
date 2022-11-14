#pragma once

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/SFML/Graphics.hpp"
	#else
		#include "SFML32/Graphics.hpp"
	#endif
#endif

#include "ImGUI/imgui.h"
#include "ImGUI/imgui-SFML.h"
#include "ImGUI/imgui_notify.h"
#include "ImGUI/tahoma.h"
#include "ImGUI/imgui_stdlib.h"

class gui
{
public:
	gui() noexcept = default;
	virtual ~gui() noexcept = default;

	inline const bool beginPopup(const std::string& title, bool& isOpen, sf::RenderWindow& window) noexcept
	{
		if (ImGui::BeginPopupModal("Create Account", &isOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::SetWindowPos("Create Account", ImVec2(window.getSize().x / 2.f - ImGui::GetWindowWidth() / 2.f,
														 window.getSize().y / 2.f - ImGui::GetWindowHeight() / 2.f));
			return true;
		}
		return false;
	};
	inline const void endPopup() noexcept { ImGui::EndPopup(); };
	const bool tooltip() noexcept;
private:
};

class panel
{
public:
	panel(const std::string& title, sf::RenderWindow& window) noexcept
	{
		if (ImGui::Begin(title.c_str(), 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground))
		{
			ImGui::SetWindowSize(title.c_str(), ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
			ImGui::SetWindowPos(title.c_str(), ImVec2(50.f, 50.f));
		}
	};
	virtual ~panel() noexcept
	{
		ImGui::End();
	};
private:
};