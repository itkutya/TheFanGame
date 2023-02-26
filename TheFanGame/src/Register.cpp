#include "Register.h"

void Register::init(sf::RenderWindow& window)
{
	this->m_open = true;
}

void Register::processEvent(const sf::Event& event) noexcept
{
}

void Register::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	if (this->m_once)
	{
		ImGui::OpenPopup("Register");
		this->m_once = false;
	}

	constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Register", &this->m_open, flags))
	{
		if (ImGui::InputText("Email:", &this->s_Account->m_email, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)										||
			ImGui::InputText("Username:", &this->s_Account->m_username, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)								||
			ImGui::InputText("Password:", &this->s_Account->m_password, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank) ||
			ImGui::Button("Create"))
		{
			if (this->RegisterAccount())
				this->close();
		}

		if (ImGui::Button("Back"))
			this->close();
		ImGui::EndPopup();
	}

	if (!this->m_open)
		this->close();
}

void Register::draw(sf::RenderWindow& window) noexcept
{
}

bool Register::RegisterAccount() noexcept
{
	if (this->s_Account->m_email.find('@') == std::string::npos ||
		this->s_Account->m_email.find('.') == std::string::npos ||
		this->s_Account->m_username.length() > MAX_USERNAME_LENGTH ||
		!this->s_Account->m_username.length())
	{
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Cannot create account, provided email is not valid! Or username is invalid!"));
	}
	else
	{
		std::ostringstream stream;
		stream << "username=" << this->s_Account->m_username << "&password=" << this->s_Account->m_password << "&email=" << this->s_Account->m_email;
		sf::Http http("http://thefangamedb.000webhostapp.com");
		sf::Http::Request request("register.php", sf::Http::Request::Method::Post, stream.str());
		sf::Http::Response response = http.sendRequest(request, sf::seconds(3.f));

		if (response.getStatus() != sf::Http::Response::Status::Ok)
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
		else
		{
			if (response.getBody().find("Success.") == std::string::npos)
				ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Failed to create an account!"));
			else
			{
				ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Succesfully created an account!"));
				return true;
			}
		}
	}
	return false;
}

void Register::close() noexcept
{
	ImGui::CloseCurrentPopup();
	this->s_StateManager->removeLastGUIState();
}