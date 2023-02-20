#include "particleSystem.h"

particleEmitter::particleEmitter(const std::size_t& reserve) noexcept
{
	this->m_Particles.reserve(reserve);
	this->m_vertexBuffer.setUsage(sf::VertexBuffer::Stream);
	this->m_vertexBuffer.setPrimitiveType(sf::Triangles);

	if (sf::Shader::isAvailable())
	{
		const std::string fragmentShader = \
			"uniform vec2 center;"\
			"uniform float radius;"\
			"void main()"\
			"{"\
			"vec2 centerFromSfml = vec2(center.x, 1080 - center.y);"\
			"vec2 p = (gl_FragCoord.xy - centerFromSfml) / radius;"\
			"float r = sqrt(dot(p, p));"\
			"gl_FragColor = mix(gl_Color, vec4(0, 0, 0, 0), r);"\
			"}";

		if (!this->m_ParticleShader.loadFromMemory(fragmentShader, sf::Shader::Fragment))
			std::printf("Cannot load fragmentShader...\n");
	}
	else
		std::printf("Shaders are not available...\n");
	if (!this->m_ParticleTexture.create(sf::Vector2u(1920, 1080)))
		std::printf("Cannot create particleTexture...\n");
}

const void particleEmitter::createParticle(const std::size_t& amount, const float& lifeTime, const sf::Color& color, const sf::Vector2f& position, const sf::Vector2f& force) noexcept
{
	for (std::size_t i = 0; i < amount; ++i)
	{
		this->m_Particles.emplace_back(lifeTime, color, position, force);
		auto& newParticle = (*(this->m_Particles.end() - 1));
	}

	if (!this->m_vertexBuffer.create(this->m_Particles.size() * 6))
		std::printf("Could not alocate memory for vertex buffer!\n");
}

const void particleEmitter::update(const sf::Time& dt) noexcept
{
	sf::RenderStates state;
	state.blendMode = sf::BlendAdd;
	state.shader = &this->m_ParticleShader;

	this->m_ParticleTexture.clear(sf::Color::Transparent);
	for (auto it = this->m_Particles.begin(); it != this->m_Particles.end(); ++it)
	{
		auto& p = *it;
		float age = ((p.m_LifeTime - p.m_Clock.getElapsedTime().asSeconds()) / p.m_LifeTime);

		p.m_radius = 100.f * age;

		p.m_Vertecies[0].position = sf::Vector2f((p.m_Position.x - p.m_radius) + (dt.asSeconds() * p.m_Force.x), 
			(p.m_Position.y - p.m_radius) + (dt.asSeconds() * p.m_Force.y));
		p.m_Vertecies[1].position = sf::Vector2f((p.m_Position.x + p.m_radius) + (dt.asSeconds() * p.m_Force.x), 
			(p.m_Position.y - p.m_radius) + (dt.asSeconds() * p.m_Force.y));
		p.m_Vertecies[2].position = sf::Vector2f((p.m_Position.x + p.m_radius) + (dt.asSeconds() * p.m_Force.x), 
			(p.m_Position.y + p.m_radius) + (dt.asSeconds() * p.m_Force.y));
		p.m_Vertecies[3].position = sf::Vector2f((p.m_Position.x - p.m_radius) + (dt.asSeconds() * p.m_Force.x), 
			(p.m_Position.y + p.m_radius) + (dt.asSeconds() * p.m_Force.y));

		p.m_Position.x = p.m_Position.x + (dt.asSeconds() * p.m_Force.x);
		p.m_Position.y = p.m_Position.y + (dt.asSeconds() * p.m_Force.y);

		p.m_Vertecies[0].color = sf::Color((std::uint8_t)(p.m_Color.r * age),
			(std::uint8_t)(p.m_Color.g * age),
			(std::uint8_t)(p.m_Color.b * age),
			(std::uint8_t)(p.m_Color.a * age));
		p.m_Vertecies[1].color = sf::Color((std::uint8_t)(p.m_Color.r * age),
			(std::uint8_t)(p.m_Color.g * age),
			(std::uint8_t)(p.m_Color.b * age),
			(std::uint8_t)(p.m_Color.a * age));
		p.m_Vertecies[2].color = sf::Color((std::uint8_t)(p.m_Color.r * age),
			(std::uint8_t)(p.m_Color.g * age),
			(std::uint8_t)(p.m_Color.b * age),
			(std::uint8_t)(p.m_Color.a * age));
		p.m_Vertecies[3].color = sf::Color((std::uint8_t)(p.m_Color.r * age),
			(std::uint8_t)(p.m_Color.g * age),
			(std::uint8_t)(p.m_Color.b * age),
			(std::uint8_t)(p.m_Color.a * age));

		sf::Vertex vertecies[6] =
		{
			sf::Vertex(p.m_Vertecies[0]),
			sf::Vertex(p.m_Vertecies[1]),
			sf::Vertex(p.m_Vertecies[2]),
			sf::Vertex(p.m_Vertecies[2]),
			sf::Vertex(p.m_Vertecies[3]),
			sf::Vertex(p.m_Vertecies[0])
		};
		std::size_t i = std::distance(this->m_Particles.begin(), it) * 6;
		if (!this->m_vertexBuffer.update(vertecies, 6, (std::uint32_t)i))
			std::printf("Failed to update buffer!\n");

		//TODO:
		//Optimize it a bit more...
		this->m_ParticleShader.setUniform("center", sf::Glsl::Vec2(p.m_Position));
		this->m_ParticleShader.setUniform("radius", p.m_radius);
		this->m_ParticleTexture.draw(this->m_vertexBuffer, i, 6, state);
	}
	this->m_ParticleTexture.display();

	this->m_Particles.erase(
		std::remove_if(
			this->m_Particles.begin(),
			this->m_Particles.end(),
			[](particle const& p) { return p.m_Clock.getElapsedTime().asSeconds() >= p.m_LifeTime; }
		),
		this->m_Particles.end()
	);
}

const void particleEmitter::addForce(const sf::Vector2f& direction) noexcept
{
	for (auto& particle : this->m_Particles)
		particle.m_Force = direction;
}

const std::size_t particleEmitter::getSize() const noexcept
{
	return this->m_Particles.size();
}

const void particleEmitter::draw(sf::RenderTarget& target) noexcept
{
	if (this->m_Particles.size() > 0)
	{
		sf::Sprite final(this->m_ParticleTexture.getTexture());
		target.draw(final, sf::BlendAdd);
	}
}

void particleEmitter::draw(sf::RenderTarget& target, const sf::RenderStates& states) const {}