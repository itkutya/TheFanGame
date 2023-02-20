#pragma once

#include "includes.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui-SFML.h"
#include "ImGUI/imgui_notify.h"
#include "ImGUI/tahoma.h"
#include "ImGUI/imgui_stdlib.h"

class window;

class guistate
{
public:
	inline guistate() noexcept = default;
	inline virtual ~guistate() noexcept = default;
	virtual const void init(window& window) = 0;
	virtual const void processEvent(const sf::Event& event) noexcept = 0;
	virtual const void update(window& window, const sf::Time& dt) noexcept = 0;
	virtual const void draw(window& window) noexcept = 0;
};

class guistateSystem
{
public:
	guistateSystem() = delete;
	guistateSystem(const guistateSystem&) = delete;
	guistateSystem(const guistateSystem&&) = delete;
	guistateSystem& operator=(guistateSystem& other) = delete;
	guistateSystem& operator=(const guistateSystem& other) = delete;
	virtual ~guistateSystem() noexcept = default;

	template<typename T>
	inline static const void add(const bool& replace = false) noexcept
	{
		m_add = true;
		m_newState = std::move(std::make_unique<T>());
		m_replace = replace;
	};

	static const void cleanUp() noexcept;
	static const void popCurrent() noexcept;
	static const void processStateChange(window& window) noexcept;
	static const std::unique_ptr<guistate>& getCurrentState() noexcept;
	static const std::vector<std::unique_ptr<guistate>>& getAllStates() noexcept;
	static const std::size_t getSize() noexcept;
private:
	inline static std::vector<std::unique_ptr<guistate>> m_stateStack;
	inline static std::unique_ptr<guistate> m_newState;

	inline static bool m_add = false;
	inline static bool m_replace = false;
	inline static bool m_remove = false;
};

class gui
{
public:
	gui() noexcept = default;
	virtual ~gui() noexcept = default;

	static const void loadFont() noexcept;
	static const void loadStyle() noexcept;

	const bool input(const char* text, const char* tip, std::string& input, const ImGuiInputTextFlags& flags = 0, const float& width = 200.f) noexcept
	{
		ImGui::SetNextItemWidth(width);
		if (ImGui::InputTextWithHint(text, tip, &input, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank | flags))
			return true;
		return false;
	};
	const bool button(const char* text, const ImVec2& size = ImVec2(), const ImVec4& color = ImGui::GetStyleColorVec4(ImGuiCol_Button), const char* tip = "") noexcept
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
	const bool checkbox(const char* text, bool& value, const ImVec2& pos = ImGui::GetCursorPos(), const ImVec4& col = ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), const char* tip = "") noexcept
	{
		bool isPushed = false;
		ImGui::SetCursorPos(pos);
		ImGui::PushStyleColor(ImGuiCol_CheckMark, col);
		if (ImGui::Checkbox(text, &value))
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
	const void sameline() noexcept
	{
		ImGui::SameLine();
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
	explicit popup(const char* text, sf::RenderWindow& window, const ImGuiWindowFlags& flags = 0, bool&& isOpen = true) noexcept : active(isOpen)
	{
		if (isOpen)
			ImGui::OpenPopup(text);
		if (ImGui::BeginPopupModal(text, &isOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | flags))
		{
			ImGui::SetWindowPos(text, ImVec2(window.getSize().x / 2.f - ImGui::GetWindowWidth() / 2.f,
				window.getSize().y / 2.f - ImGui::GetWindowHeight() / 2.f));
			this->shouldClosePopUp = true;
		}
	};

	explicit popup(const char* text, sf::RenderWindow& window, bool& isOpen, const ImGuiWindowFlags& flags = 0) noexcept : active(isOpen)
	{
		if (isOpen)
			ImGui::OpenPopup(text);
		if (ImGui::BeginPopupModal(text, &isOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | flags))
		{
			ImGui::SetWindowPos(text, ImVec2(window.getSize().x / 2.f - ImGui::GetWindowWidth() / 2.f,
											 window.getSize().y / 2.f - ImGui::GetWindowHeight() / 2.f));
			this->shouldClosePopUp = true;
		}
	};

	virtual ~popup() noexcept
	{
		if (this->shouldClosePopUp)
			ImGui::EndPopup();
	};

	const void close() noexcept
	{
		this->active = false;
		ImGui::CloseCurrentPopup();
	};
private:
	bool& active;
	bool shouldClosePopUp = false;
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