#include "LoginScreen.h"

void LoginScreen::init(sf::RenderWindow& window)
{
	if (this->s_Account.m_rememberme && this->s_Account.Login(true))
		this->s_StateManager.addGUIState<MainScreen>(this->m_app, true);
}

void LoginScreen::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	if (ImGui::Begin("##Login", 0, flags))
	{
		if (ImGui::InputText("Nickname", &this->s_Account.m_username, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank) ||
			ImGui::InputText("Password", &this->s_Account.m_password, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_Password) ||
			ImGui::Button("Login"))
		{
			if (this->s_Account.Login())
				this->s_StateManager.addGUIState<MainScreen>(this->m_app, true);
		}
		ImGui::SameLine();
		if (ImGui::Button("Create Account"))
			this->s_StateManager.addGUIState<RegisterScreen>();
		ImGui::SameLine();
		ImGui::Checkbox("Remember me", &this->s_Account.m_rememberme);
		
		if (ImGui::Button("Quit"))
			window.close();
	}
	ImGui::End();
}