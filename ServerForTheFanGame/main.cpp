#include <list>
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
static std::list<sf::TcpSocket*> clients;
static sf::SocketSelector selector;
static std::uint32_t currPlayers = 0;

const void startServer()
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
                }
                else
                {
                    delete client;
                }
            }
            else
            {
                for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                {
                    sf::TcpSocket& client = **it;
                    if (selector.isReady(client))
                    {
                        sf::Packet packet;
                        if (client.receive(packet) == sf::Socket::Done)
                        {
                            char msg[256] = { "" };
                            packet >> msg;
                            std::cout << "Client " << client.getRemoteAddress() << " has sent us a msg: " << msg << '\n';
                        }
                        if (client.receive(packet) == sf::Socket::Disconnected)
                        {
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

            if (ImGui::BeginListBox("Current connections\nto the server"))
            {
                for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                {
                    sf::TcpSocket& client = **it;
                    if (selector.isReady(client))
                    {
                        std::string clientInfo = client.getRemoteAddress().toString() + ":" + std::to_string(client.getRemotePort());
                        ImGui::Text(clientInfo.c_str());
                        ImGui::SameLine(200.f);
                        if (ImGui::Button("Kick"))
                        {
                            //TODO: FIX it later...
                        }
                        ImGui::SameLine(275.f);
                        if (ImGui::Button("Ban"))
                        {
                            //TODO: Make a ban list or something...
                        }
                    }
                }
                ImGui::EndListBox();
            }

        }ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    server.terminate();
    ImGui::SFML::Shutdown();

    return 0;
}

/*
sf::TcpSocket socket;
if (socket.connect(sf::IpAddress::getLocalAddress(), 52420) != sf::Socket::Done)
    std::cout << "Error!\n";

char buff[256] = { "" };
ImGui::InputText("Msg to the server", buff, 256);
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
{
    sf::Packet packet;
    packet << buff;
    socket.send(packet);
    packet.clear();
}
*/