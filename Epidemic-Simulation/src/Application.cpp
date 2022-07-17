#include "Application.h"
#include <imgui.h>
#include <implot.h>
#include <implot_internal.h>

#include "Epidemic/Random.h"


Application::Application()
	: BaseWindow("Epidemic-Simulation", 1000, 1000), m_Epidemic(
		50, 0.7f, 300, 0.3f, 3, 4, 0.02f, 0.3f, 400, 0.0f, 400, 0.0f
	), m_ConfigRect{0.f, 0.f, 300.f, 1000.f}
{
	Random::Init();
	ImGui::GetStyle().ItemSpacing = { 0, 10 };
	ImGui::GetStyle().WindowPadding = { 10, 10 };
	ImGui::GetStyle().FramePadding = { 5, 5 };
	ImGui::GetStyle().ScrollbarSize = 10;

	m_Epidemic.Generate();
}


void Application::HandleEvents(sf::Event event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !m_GuiHovered && !m_ShowPlot)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(p_Window);
		size_t size = m_Epidemic.getCurrentSize();

		if (size > mousePos.x / 20 && size > mousePos.y / 20)
			m_Epidemic.InitialInfect({ (uint16_t)(mousePos.x / 20), (uint16_t)(mousePos.y / 20) });
	}
}


void Application::Render()
{
	m_Epidemic.DrawGrid(p_Window, 20);
	m_Epidemic.Step();


	ImGui::Begin("Configuration", nullptr);
	ImGui::SetWindowSize({ m_ConfigRect.width, m_ConfigRect.height });
	ImGui::SetWindowPos({ m_ConfigRect.left, m_ConfigRect.top });


	// Set m_GuiHovered
	sf::Vector2i mousePos = sf::Mouse::getPosition(p_Window);
	m_GuiHovered = m_ConfigRect.intersects({ (float)mousePos.x, (float)mousePos.y, 1, 1 });
	if (ImGui::IsWindowCollapsed())
		m_GuiHovered = false;


	// Buttons
	if (ImGui::Button("Generate", { m_ConfigRect.width - 20, 30 }))
	{
		m_Epidemic.Generate();
		m_Epidemic.running = false;
	}

	if (ImGui::Button("Reset", { m_ConfigRect.width - 20, 30 }))
	{
		m_Epidemic.Reset();
		m_Epidemic.running = false;
	}

	if (ImGui::Button("Run", { m_ConfigRect.width - 20, 30 }))
	{
		m_Epidemic.Reset();
		m_Epidemic.running = true;
	}


	// Set m_ShowPlot
	const char* showPlotTxt = m_ShowPlot ? "Hide Plot" : "Show Plot";
	if (ImGui::Button(showPlotTxt, { m_ConfigRect.width - 20, 30 }))
		m_ShowPlot = !m_ShowPlot;


	// Day & population
	std::string day_txt = "Day:" + std::to_string(m_Epidemic.t);
	std::string population_txt = "Population:" + std::to_string((int)pow(m_Epidemic.getCurrentSize(), 2));

	ImGui::Text(day_txt.c_str());
	ImGui::Text(population_txt.c_str());


	// Stats
	for (auto& [key, vec] : m_Epidemic.stats)
	{
		int totalCount = 0;
		for (int count : vec)
			totalCount += count;

		std::string txt = key + ":" + std::to_string(totalCount);
		ImGui::Text(txt.c_str());
	}


	// Inputs
	ImGui::SliderFloat("Density", &m_Epidemic.density, 0.f, 1.f);
	ImGui::SliderInt("Days", &m_Epidemic.days, 1, 365 * 4);
	ImGui::SliderFloat("Rate", &m_Epidemic.rate, 0.f, 1.f);
	ImGui::SliderInt("Incubation", &m_Epidemic.incubation, 0, 120);
	ImGui::SliderInt("Duration", &m_Epidemic.days, 1, 120);
	ImGui::SliderFloat("Fatality", &m_Epidemic.fatality, 0.f, 1.f);
	ImGui::SliderFloat("Immunity", &m_Epidemic.immunity, 0.f, 1.f);
	ImGui::SliderInt("Medicine at day", &m_Epidemic.medIntroduced, 0, m_Epidemic.days + 1);
	ImGui::SliderFloat("Medicine effectiveness", &m_Epidemic.medEffectiveness, 0.f, 1.f);
	ImGui::SliderInt("Quarantine at day", &m_Epidemic.qIntroduced, 0, m_Epidemic.days + 1);
	ImGui::SliderFloat("Quarantine effectiveness", &m_Epidemic.qEffectiveness, 0.f, 1.f);

	ImGui::End();


	// Plot
	if (m_ShowPlot && m_Epidemic.stats["infections"].size() == m_Epidemic.days)
	{
		ImPlot::BeginPlot("Graph");
		
		for (auto& [key, vec] : m_Epidemic.stats)
		{
			int* data = &vec[0];

			std::vector<int> timeline_vec;
			for (int d = 0; d <= m_Epidemic.days; d++)
				timeline_vec.push_back(d);

			int* timeline = &timeline_vec[0];

			ImPlot::PlotLine(key.c_str(), timeline, data, 300);
		}
		ImPlot::EndPlot();
	}
}
