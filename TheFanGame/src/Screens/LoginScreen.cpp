#include "LoginScreen.h"

void LoginScreen::init(sf::RenderWindow& window)
{
	if (this->s_Settings.load("Settings.ini"))
	{
		this->s_Account->m_username = this->s_Settings[Settings::FileNumber::USERNAME];
		this->s_Account->m_random = std::stoull(this->s_Settings[Settings::FileNumber::RANDOM]);
		this->s_Account->m_rememberme = std::stoi(this->s_Settings[Settings::FileNumber::REMEMBERME]);

		if (this->s_Account->m_rememberme)
			if (this->LoginAccount())
				this->s_StateManager->addGUIState<MainScreen>(true);
	}
}

void LoginScreen::processEvent(sf::Event& event) noexcept
{
}

void LoginScreen::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
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
		{
			if (this->LoginAccount())
				this->s_StateManager->addGUIState<MainScreen>(true);
		}
		ImGui::SameLine();
		if (ImGui::Button("Create Account"))
			this->s_StateManager->addGUIState<RegisterScreen>();
		ImGui::SameLine();
		ImGui::Checkbox("Remember me", &this->s_Account->m_rememberme);
		if (ImGui::Button("Quit"))
			window.close();
	}
	ImGui::End();
}

void LoginScreen::draw(sf::RenderWindow& window) noexcept
{
}

bool LoginScreen::LoginAccount() noexcept
{
	if (this->s_Account->m_rememberme)
		this->s_Account->m_random = this->s_Account->CreateHashNumber<std::string>(this->s_Account->m_username);
	else
		this->s_Account->m_random = 0;

	std::ostringstream stream;
	stream << "username=" << this->s_Account->m_username << "&password=" << this->s_Account->m_password << "&random=" << this->s_Account->m_random;
	sf::Http http("http://thefangamedb.000webhostapp.com");
	sf::Http::Request request("login.php", sf::Http::Request::Method::Post, stream.str());
	sf::Http::Response response = http.sendRequest(request, sf::seconds(3.f));

	if (response.getStatus() != sf::Http::Response::Status::Ok)
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
	else
	{
		std::vector<std::string> data = this->s_FileManager->load(response.getBody(), '#');
		if (data[Settings::ServerData::SUCCESS] != std::string("Success."))
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Incorrect login details!"));
		else
		{
			this->s_Account->m_experience = Experience(std::stoull(data[Settings::ServerData::LVL]), 
													   std::stof(data[Settings::ServerData::XP]),
													   std::stof(data[Settings::ServerData::XPCAP]));
			if (this->s_Account->m_rememberme && this->s_Account->m_random)
				if (!this->s_FileManager->save("Settings.ini", { this->s_Account->m_username,
												  std::to_string(this->s_Account->m_random),
												  std::to_string(this->s_Account->m_rememberme) }))
				{
					ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Failed to save settings!"));
				}
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Succesfully loged in!"));
			return true;
		}
	}
	std::printf("Server response: %i", static_cast<int>(response.getStatus()));
	return false;
}