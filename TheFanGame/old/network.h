#pragma once

#include "includes.h"

enum class Network_MSG
{
	None, Error, Test, Count
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
			std::uint32_t type = 0;
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
			default:
				break;
			}
			this->m_packet.clear();
		}
	};

	template<typename T>
	inline const T* getData(const Network_MSG& msg) noexcept
	{
		std::lock_guard<std::mutex> lock(this->m_mutex);
		if (this->m_data[msg] != nullptr)
			return static_cast<T*>(this->m_data[msg]);
		else
			return static_cast<T*>(nullptr);
	};

	inline const std::future<void>& keepAlive(const std::int64_t& thickRate) noexcept
	{
		return this->m_future.emplace_back(std::async(std::launch::async,
			[&]()
			{
				this->m_keepAlive = true;
				sf::SocketSelector selector;
				selector.add(this->m_socket);
				while (this->m_keepAlive)
				{
					if (selector.wait(sf::microseconds(thickRate)))
					{
						std::lock_guard<std::mutex> lock(this->m_mutex);
						if (selector.isReady(this->m_socket))
						{
							switch (this->m_socket.receive(this->m_packet))
							{
							case sf::Socket::Status::Done:
								std::printf("Done\n");
								break;
							case sf::Socket::Status::NotReady:
								std::printf("NotReady\n");
								break;
							case sf::Socket::Status::Partial:
								std::printf("Partial\n");
								break;
							case sf::Socket::Status::Disconnected:
							{
								this->m_keepAlive = false;
								this->m_socket.disconnect();
								break;
							}
							case sf::Socket::Status::Error:
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

	inline const bool join(const sf::IpAddress& ip = sf::IpAddress::getLocalAddress().value(), const std::uint16_t& port = 52200, const sf::Time& timeout = sf::Time::Zero) noexcept
	{
		this->disconnect();
		if (this->m_socket.connect(ip, port, timeout) == sf::Socket::Status::Done)
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

	template<typename T, typename... Args>
	inline const std::future<void>& send(T& firstParam, Args&... otherParams) noexcept
	{
		return this->m_future.emplace_back(std::async(std::launch::async,
			[&](T firstArg, Args... otherArg)
			{
				std::lock_guard<std::mutex> lock(this->m_mutex);
				this->makeSendPacket(firstArg, otherArg...);
				auto status = this->m_socket.send(this->m_packet);
				this->m_packet.clear();
			}, firstParam, otherParams...));
	};

	template<typename T, typename... Args>
	inline const std::future<void>& send(const T& firstParam, const Args&... otherParams) noexcept
	{
		return this->m_future.emplace_back(std::async(std::launch::async,
			[&](T firstArg, Args... otherArg)
			{
				std::lock_guard<std::mutex> lock(this->m_mutex);
				this->makeSendPacket(firstArg, otherArg...);
				auto status = this->m_socket.send(this->m_packet);
				this->m_packet.clear();
			}, firstParam, otherParams...));
	};

	template<typename T, typename... Args>
	inline const std::future<void>& receive(T& firstParam, Args&... otherParams) noexcept
	{
		return this->m_future.emplace_back(std::async(std::launch::async,
			[&]()
			{
				std::lock_guard<std::mutex> lock(this->m_mutex);
				auto status = this->m_socket.receive(this->m_packet);
				this->makeReceivePacket(firstParam, otherParams...);
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

	template<typename T, typename... Args>
	inline constexpr void makeSendPacket(T& firstParam, Args&... otherParams)
	{
		this->m_packet << firstParam;
		if constexpr (sizeof...(otherParams) > 0)
			this->makeSendPacket(otherParams...);
	};

	template<typename T, typename... Args>
	inline constexpr void makeReceivePacket(T& firstParam, Args&... otherParams)
	{
		this->m_packet >> firstParam;
		if constexpr (sizeof...(otherParams) > 0)
			this->makeReceivePacket(otherParams...);
	};
};

class localhost
{
public:
	explicit localhost() noexcept = default;
	virtual ~localhost() noexcept { this->shutdown(); };

	inline const bool start() noexcept 
	{ 
		this->shouldRun = true;
		this->m_serverThread = std::thread(&localhost::localServer, this);
		return true;
	};

	inline const void shutdown() noexcept 
	{	
		this->shouldRun = false;
		if (this->m_serverThread.joinable())
			this->m_serverThread.join();
	};
private:
	std::thread m_serverThread;
	sf::TcpListener m_listener;
	sf::SocketSelector m_selector;
	std::vector<std::unique_ptr<sf::TcpSocket>> m_clients;
	std::atomic<bool> shouldRun = false;

	void localServer()
	{
		if (this->m_listener.listen(52200, sf::IpAddress::getLocalAddress().value()) != sf::Socket::Status::Done)
			std::printf("Error: Cannot start local server!\n");
		else
		{
			std::printf("Success: Local server has been started on %s:%u\n", sf::IpAddress::getLocalAddress().value().toString().c_str(), this->m_listener.getLocalPort());
			this->m_selector.add(this->m_listener);

			while (this->shouldRun)
			{
				if (this->m_selector.wait(sf::microseconds(10)))
				{
					if (this->m_selector.isReady(this->m_listener))
					{
						std::unique_ptr<sf::TcpSocket> client = std::make_unique<sf::TcpSocket>();
						if (this->m_listener.accept(*client) == sf::Socket::Status::Done)
						{
							std::printf("Player connected: %s:%u\n", client->getRemoteAddress().value().toString().c_str(), client->getRemotePort());
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
								case sf::Socket::Status::Disconnected:
									std::printf("Player disconnected: %s:%u\n", client.getRemoteAddress().value().toString().c_str(), client.getRemotePort());
									disconnected.push_back(it);
									this->m_selector.remove(client);
									client.disconnect();
									break;
								case sf::Socket::Status::Done:
								{
									for (auto it2 = this->m_clients.begin(); it2 != this->m_clients.end(); ++it2)
									{
										sf::TcpSocket& client2 = **it2;
										if (it2 != it)
										{
											auto status = client.send(packet);
											if (status != sf::Socket::Status::Done)
												std::printf("Packet error...\n");
										}
									}
								}
								break;
								case sf::Socket::Status::Error:
									std::printf("Error");
									break;
								case sf::Socket::Status::NotReady:
									std::printf("NotReady");
									break;
								case sf::Socket::Status::Partial:
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

class request
{
public:
	request(const std::string& server, const std::string& php) noexcept
	{
		this->m_request.setMethod(sf::Http::Request::Method::Post);
		this->m_request.setUri(php);
		this->m_http.setHost(server);
	};
	virtual ~request() = default;

	template<typename T, typename... Args>
	[[nodiscard]] inline constexpr std::ostringstream setParams(const T& firstParam, const Args&... otherParams) noexcept
	{
		std::ostringstream stream;
		stream << firstParam;
		if constexpr (sizeof...(otherParams) > 0)
			stream << this->setParams(otherParams...).str();
		return stream;
	};

	//TODO:
	//Make this threaded so it won't block main thread...
	const bool sendRequest(const std::string& stream, const sf::Time& timeout = sf::Time::Zero) noexcept
	{
		this->m_request.setBody(stream);
		this->m_response = this->m_http.sendRequest(this->m_request, timeout);
		//For testing leave it here
		//Sometimes it just error's???
		std::printf("%zu", static_cast<std::size_t>(this->m_response.getStatus()));
		if (this->m_response.getStatus() == sf::Http::Response::Status::Ok)
			return true;
		return false;
	};

	[[nodiscard]] const sf::Http::Response& getResponse() const noexcept
	{
		return this->m_response;
	};
private:
	sf::Http::Request m_request;
	sf::Http m_http;
	sf::Http::Response m_response;
};