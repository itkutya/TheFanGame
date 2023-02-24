#include "Account.h"

Account& Account::getInstance()
{
	static Account instance;
	return instance;
}

bool Account::Login() noexcept
{
	if (this->m_rememberme)
		this->m_random = this->CreateHashNumber<std::string>(this->m_username);
	else
		this->m_random = 0;

	std::ostringstream stream;
	stream << "username=" << this->m_username << "&password=" << this->m_password << "&random=" << this->m_random;
	sf::Http http("http://thefangamedb.000webhostapp.com");
	sf::Http::Request request("login.php", sf::Http::Request::Method::Post, stream.str());
	sf::Http::Response response = http.sendRequest(request, sf::seconds(3.f));

	if (response.getStatus() != sf::Http::Response::Status::Ok)
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
	else
	{
		if (response.getBody().find("Success.") == std::string::npos)
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Incorrect login details!"));
		else
		{
			std::stringstream ss(response.getBody());
			std::string line;
			int x = 0;
			std::uint64_t lvl = 0;
			float xp = 0.f;
			float xpcap = 0.f;
			while (std::getline(ss, line, '#'))
			{
				if (x == 1)
					lvl = std::stoi(line);
				if (x == 2)
					xp = std::stof(line);
				if (x == 3)
					xpcap = std::stof(line);
				++x;
			}
			this->m_Experience = Experience(lvl, xp, xpcap);
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Succesfully loged in!"));

			if (this->m_rememberme && this->m_random)
			{
				std::ofstream file;
				file.open("Settings.ini");
				if (file.is_open())
					file << this->m_username << '\n' << this->m_random << '\n' << this->m_rememberme;
				file.close();
			}
			return true;
		}
	}
	return false;
}

bool Account::Register() noexcept
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
		std::ostringstream stream;
		stream << "username=" << this->m_username << "&password=" << this->m_password << "&email=" << this->m_email;
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