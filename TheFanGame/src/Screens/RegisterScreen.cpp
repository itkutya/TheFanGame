#include "RegisterScreen.h"

void RegisterScreen::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal(this->m_name, &this->m_open, flags))
	{
		if (ImGui::InputText("Email:",	  &this->s_Account.m_email,	   ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)							       ||
			ImGui::InputText("Username:", &this->s_Account.m_username, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)								   ||
			ImGui::InputText("Password:", &this->s_Account.m_password, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank) ||
			ImGui::Button("Create"))
		{
			if (this->s_Account.Register())
				this->close();
		}

		if (ImGui::Button("Back"))
			this->close();
		ImGui::EndPopup();
	}

	if (!this->m_open)
		this->close();
}