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
	const bool button(const char* text, const ImVec2& size = ImVec2(), const ImVec4& color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_Button)), const char* tip = "") noexcept
	{
		bool isPushed = false;
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		if (ImGui::Button(text, size))
			isPushed = true;
		ImGui::PopStyleColor();
		if (tip != "")
			this->tooltip(tip);
		return isPushed;
	};
	template<typename... Args>
	const void text(const char* t, const ImVec4& color = ImVec4(1, 1, 1, 1), const ImVec2& pos = ImGui::GetCursorPos(), Args... args) noexcept
	{
		ImGui::SetCursorPos(pos);
		if constexpr (sizeof...(args) > 0)
			ImGui::TextColored(color, t, args...);
		else
			ImGui::TextColored(color, t);
	};
	const bool tooltip(const char* tip, const ImVec4& color = ImVec4(1, 1, 1, 1)) noexcept
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			this->text(tip, color);
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

class table : public gui
{
public:
	table(const char* text, const ImVec2& size, const ImGuiTableFlags& flags = 0) noexcept
	{
		if (ImGui::BeginTable(text, (int)size.y, flags))
		{
			ImGui::TableNextColumn();
			this->shouldEndTable = true;
		}
	};
	virtual ~table()
	{
		if (this->shouldEndTable)
		{
			ImGui::EndTable();
			this->shouldEndTable = false;
		}
	};
private:
	bool shouldEndTable = false;
};