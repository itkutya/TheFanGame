#include "multiplayerPanel.h"

const void multiplayerPanel::init(window& window) noexcept
{
}

const void multiplayerPanel::processEvent(const sf::Event& event) noexcept
{
}

const void multiplayerPanel::update(window& window, const sf::Time& dt) noexcept
{
	panel MultiplayerPanel("Multiplayer", window.getWindow());

	if (ImGui::BeginChild(ImGuiID(2), ImVec2(ImGui::GetWindowSize().x - 100.f, ImGui::GetWindowSize().y - 275.f), true, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Local"))
			{
				this->m_state = state::Local;
				this->refresLocal();
			}
			if (ImGui::MenuItem("Public"))
			{
				this->m_state = state::Public;
				this->refresPublic();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginListBox("Servers", ImGui::GetContentRegionAvail()))
		{
			for (const auto& server : this->servers)
			{
				std::string serverInfo = server.first.toString() + ':' + std::to_string(server.second);
				if (ImGui::Selectable(serverInfo.c_str()))
					std::printf("connecting to %s.\n", serverInfo.c_str());
			}
			ImGui::EndListBox();
		}
		ImGui::EndChild();
	}

	if (MultiplayerPanel.button("Refress"))
	{
		if (this->m_state == state::Public)
			this->refresPublic();
		else
			this->refresLocal();
	}

	if (MultiplayerPanel.input("Server IP", "127.0.0.0", this->InputIp))
		std::printf("%s\n", this->InputIp.c_str());
	MultiplayerPanel.sameline();
	if (MultiplayerPanel.input("Port", "52420", this->InputPort))
		std::printf("%s\n", this->InputPort.c_str());
	MultiplayerPanel.sameline();
	if (MultiplayerPanel.button("Join", ImVec2(200.f, 50.f)))
		std::printf("Connecting to%s:%s\n", this->InputIp.c_str(), this->InputPort.c_str());

	if (MultiplayerPanel.button("Local Host", ImVec2(300.f, 75.f)))
	{
		this->host.start();
		//this->m_State = state::MultiLobby;
	}
	MultiplayerPanel.sameline();
	if (MultiplayerPanel.button("Public Host", ImVec2(300.f, 75.f)))
	{
		this->host.start();
		//this->m_State = state::MultiLobby;
	}

	//if (MultiplayerPanel.button("Back##Multiplayer", ImVec2(300.f, 75.f)))
		//this->m_State = state::MainMenu;
}

const void multiplayerPanel::draw(window& window) noexcept
{
}

const void multiplayerPanel::refresLocal() noexcept
{
}

const void multiplayerPanel::refresPublic() noexcept
{
	this->servers.clear();
	using namespace std::chrono_literals;
	if (this->network.receive(this->ServerNum).wait_for(3s) == std::future_status::ready)
	{
		for (std::size_t i = 0; i < this->ServerNum; ++i)
			this->servers.push_back({ sf::IpAddress(0), 0 });
	}
}
