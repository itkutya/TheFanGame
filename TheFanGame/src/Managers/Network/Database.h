#pragma once

#include <string>
#include <fstream>
#include <vector>

#include <SFML/Network.hpp>

class Database
{
public:
	Database(const char* link, const char* php, sf::Http::Request::Method method, const sf::Time timeout = sf::Time::Zero, std::string post = "") noexcept;
	~Database() noexcept = default;

	const sf::Http::Response::Status getStatus() noexcept;
	const std::string getData(const std::size_t index = 0) noexcept;
	const std::size_t getDataSize() noexcept;
private:
	const std::vector<std::string>& parseData() noexcept;
	
	sf::Http m_http;
	sf::Http::Request m_request;
	sf::Http::Response m_respone;
	std::vector<std::string> m_data;
};