#include "Database.h"

Database::Database(const char* link, const char* php, sf::Http::Request::Method method, const sf::Time timeout, std::string post) noexcept
{
	this->m_http.setHost(link);
	this->m_request.setUri(php);
	this->m_request.setMethod(method);
	if (method == sf::Http::Request::Method::Post)
		this->m_request.setBody(post);
	this->m_respone = this->m_http.sendRequest(this->m_request, timeout);

	if (this->getStatus() != sf::Http::Response::Status::Ok)
		std::printf("Error while sending request to the database!\n Error:%s", this->m_respone.getBody().c_str());
	else
		this->parseData();
}

const sf::Http::Response::Status Database::getStatus() noexcept
{
	return this->m_respone.getStatus();
}

const std::string Database::getData(const std::size_t index) noexcept
{
	return this->m_data[index];
}

const std::size_t Database::getDataSize() noexcept
{
	return this->m_data.size();
}

const std::vector<std::string>& Database::parseData() noexcept
{
	this->m_data.clear();
	std::stringstream ss(this->m_respone.getBody());
	while (std::getline(ss, this->m_data.emplace_back(), '#'));
	return this->m_data;
}