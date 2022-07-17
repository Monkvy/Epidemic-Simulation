#pragma once
#include <stdint.h>
#include <SFML/Graphics.hpp>


class BaseWindow
{
public:
	BaseWindow(const std::string& title, uint16_t width, uint16_t height, uint16_t maxFps = 144);
	~BaseWindow();

	inline virtual float getFps() final { return m_Fps; }
	virtual void Run(sf::Color fillColor = sf::Color::Black) final;

protected:
	virtual void HandleEvents(sf::Event event) {}
	virtual void Render() {}

	sf::RenderWindow p_Window;

private:
	float m_DeltaTime = 0.f;
	float m_Fps = 0.f;
};

