#include <memory>
#include <cstdio>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <unordered_map>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui-SFML.h"

#if _WIN32 || _WIN64
    #if _WIN64
        #include "SFML64/System.hpp"
		#include "SFML64/Graphics.hpp"
        #include "SFML64/Network.hpp"
    #else
        #include "SFML32/System.hpp"
		#include "SFML32/Graphics.hpp"
        #include "SFML32/Network.hpp"
    #endif
#endif

#define u8(x) reinterpret_cast<const char*>(u8#x)
#define LOG(fmt, ...) std::printf(fmt, __VA_ARGS__)
#define LOG_ERROR(x) std::printf("\033[31mError:\033[34m %s\033[0m\n", x)
#define LOG_SUCCES(x) std::printf("\033[32mSucces:\033[34m %s\033[0m\n", x)
#define LOG_ERROR_WITH_IP(x, y, z) std::printf("\033[31mError:\033[34m %s \033[37m(%s:%u)\033[0m\n", x, y, z)
#define LOG_SUCCES_WITH_IP(x, y, z) std::printf("\033[32mSucces:\033[34m %s \033[37m(%s:%u)\033[0m\n", x, y, z)

enum class Network_MSG
{
	None, Error, Test, LogInAttempt, LogInResult, Count
};

static sf::TcpListener listener;
static sf::SocketSelector selector;
static std::vector<std::unique_ptr<sf::TcpSocket>> clients;
static std::atomic<bool> shouldRun = true;
static std::mutex mutex;

static std::unordered_map<std::string, std::string> users;

void startServer(const sf::Int64& thickRate = 0)
{
	users.insert({ "admin", "admin" });
	if (listener.listen(52200, sf::IpAddress::getLocalAddress()) != sf::Socket::Done)
		LOG_ERROR("Cannot start server!");
	else
	{
		LOG_SUCCES_WITH_IP("Server has been started!", sf::IpAddress::getLocalAddress().toString().c_str(), listener.getLocalPort());
		selector.add(listener);

		while (shouldRun)
		{
			if (selector.wait(sf::microseconds(thickRate)))
			{
				const std::lock_guard<std::mutex> lock(mutex);
				if (selector.isReady(listener))
				{
					auto client = std::make_unique<sf::TcpSocket>();
					if (listener.accept(*client) == sf::Socket::Done)
					{
						LOG_SUCCES_WITH_IP("Player connected:", client->getRemoteAddress().toString().c_str(), client->getRemotePort());
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
							case sf::Socket::Disconnected:
								LOG_ERROR_WITH_IP("Player disconnected:", client.getRemoteAddress().toString().c_str(), client.getRemotePort());
								disconnected.push_back(it);
								selector.remove(client);
								client.disconnect();
								break;
							case sf::Socket::Done:
							{
								sf::Uint32 tempType;
								packet >> tempType;
								Network_MSG msg = static_cast<Network_MSG>(tempType);
								switch (msg)
								{
								case Network_MSG::LogInAttempt:
								{
									std::string temp_Un;
									std::string temp_Pw;
									packet >> temp_Un >> temp_Pw;
									packet.clear();
									if (users.contains(temp_Un) && users.at(temp_Un) == temp_Pw)
									{
										packet << true;
										LOG_SUCCES_WITH_IP("Player loged in:", client.getRemoteAddress().toString().c_str(), client.getRemotePort());
										client.send(packet);
									}
									else
									{
										packet << false;
										LOG_ERROR_WITH_IP("Player failed to login:", client.getRemoteAddress().toString().c_str(), client.getRemotePort());
										client.send(packet);
									}
									break;
								}
								default:
								{
									for (auto it2 = clients.begin(); it2 != clients.end(); ++it2)
									{
										sf::TcpSocket& client2 = **it2;
										if (it2 != it)
											client2.send(packet);
									}
									break;
								}
								}
							}
							break;
							case sf::Socket::Error:
								LOG_ERROR("ERROR");
								break;
							case sf::Socket::NotReady:
								LOG("NotReady");
								break;
							case sf::Socket::Partial:
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
	std::printf("Shutting down the server!\n");
	clients.clear();
	selector.clear();
	listener.close();
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Server");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);
    ImGui::StyleColorsDark();
    auto& io = ImGui::GetIO();
    io.Fonts->Clear();
    ImFont* font = io.Fonts->AddFontFromFileTTF("Gen Jyuu Gothic Monospace Bold.ttf", 25.0f, 0, io.Fonts->GetGlyphRangesJapanese());
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
            ImGui::Text(u8(平仮名));
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