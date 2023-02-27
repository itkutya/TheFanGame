#include "Login.h"

void Login::init(sf::RenderWindow& window)
{
	std::vector<std::string> data = std::move(this->s_FileManager->load("Settings.ini"));
	this->s_Account->m_username = data[0];
	this->s_Account->m_random = std::stoull(data[1]);
	this->s_Account->m_rememberme = std::stoi(data[2]);

	if (this->s_Account->m_rememberme)
		if (this->LoginAccount())
			this->s_StateManager->removeLastGUIState();
}

void Login::processEvent(const sf::Event& event) noexcept
{
}

void Login::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
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
			this->s_StateManager->addGUIState<Register>();
		ImGui::SameLine();
		ImGui::Checkbox("Remember me", &this->s_Account->m_rememberme);
		if (ImGui::Button("Quit"))
			window.close();
	}
	ImGui::End();
}

void Login::draw(sf::RenderWindow& window) noexcept
{
}

bool Login::LoginAccount() noexcept
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
		std::vector<std::string> data = std::move(this->s_FileManager->load(response.getBody(), '#'));
		if (data[0] != std::string("Success."))
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Incorrect login details!"));
		else
		{
			this->s_Account->m_Experience = Experience(std::stoull(data[1]), std::stof(data[2]), std::stof(data[3]));
			if (this->s_Account->m_rememberme && this->s_Account->m_random)
				this->s_FileManager->save("Settings.ini");

			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Succesfully loged in!"));
			return true;
		}
	}
	return false;
}