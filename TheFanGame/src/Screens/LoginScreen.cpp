#include "LoginScreen.h"

void LoginScreen::init(sf::RenderWindow& window)
{
	this->s_Account.m_random = std::stoull(this->s_Settings[SettingsManager::FileNumber::RANDOM]);
	this->s_Account.m_rememberme = std::stoi(this->s_Settings[SettingsManager::FileNumber::REMEMBERME]);

	if (this->s_Account.m_rememberme && this->LoginAccount())
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
			if (this->LoginAccount())
				this->s_StateManager.addGUIState<MainScreen>(this->m_app, true);
		}
		ImGui::SameLine();
		if (ImGui::Button("Create Account"))
			this->s_StateManager.addGUIState<RegisterScreen>();
		ImGui::SameLine();
		if (ImGui::Checkbox("Remember me", &this->s_Account.m_rememberme))
			this->s_Settings[SettingsManager::FileNumber::REMEMBERME] = this->s_Account.m_rememberme ? "1" : "0";
		if (ImGui::Button("Quit"))
			window.close();
	}
	ImGui::End();
}

bool LoginScreen::LoginAccount() noexcept
{
	if (this->s_Account.m_rememberme)
		this->s_Account.m_random = this->s_Account.CreateHashNumber<std::string>(this->s_Account.m_username);
	else
		this->s_Account.m_random = 0;
	this->s_Settings[SettingsManager::FileNumber::RANDOM] = std::to_string(this->s_Account.m_random);

	std::ostringstream stream;
	stream << "username=" << this->s_Account.m_username << "&password=" << this->s_Account.m_password << "&random=" << this->s_Account.m_random;
	sf::Http http("http://thefangamedb.000webhostapp.com");
	sf::Http::Request request("/login.php", sf::Http::Request::Method::Post, stream.str());
	sf::Http::Response response = http.sendRequest(request, sf::seconds(3.f));

	if (response.getStatus() != sf::Http::Response::Status::Ok)
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
	else
	{
		std::vector<std::string> data;
		std::stringstream ss(response.getBody());
		while (std::getline(ss, data.emplace_back(), '#'));
		if (data[SettingsManager::ServerData::SUCCESS] != std::string("Success."))
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Incorrect login details!"));
		else
		{
			this->s_Account.m_experience = Experience(std::stoull(data[SettingsManager::ServerData::LVL]), 
													   std::stof(data[SettingsManager::ServerData::XP]),
													   std::stof(data[SettingsManager::ServerData::XPCAP]));
			if (this->s_Account.m_rememberme && this->s_Account.m_random && !this->s_Settings.save("Settings.ini"))
				ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Failed to save settings!"));
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Succesfully loged in!"));
			return true;
		}
	}
	return false;
}