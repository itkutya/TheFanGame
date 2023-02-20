#include "loginPanel.h"

const void loginPanel::init(window& window) noexcept
{
}

const void loginPanel::processEvent(const sf::Event& event) noexcept
{
}

const void loginPanel::update(window& window, const sf::Time& dt) noexcept
{
	panel loginPanel("Login", window.getWindow());
	if (loginPanel.input("User Name:", "Enter your user name here...", settings::ProfileNickname) ||
		loginPanel.input("Password:", "Enter your password here...", settings::ProfilePassword, ImGuiInputTextFlags_Password) ||
		loginPanel.button("Login"))
		if (this->login(settings::ProfileNickname, settings::ProfilePassword))
			guistateSystem::add<mainmenuPanel>(true);

	loginPanel.sameline();
	if (loginPanel.button("Create Account"))
		this->createAccountPanel = true;
	loginPanel.sameline();
	if (loginPanel.checkbox("Remember Me!", settings::rememberToStayLogedIn))
		settings::saveSettings("res/Settings.ini");

	if (this->createAccountPanel)
	{
		popup createAccount("Create Account", window.getWindow(), this->createAccountPanel);
		if (createAccount.input("Email:", "Enter your email here...", this->createAccountEmail) ||
			createAccount.input("Username:", "Enter your name here...", this->createAccountName) ||
			createAccount.input("Password:", "Enter your password here...", this->createAccountPassword, ImGuiInputTextFlags_Password) ||
			createAccount.button("Create"))
		{
			if (this->createAccount(this->createAccountName, this->createAccountPassword, this->createAccountEmail))
				guistateSystem::add<mainmenuPanel>(true);
		}
		loginPanel.sameline();
		if (createAccount.button("Cancel"))
			createAccount.close();
	}
}

const void loginPanel::draw(window& window) noexcept
{
}

const bool loginPanel::login(const std::string& name, const std::string& password) noexcept
{
	//TODO:
	//if login was succesful && remember me -> generate buffer so it will remember it next time...
	if (settings::rememberToStayLogedIn)
		std::printf("Do the magic...");

	request loginRequest("thefangamedb.000webhostapp.com", "/login.php");
	std::ostringstream stream = loginRequest.setParams<std::string>("username=", name, "&password=", password);

	if (!loginRequest.sendRequest(stream.str(), sf::seconds(3)))
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
	else
	{
		if (loginRequest.getResponse().getBody().find("Success.") == std::string::npos)
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Incorrect login details!"));
		else
		{
			profile::name = name;
			std::stringstream ss(loginRequest.getResponse().getBody());
			std::string line;
			int x = 0;
			while (std::getline(ss, line, '#'))
			{
				if (x == 1)
					profile::lvl = std::stoi(line);
				if (x == 2)
					profile::xp = std::stof(line);
				if (x == 3)
					profile::xp_cap = std::stof(line);
				++x;
			}
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Succesfully loged in!"));
			return true;
		}
	}
	return false;
}

const bool loginPanel::createAccount(const std::string& name, const std::string& password, const std::string& email) noexcept
{
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
}