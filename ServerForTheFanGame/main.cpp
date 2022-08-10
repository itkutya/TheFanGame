#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>

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

static sf::TcpListener listener;
static std::vector<sf::TcpSocket*> clients;
static sf::SocketSelector selector;
static std::uint32_t currPlayers = 0;
static std::vector<std::pair<sf::IpAddress, sf::Uint16>> servers;
static sf::Mutex mutex;

void startServer()
{
    listener.listen(52420);
    selector.add(listener);

    while (true)
    {
        if (selector.wait())
        {
            if (selector.isReady(listener))
            {
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done)
                {
                    clients.push_back(client);
                    selector.add(*client);
                    std::cout << "A client has connected: " << client->getRemoteAddress() << ":" << client->getRemotePort() << '\n';
                    ++currPlayers;

                    sf::Packet packet;
                    packet << std::uint32_t(servers.size());
                    if (client->send(packet) == sf::Socket::Done)
                    {
                        packet.clear();
                        for (std::size_t j = 0; j < servers.size(); ++j)
                        {
                            packet << servers[j].first.toString() << servers[j].second;
                            if (client->send(packet) != sf::Socket::Done)
                                std::cout << "Error! Cannot reach the client...\n";
                            packet.clear();
                        }
                    }
                    packet.clear();
                }
                else
                    delete client;
            }
            else
            {
                sf::Lock lock(mutex);
                for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                {
                    sf::TcpSocket& client = **it;
                    if (selector.isReady(client)) 
                    {
                        sf::Packet packet;
                        if (client.receive(packet) == sf::Socket::Done)
                        {
                            if(packet.getDataSize() == sizeof(std::uint8_t))
                            {
                                std::uint8_t data;
                                packet >> data;
                                std::cout << data << '\n';
                                packet.clear();

                                switch (data)
                                {
                                case 52:
                                    std::cout << "Quit!\n";
                                    for (std::vector<std::pair<sf::IpAddress, sf::Uint16>>::iterator it = servers.begin(); it != servers.end(); ++it)
                                    {
                                        if (client.getRemoteAddress() == it->first)
                                        {
                                            std::cout << "deleted\n";
                                            servers.erase(it);
                                            break;
                                        }
                                    }
                                    break;
                                case 53:
                                    std::cout << "Refress!\n";
                                    packet << std::uint32_t(servers.size());
                                    if (client.send(packet) == sf::Socket::Done)
                                    {
                                        packet.clear();
                                        for (std::size_t j = 0; j < servers.size(); ++j)
                                        {
                                            packet << servers[j].first.toString() << servers[j].second;
                                            if (client.send(packet) != sf::Socket::Done)
                                                std::cout << "Error! Cannot reach the client...\n";
                                            packet.clear();
                                        }
                                    }
                                    packet.clear();
                                    break;
                                default:
                                    std::cout << "Client sent an unknown command...\n";
                                    break;
                                }
                            }
                            else
                            {
                                std::string ip;
                                sf::Uint16 port;
                                packet >> ip >> port;
                                std::cout << "Client " << client.getRemoteAddress() << " has sent us a msg: " << ip << ":" << port << '\n';
                                servers.push_back(std::pair<sf::IpAddress, sf::Uint16>(sf::IpAddress(ip), port));
                            }
                        }
                        else
                        {
                            //std::cout << "Error! while receeving the packet...\n";
                            std::cout << "Player disconnected: " << client.getRemoteAddress().toString() << ":" << client.getRemotePort() << '\n';
                            selector.remove(client);
                            client.disconnect();
                            clients.erase(it);
                            --currPlayers;
                            clients.resize(currPlayers);
                            break;
                        }
                        packet.clear();
                    }
                }
            }
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Server");
    window.setFramerateLimit(60);

    sf::Clock deltaTime;

    ImGui::SFML::Init(window);
    ImGui::StyleColorsDark();

    sf::Thread server(&startServer);
    server.launch();

    while (window.isOpen())
    {
        sf::Time dt = deltaTime.restart();
        ImGui::SFML::Update(window, dt);

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(ImGui::Begin("Teszt"))
        {
            ImGui::Text("FPS: %.3f", 1.f / dt.asSeconds());
            ImGui::Text("Current online players: %u", currPlayers);
            ImGui::Text("Current local servers: %u", servers.size());

            if (ImGui::BeginListBox("Current connections\nto the server"))
            {
                for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                {
                    sf::TcpSocket& client = **it;
                    std::string clientInfo = client.getRemoteAddress().toString() + ":" + std::to_string(client.getRemotePort());
                    ImGui::Text(clientInfo.c_str());
                    ImGui::SameLine(200.f);
                    ImGui::PushID(clientInfo.c_str());
                    if (ImGui::Button("Kick"))
                    {
                        //TODO: FIX me later...
                    }
                    ImGui::PopID();
                    ImGui::SameLine(275.f);
                    ImGui::PushID(clientInfo.c_str());
                    if (ImGui::Button("Ban"))
                    {
                        //TODO: Make a ban list or something...
                    }
                    ImGui::PopID();
                }
            }ImGui::EndListBox();
        }ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    server.terminate();
    ImGui::SFML::Shutdown();

    return 0;
}