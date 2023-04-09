#include "Account.h"

Account& Account::getInstance()
{
	static Account instance;
	return instance;
}

const bool Account::Login(bool loaduplogin) noexcept
{
	if (!loaduplogin)
		this->m_random = (this->CreateHashNumber<std::string>(this->m_password) / 333);
	std::ostringstream stream;
	stream << "username=" << this->m_username << "&password=" << this->m_random;
	sf::Http http("http://thefangamedb.000webhostapp.com");
	sf::Http::Request request("/login.php", sf::Http::Request::Method::Post, stream.str());
	sf::Http::Response response = http.sendRequest(request, sf::seconds(3.f));

	if (response.getStatus() != sf::Http::Response::Status::Ok)
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
	else
	{
		enum ServerData { SUCCESS = 0, LVL, XP, XPCAP, Currency };
		std::printf("%s\n", response.getBody().c_str());

		std::vector<std::string> data;
		std::stringstream ss(response.getBody());
		while (std::getline(ss, data.emplace_back(), '#'));
		if (data[ServerData::SUCCESS] != std::string("Success."))
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Incorrect login details!"));
		else
		{
			this->m_experience = Experience(std::stoull(data[ServerData::LVL]),
											std::stof(data[ServerData::XP]),
											std::stof(data[ServerData::XPCAP]));
			this->m_currency =				std::stoull(data[ServerData::Currency]);

			if (this->m_rememberme && this->m_random && !this->s_Settings.save("Settings.ini"))
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
		this->m_random = (this->CreateHashNumber<std::string>(this->m_password) / 333);
		std::ostringstream stream;
		stream << "username=" << this->m_username << "&password=" << this->m_random << "&email=" << this->m_email;
		sf::Http http("http://thefangamedb.000webhostapp.com");
		sf::Http::Request request("/register.php", sf::Http::Request::Method::Post, stream.str());
		sf::Http::Response response = http.sendRequest(request, sf::seconds(3.f));

		if (response.getStatus() != sf::Http::Response::Status::Ok)
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
		else
		{
			std::printf("%s\n", response.getBody().c_str());

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