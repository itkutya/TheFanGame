#include <memory>
#include <cstdio>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <fstream>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui-SFML.h"

#if _WIN32 || _WIN64
    #if _WIN64
		#include "SFML64/SFML/Graphics.hpp"
        #include "SFML64/SFML/Network.hpp"
    #else
		#include "SFML32/Graphics.hpp"
        #include "SFML32/Network.hpp"
    #endif
#endif

#define u8(x) reinterpret_cast<const char*>(u8#x)
#define LOG(fmt, ...) std::printf(fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...)	{											\
								std::printf("\033[31mError:\033[34m "); \
								std::printf(fmt, __VA_ARGS__);			\
								std::printf("\033[0m\n");				\
							}

#define LOG_SUCCES(fmt, ...){											\
								std::printf("\033[32mSucces:\033[34m ");\
								std::printf(fmt, __VA_ARGS__);			\
								std::printf("\033[0m\n");				\
							}

static sf::TcpListener listener;
static sf::SocketSelector selector;
static std::vector<std::unique_ptr<sf::TcpSocket>> clients;
static std::atomic<bool> shouldRun = true;
static std::mutex mutex;

void startServer(const std::uint64_t& thickRate = 0) noexcept
{
	if (listener.listen(52200, sf::IpAddress::getLocalAddress().value()) != sf::Socket::Status::Done)
	{
		LOG_ERROR("Cannot start server!");
	}
	else
	{
		LOG_SUCCES("Server has been started! %s(%s:%u)", "\033[37m", sf::IpAddress::getLocalAddress().value().toString().c_str(), listener.getLocalPort());
		selector.add(listener);

		while (shouldRun)
		{
			if (selector.wait(sf::microseconds(thickRate)))
			{
				const std::lock_guard<std::mutex> lock(mutex);
				if (selector.isReady(listener))
				{
					auto client = std::make_unique<sf::TcpSocket>();
					if (listener.accept(*client) == sf::Socket::Status::Done)
					{
						LOG_SUCCES("Player connected: %s(%s:%u)", "\033[37m", client->getRemoteAddress().value().toString().c_str(), client->getRemotePort());
						clients.push_back(std::move(client));
						auto& newClient = (**(clients.end() - 1));
						selector.add(newClient);
					}
				}
				else
				{
					std::vector<std::vector<std::unique_ptr<sf::TcpSocket>>::iterator> disconnected;
					for (auto it = clients.begin(); it != clients.end(); ++it)
					{
						sf::TcpSocket& client = **it;
						if (selector.isReady(client))
						{
							sf::Packet packet;
							switch (client.receive(packet))
							{
							case sf::Socket::Status::Disconnected:
								LOG_ERROR("Player disconnected: %s(%s:%u)", "\033[37m", client.getRemoteAddress().value().toString().c_str(), client.getRemotePort());
								disconnected.push_back(it);
								selector.remove(client);
								client.disconnect();
								break;
							case sf::Socket::Status::Done:
							{
								for (auto it2 = clients.begin(); it2 != clients.end(); ++it2)
								{
									sf::TcpSocket& client2 = **it2;
									if (it2 != it)
										auto temp = client2.send(packet);
								}
							}
							break;
							case sf::Socket::Status::Error:
								LOG_ERROR("ERROR");
								break;
							case sf::Socket::Status::NotReady:
								LOG("NotReady");
								break;
							case sf::Socket::Status::Partial:
								LOG("Partial");
								break;
							default:
								LOG("...");
								break;
							}
						}
					}
					for (auto it = disconnected.begin(); it != disconnected.end(); ++it)
						clients.erase(*it);
					disconnected.clear();
				}
			}
		}
	}
	LOG_SUCCES("Shutting down the server!\n");
	clients.clear();
	selector.clear();
	listener.close();
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Server");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);
    ImGui::StyleColorsDark();
    auto& io = ImGui::GetIO();
    io.Fonts->Clear();
    ImFont* font = io.Fonts->AddFontFromFileTTF("tuffy.ttf", 25.0f, 0, io.Fonts->GetGlyphRangesJapanese());
    ImGui::SFML::UpdateFontTexture();

	std::thread server(&startServer, 16);

    sf::Clock deltaTime;
    while (window.isOpen())
    {
        ImGui::SFML::Update(window, deltaTime.restart());

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (ImGui::Begin("Main Sever"))
        {
			std::lock_guard<std::mutex> lock(mutex);
            ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
			ImGui::Text("Currently Connected Users: ");
			for (auto& client : clients)
			{
				std::string text = client->getRemoteAddress().value().toString() + std::to_string(client->getRemotePort());
				if (ImGui::BeginListBox("##Connected_clients"))
				{
					ImGui::Selectable(text.c_str());
					ImGui::EndListBox();
				}
			}
        }ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
	shouldRun = false;
	server.join();
    return 0;
}