#include "gui.h"

gui::gui(window& window) noexcept { this->m_window = &window; }

gui::~gui() noexcept { ImGui::SFML::Shutdown(); }

const void gui::init(sf::RenderWindow& window)
{
	ImGui::SFML::Init(window);
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.f;
	style.FrameRounding = 15.3f;
	style.ScrollbarRounding = 2.5f;

	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);

	this->m_videomodes = sf::VideoMode::getFullscreenModes();
}

const void gui::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	ImGui::SFML::Update(window, dt);

	if (ImGui::Begin("Main Window", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		ImGui::SetWindowSize("Main Window", ImVec2((float)window.getSize().x, (float)window.getSize().y));
		ImGui::SetWindowPos("Main Window", ImVec2(0.f, 0.f));

		ImGui::Text("FPS: %f", 1.f / dt.asSeconds());

		this->createImage(this->m_window->getTexture(0), sf::IntRect(0, 0, 64, 64), sf::Vector2f(350.f, 175.f), sf::Vector2f(300.f, 300.f));
		this->setToolTip("This is a test tip!");

		if (this->createButton("Play", sf::Vector2f(50.f, 115.f), sf::Vector2f(200.f, 50.f)))
			this->m_window->addState<game>();
		this->setToolTip("Starts the game state.");

		if (this->createButton("Settings", sf::Vector2f(50.f, 215.f), sf::Vector2f(200.f, 50.f)))
			this->settingsPanel = true;
		this->setToolTip("Opens the setting menu.");

		if (this->createButton("Quit", sf::Vector2f(50.f, 315.f), sf::Vector2f(200.f, 50.f)))
			window.close();
		this->setToolTip("Goodbye!");

		if (this->settingsPanel)
		{
			if (ImGui::Begin("Settings Panel", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Settings Panel", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Settings Panel", ImVec2(25.f, 25.f));
				ImGui::SetWindowFocus("Settings Panel");

				if (ImGui::Checkbox("Fullscreen: ", &this->fullscreen))
					this->m_window->setFullscreen(this->fullscreen);

				if (ImGui::BeginListBox("Resolution: ", ImVec2(200, 200)))
				{
					for (std::size_t i = 0; i < this->m_videomodes.size(); ++i)
					{
						std::string text = std::to_string(this->m_videomodes[i].width) + "x" + std::to_string(this->m_videomodes[i].height);
						if (ImGui::Selectable(text.c_str()))
							this->m_window->setSize(sf::Vector2u(this->m_videomodes[i].width, this->m_videomodes[i].height));
					}
					ImGui::EndListBox();
				}

				if (ImGui::Checkbox("FPS Limit: ", &this->isFPSLimited))
				{
					if (!this->isFPSLimited)
						this->m_window->setFramerateLimit(0);
					else
						this->m_window->setFramerateLimit(this->fps_limit);
				}
				if (this->isFPSLimited)
				{
					ImGui::SameLine();
					if (ImGui::SliderInt("###Limit: ", &this->fps_limit, 30, 180))
						this->m_window->setFramerateLimit(this->fps_limit);
				}

				ImGui::SliderFloat("Sensivity: ", &this->sensivity, 0.f, 20.f);
				ImGui::SliderFloat("Game volume: ", &this->game_volume, 0.f, 100.f);
				ImGui::SliderFloat("Music volume: ", &this->music_volume, 0.f, 100.f);

				if (this->createButton("Back", sf::Vector2f(ImGui::GetWindowSize().x - 250.f, ImGui::GetWindowSize().y - 100.f), sf::Vector2f(200.f, 50.f)))
					this->settingsPanel = false;

				ImGui::End();
			}
		}
		ImGui::End();
	}
}

const void gui::processEvent(const sf::Event& event) noexcept { ImGui::SFML::ProcessEvent(event); }

const void gui::draw(sf::RenderWindow& window) noexcept { ImGui::SFML::Render(window); }

const bool gui::createButton(const char* name, const sf::Vector2f& pos, const sf::Vector2f& size) noexcept
{
	ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
	if (ImGui::Button(name, ImVec2(size.x, size.y)))
		return true;
	return false;
}

const void gui::createImage(const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f& pos, const sf::Vector2f& size) noexcept
{
	this->sprite.setTexture(texture);
	this->sprite.setTextureRect(rect);
	ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
	ImGui::Image(this->sprite, size);
}

const bool gui::setToolTip(const char* text) noexcept
{
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(text);
		return true;
	}
	return false;
}
