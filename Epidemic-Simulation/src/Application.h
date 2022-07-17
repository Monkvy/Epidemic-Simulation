#pragma once
#include "BaseWindow.h"
#include "Epidemic/Epidemic.h"


class Application : public BaseWindow
{
public:
	Application();
	~Application() {}

private:
	void HandleEvents(sf::Event) override;
	void Render() override;

	Epidemic m_Epidemic;
	sf::Rect<float> m_ConfigRect;
	bool m_GuiHovered = false;
	bool m_ShowPlot = false;
};

