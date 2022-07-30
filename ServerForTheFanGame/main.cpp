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

sf::TcpListener listener;
std::list<sf::TcpSocket*> clients;
sf::SocketSelector selector;

const void startServer();

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

            if (ImGui::BeginListBox("Current connections to the server"))
            {
                for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                {
                    sf::TcpSocket& client = **it;
                    std::string clientIP = client.getRemoteAddress().toString();
                    std::string clientPort = std::to_string(client.getRemotePort());
                    std::string clientInfo = clientIP + ":" + clientPort;
                    if (ImGui::Selectable(clientInfo.c_str()))
                    {
                        std::cout << "A client has been selected.\n";
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
                        packet.clear();
                    }
                }
            }
        }
    }
};

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