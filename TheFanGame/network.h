#pragma once

#include <memory>
#include <stdlib.h>
#include <stdio.h>
#include <future>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <utility>
#include <unordered_map>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/System.hpp"
		#include "SFML64/Network.hpp"
	#else
		#include "SFML32/System.hpp"
		#include "SFML32/Network.hpp"
	#endif
#endif

enum class Network_MSG
{
	None, Error, Test, LogInAttempt, LogInResult, Count
};

class client
{
public:
	client() noexcept = default;
	virtual ~client() noexcept 
	{ 
		this->disconnect();
		for (auto& data : this->m_data)
			if (data.second != nullptr)
				delete data.second;
	};

	const void update() noexcept
	{
		this->m_future.erase(std::remove_if(this->m_future.begin(), this->m_future.end(),
			[](const std::future<void>& future)
			{
				using namespace std::chrono_literals;
				return (future.wait_for(0ms) == std::future_status::ready);
			}), this->m_future.end());

		std::lock_guard<std::mutex> lock(this->m_mutex);
		if (this->m_packet.getDataSize() > 0)
		{
			sf::Uint32 type;
			this->m_packet >> type;
			Network_MSG msg = static_cast<Network_MSG>(type);
			switch (msg)
			{
			case Network_MSG::None:
				break;
			case Network_MSG::Error:
				break;
			case Network_MSG::Test:
			{
				std::string temp;
				this->m_packet >> temp;
				this->m_data[msg] = new std::string(temp);
				break;
			}
			case Network_MSG::LogInResult:
			{
				bool tempResult;
				this->m_packet >> tempResult;
				this->m_data[msg] = new bool(tempResult);
				break;
			}
			default:
				break;
			}
			this->m_packet.clear();
		}
	};
	/*
	 * with std::move we get the data once, and after that it gets set to nullptr,
	 * without std::move, we continiously can get the sent data...
	 */
	template<typename T>
	inline const T* getData(const Network_MSG& msg) noexcept
	{
		std::lock_guard<std::mutex> lock(this->m_mutex);
		if (this->m_data[msg] != nullptr)
			return static_cast<T*>(this->m_data.at(msg));
		else
			return static_cast<T*>(nullptr);
	};

	inline const std::future<void>& keepAlive() noexcept
	{
		return this->m_future.emplace_back(std::async(std::launch::async,
			[&]()
			{
				this->m_keepAlive = true;
				sf::SocketSelector selector;
				selector.add(this->m_socket);
				while (this->m_keepAlive)
				{
					if (selector.wait(sf::microseconds(1)))
					{
						std::lock_guard<std::mutex> lock(this->m_mutex);
						if (selector.isReady(this->m_socket))
						{
							switch (this->m_socket.receive(this->m_packet))
							{
							case sf::Socket::Done:
								std::printf("Done\n");
								break;
							case sf::Socket::NotReady:
								std::printf("NotReady\n");
								break;
							case sf::Socket::Partial:
								std::printf("Partial\n");
								break;
							case sf::Socket::Disconnected:
							{
								this->m_keepAlive = false;
								this->m_socket.disconnect();
								break;
							}
							case sf::Socket::Error:
								std::printf("Error\n");
								break;
							default:
								break;
							}
						}
					}
				}
				selector.clear();
			}));
	};

	inline const bool join(const sf::IpAddress& ip = sf::IpAddress::getLocalAddress(), const sf::Uint16& port = 52200, const sf::Time& timeout = sf::Time::Zero) noexcept
	{
		this->disconnect();
		if (this->m_socket.connect(ip, port, timeout) == sf::Socket::Done)
			return true;
		return false;
	};

	inline const void disconnect() noexcept 
	{
		this->m_keepAlive = false;
		for (auto& future : this->m_future)
			future.wait();
		this->m_socket.disconnect(); 
	};
	
	template<typename... Args>
	inline const std::future<void>& send(Args&... args) noexcept
	{
		return this->m_future.emplace_back(std::async(std::launch::async,
			[&](Args... arguments)
			{
				std::lock_guard<std::mutex> lock(this->m_mutex);
				int dummy[] = { 0, ((void)(this->m_packet << std::forward<Args>(arguments)), 0)... };
				this->m_socket.send(this->m_packet);
				this->m_packet.clear();
			}, args...));
	};

	template<typename... Args>
	inline const std::future<void>& send(const Args&... args) noexcept
	{
		return this->m_future.emplace_back(std::async(std::launch::async,
			[&](Args... arguments)
			{
				std::lock_guard<std::mutex> lock(this->m_mutex);
				int dummy[] = { 0, ((void)(this->m_packet << std::forward<Args>(arguments)), 0)... };
				this->m_socket.send(this->m_packet);
				this->m_packet.clear();
			}, args...));
	};

	template<typename... Args>
	inline const std::future<void>& receive(Args&... args) noexcept
	{
		return this->m_future.emplace_back(std::async(std::launch::async,
			[&]()
			{
				std::lock_guard<std::mutex> lock(this->m_mutex);
				this->m_socket.receive(this->m_packet);
				int dummy[] = { 0, ((void)(this->m_packet >> args), 0)... };
				this->m_packet.clear();
			}));
	};
private:
	sf::TcpSocket m_socket;
	std::vector<std::future<void>> m_future;
	std::atomic<bool> m_keepAlive = false;
	std::mutex m_mutex;
	sf::Packet m_packet;
	std::unordered_map<Network_MSG, void*> m_data;
};

class localhost
{
public:
	explicit localhost() noexcept = default;
	virtual ~localhost() noexcept { this->shutdown(); };

	inline const bool start() noexcept 
	{ 
		this->shouldRun = true;
		this->m_serverThread = std::make_unique<std::thread>(&localhost::localServer, this);
		return true;
	};

	inline const void shutdown() noexcept 
	{
		if (this->m_serverThread != nullptr)
		{
			this->shouldRun = false;
			this->m_serverThread->join();
		}
	};
private:
	std::unique_ptr<std::thread> m_serverThread;
	sf::TcpListener m_listener;
	sf::SocketSelector m_selector;
	std::vector<std::unique_ptr<sf::TcpSocket>> m_clients;
	std::atomic<bool> shouldRun = false;

	void localServer()
	{
		if (this->m_listener.listen(52200, sf::IpAddress::getLocalAddress()) != sf::Socket::Done)
			std::printf("Error: Cannot start local server!\n");
		else
		{
			std::printf("Success: Local server has been started on %s:%u\n", sf::IpAddress::getLocalAddress().toString().c_str(), this->m_listener.getLocalPort());
			this->m_selector.add(this->m_listener);

			while (this->shouldRun)
			{
				if (this->m_selector.wait(sf::microseconds(10)))
				{
					if (this->m_selector.isReady(this->m_listener))
					{
						std::unique_ptr<sf::TcpSocket> client = std::make_unique<sf::TcpSocket>();
						if (this->m_listener.accept(*client) == sf::Socket::Done)
						{
							std::printf("Player connected: %s:%u\n", client->getRemoteAddress().toString().c_str(), client->getRemotePort());
							this->m_clients.push_back(std::move(client));
							auto& newClient = (**(this->m_clients.end() - 1));
							this->m_selector.add(newClient);
						}
					}
					else
					{
						std::vector<std::vector<std::unique_ptr<sf::TcpSocket>>::iterator> disconnected;
						for (auto it = this->m_clients.begin(); it != this->m_clients.end(); ++it)
						{
							sf::TcpSocket& client = **it;
							if (this->m_selector.isReady(client))
							{
								sf::Packet packet;
								switch (client.receive(packet))
								{
								case sf::Socket::Disconnected:
									std::printf("Player disconnected: %s:%u\n", client.getRemoteAddress().toString().c_str(), client.getRemotePort());
									disconnected.push_back(it);
									this->m_selector.remove(client);
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
										//On real server we will store it in txt and undordered_map...
										if (temp_Un == std::string("admin") && temp_Pw == std::string("admin"))
										{
											packet << true;
											client.send(packet);
										}
										else
										{
											packet << false;
											client.send(packet);
										}
										break;
									}
									default:
									{
										for (auto it2 = this->m_clients.begin(); it2 != this->m_clients.end(); ++it2)
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
									std::printf("Error");
									break;
								case sf::Socket::NotReady:
									std::printf("NotReady");
									break;
								case sf::Socket::Partial:
									std::printf("Partial");
									break;
								default:
									std::printf("...");
									break;
								}
							}
						}
						for (auto it = disconnected.begin(); it != disconnected.end(); ++it)
							this->m_clients.erase(*it);
						disconnected.clear();
					}
				}
			}
		}
		std::printf("Shutting down the server!\n");
		this->m_clients.clear();
		this->m_selector.clear();
		this->m_listener.close();
	}
};