#include "BaseWindow.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <implot.h>


BaseWindow::BaseWindow(const std::string& title, uint16_t width, uint16_t height, uint16_t maxFps)
	: p_Window(sf::VideoMode({ width, height }), title, sf::Style::Close | sf::Style::Titlebar)
{
	p_Window.setFramerateLimit(maxFps);

	// Initialize everything
	ImGui::SFML::Init(p_Window);
	ImPlot::CreateContext();
	ImGui::GetIO().IniFilename = nullptr;
}


BaseWindow::~BaseWindow()
{
	ImGui::SFML::Shutdown();
	ImPlot::DestroyContext();
}


void BaseWindow::Run(sf::Color fillColor)
{
	sf::Clock deltaClock;
	while (p_Window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (p_Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				p_Window.close();

			ImGui::SFML::ProcessEvent(event);
			HandleEvents(event);
		}

		// Calculate delta time & fps
		sf::Time dtObj = deltaClock.restart();
		m_DeltaTime = dtObj.asSeconds();
		m_Fps = 1.0f / m_DeltaTime;

		// Update ImGui
		ImGui::SFML::Update(p_Window, dtObj);

		// Rendering
		p_Window.clear(fillColor);
		Render();

		ImGui::SFML::Render(p_Window);
		p_Window.display();
	}
}
