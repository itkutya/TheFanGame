#include "Account.h"

Account& Account::getInstance()
{
	static Account instance;
	return instance;
}

bool Account::Login() noexcept
{
	std::ostringstream stream;
	stream << "username=" << this->m_username << "&password=" << this->m_password;
	if (this->m_rememberme && this->m_random)
		stream << "random=" << this->m_random;

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
			return true;
		}
	}
	return false;
}

bool Account::Register() noexcept
{
	/*
	if (email.find('@') == std::string::npos ||
		email.find('.') == std::string::npos ||
		name.length() > MAX_USERNAME_LENGTH ||
		!name.length())
	{
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Cannot create account, provided email is not valid! Or username is invalid!"));
	}
	else
	{
		request createProfile("thefangamedb.000webhostapp.com", "/register.php");
		std::ostringstream stream = createProfile.setParams<std::string>("username=", name, "&password=", password, "&email=", email);

		if (!createProfile.sendRequest(stream.str(), sf::seconds(3)))
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
		else
		{
			if (createProfile.getResponse().getBody().find("Success.") == std::string::npos)
				ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Failed to create an account!"));
			else
			{
				profile::name = name;
				ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Succesfully created an account!"));
				return true;
			}
		}
	}
	return false;
	*/
	return false;
}

void Account::CreateRandomNumber() noexcept
{
	std::random_device rd;
	std::uniform_int_distribution<std::uint64_t> dist(UINT32_MAX, UINT64_MAX);
	this->m_random = dist(rd);
}