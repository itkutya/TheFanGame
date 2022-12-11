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

	static const void loadFont() noexcept;
	static const void loadStyle() noexcept;

	const bool input(const char* text, const char* tip, std::string& input, const ImGuiInputTextFlags& flags = 0) noexcept
	{
		if (ImGui::InputTextWithHint(text, tip, &input, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank | flags))
			return true;
		return false;
	};
	const bool button(const char* text, const ImVec2 size = ImVec2()) noexcept
	{
		if (ImGui::Button(text, size))
			return true;
		return false;
	};
	const bool button(const char* text, const char* tip, const ImVec2 size = ImVec2()) noexcept
	{
		bool isPushed = false;
		if (ImGui::Button(text, size))
			isPushed = true;
		this->tooltip(tip);
		return isPushed;
	};
	const bool button(const char* text, const ImVec4 color, const ImVec2 size = ImVec2()) noexcept
	{
		bool isPushed = false;
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		if (ImGui::Button(text, size))
			isPushed = true;
		ImGui::PopStyleColor();
		return isPushed;
	};
	const bool button(const char* text, const char* tip, const ImVec4 color, const ImVec2 size = ImVec2()) noexcept
	{
		bool isPushed = false;
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		if (ImGui::Button(text, size))
			isPushed = true;
		ImGui::PopStyleColor();
		this->tooltip(tip);
		return isPushed;
	};
	const bool tooltip(const char* tip) noexcept
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(tip);
			ImGui::EndTooltip();
			return true;
		}
		return false;
	};
private:
};

class panel : public gui
{
public:
	panel(const std::string& title, sf::RenderWindow& window) noexcept
	{
		if (ImGui::Begin(title.c_str(), 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground))
		{
			ImGui::SetWindowSize(title.c_str(), ImVec2((float)window.getSize().x, (float)window.getSize().y));
			ImGui::SetWindowPos(title.c_str(), ImVec2(0.f, 0.f));
		}
	};
	virtual ~panel() noexcept
	{
		ImGui::End();
	};
private:
};

class popup : public gui
{
public:
	popup(const char* text, sf::RenderWindow& window, bool& isOpen, const ImGuiWindowFlags& flags = 0) noexcept : shouldClosePopUp(isOpen)
	{
		if (isOpen)
			ImGui::OpenPopup(text);
		if (ImGui::BeginPopupModal(text, &isOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | flags))
		{
			ImGui::SetWindowPos(text, ImVec2(window.getSize().x / 2.f - ImGui::GetWindowWidth() / 2.f,
											 window.getSize().y / 2.f - ImGui::GetWindowHeight() / 2.f));
		}
	};

	virtual ~popup() noexcept
	{
		if (this->shouldClosePopUp)
			ImGui::EndPopup();
	};

	const void close() noexcept
	{
		this->shouldClosePopUp = false;
		ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	};
private:
	bool& shouldClosePopUp;
};