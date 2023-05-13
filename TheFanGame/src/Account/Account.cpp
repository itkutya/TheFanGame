#include "Account.h"

const bool Account::Login(bool loaduplogin) noexcept
{
	if (!loaduplogin)
		this->m_random = this->CreateHashNumber<std::string>(this->m_password) / 333;

	std::ostringstream stream;
	stream << "username=" << this->m_username << "&password=" << this->m_random;
	Database db("http://thefangamedb.000webhostapp.com", "/login.php", sf::Http::Request::Method::Post, sf::seconds(3.f), stream.str());

	if (db.getStatus() != sf::Http::Response::Status::Ok)
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
	else
	{
		enum ServerData { SUCCESS = 0, LVL, XP, XPCAP, Currency };
		if (db.getData(ServerData::SUCCESS) != "Success.")
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Incorrect login details!"));
		else
		{
			this->m_experience = Experience(std::stoull(db.getData(ServerData::LVL)),
											std::stof(  db.getData(ServerData::XP)),
											std::stof(  db.getData(ServerData::XPCAP)));
			this->m_currency =				std::stoull(db.getData(ServerData::Currency));

			if (this->m_rememberme && this->m_random && !this->s_SettingsManager.save("Settings.ini"))
				ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Failed to save settings!"));
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Succesfully loged in!"));
			return true;
		}
	}
	return false;
}

const bool Account::Register() noexcept
{
	if (this->m_email.find('@') == std::string::npos	||
		this->m_email.find('.') == std::string::npos	||
		this->m_username.length() > MAX_USERNAME_LENGTH ||
		!this->m_username.length())
	{
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Cannot create account, provided email is not valid! Or username is invalid!"));
	}
	else
	{
		this->m_random = this->CreateHashNumber<std::string>(this->m_password) / 333;

		std::ostringstream stream;
		stream << "username=" << this->m_username << "&password=" << this->m_random << "&email=" << this->m_email;
		Database db("http://thefangamedb.000webhostapp.com", "/register.php", sf::Http::Request::Method::Post, sf::seconds(3.f), stream.str());

		if (db.getStatus() != sf::Http::Response::Status::Ok)
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
		else
		{
			if (db.getData() != "Success.")
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