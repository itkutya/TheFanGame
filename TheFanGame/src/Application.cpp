#include "Application.h"

Application::Application(const std::string& title) noexcept : m_title(title)
{
	this->recreateWindow();

	ImGui::SFML::Init(this->m_window);
	this->s_StateManager.add<Menu>(this);

	this->m_BackgroundImage = this->s_ResourceManager.add<ResourceManager::Object>("BackgroundImage");
	if (this->m_BackgroundImage->Texture.loadFromFile("Resources/Backgrounds.png"))
	{
		this->m_BackgroundImage->Sprite.setTexture(this->m_BackgroundImage->Texture);
		this->m_BackgroundImage->Sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 1920, 1080 }));
		this->m_BackgroundImage->Sprite.setPosition(sf::Vector2f({ 0.f, 0.f }));
	}

	this->m_deltatime.restart();
}

Application::~Application() noexcept
{
	ImGui::SFML::Shutdown();
}

sf::RenderWindow& Application::getWindow() noexcept
{
	return this->m_window;
}

void Application::pollEvents() noexcept
{
	this->s_StateManager.processStateChange(this->m_window);

	sf::Event event;
	while (this->m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed)
			this->m_window.close();

		if (event.type == sf::Event::Resized)
			this->m_size = sf::VideoMode({ event.size.width, event.size.height });

		if (this->s_StateManager.getSize())
			this->s_StateManager.getCurrentState()->processEvent(event);
	}
}

void Application::update() noexcept
{
	const sf::Time dt = this->m_deltatime.restart();
	ImGui::SFML::Update(this->m_window, dt);
	constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | 
									   ImGuiWindowFlags_NoSavedSettings  |
									   ImGuiWindowFlags_NoBackground	 | 
									   ImGuiWindowFlags_NoDecoration     |
									   ImGuiWindowFlags_NoCollapse		 | 
									   ImGuiWindowFlags_NoInputs         |
									   ImGuiWindowFlags_NoMove;
	if (this->m_showfps)
	{
		if (ImGui::Begin("FPS Counter", 0, flags))
		{
			auto& io = ImGui::GetIO();
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetWindowPos("FPS Counter", ImVec2(viewport->WorkSize.x * 0.85f, viewport->WorkPos.y));
			ImGui::TextColored(ImVec4(0, 0, 1, 1), "%.2f fps (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
		}ImGui::End();
	}
	if (this->s_StateManager.getSize())
		this->s_StateManager.getCurrentState()->update(this->m_window, dt);
}

void Application::draw() noexcept
{
	this->m_window.clear();
	this->m_window.draw(this->m_BackgroundImage->Sprite);
	if (this->s_StateManager.getSize())
		this->s_StateManager.getCurrentState()->draw(this->m_window);
	ImGui::SFML::Render(this->m_window);
	this->m_window.display();
}

void Application::recreateWindow()
{
	this->m_window.create(this->m_size , this->m_title, this->m_fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	this->m_window.setFramerateLimit(this->m_fpslimit);
}