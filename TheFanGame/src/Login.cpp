#include "Login.h"

void Login::init(sf::RenderWindow& window)
{
}

void Login::processEvent(const sf::Event& event) noexcept
{
}

void Login::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	if (ImGui::Begin("##Login", 0, flags))
	{
		if (ImGui::InputText("Nickname", &this->s_Account->m_username, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank) ||
			ImGui::InputText("Password", &this->s_Account->m_password, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_Password) ||
			ImGui::Button("Login"))
			if (this->s_Account->Login())
				std::printf("Login...\n");
		ImGui::SameLine();
		if (ImGui::Button("Create Account"))
			std::printf("Soon TM...");
		ImGui::SameLine();
		if (ImGui::Checkbox("Remember me", &this->s_Account->m_rememberme) && this->s_Account->m_rememberme)
			this->s_Account->CreateRandomNumber();
	}
	ImGui::End();
}

void Login::draw(sf::RenderWindow& window) noexcept
{
}