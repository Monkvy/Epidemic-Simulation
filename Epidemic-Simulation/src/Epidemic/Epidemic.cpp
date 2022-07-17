#include "Epidemic.h"
#include "Random.h"


sf::Color Epidemic::defaultColor{0, 150, 50};
sf::Color Epidemic::infectedColor{0, 255, 255};
sf::Color Epidemic::deadColor{255, 0, 0};


Epidemic::Epidemic(
	uint16_t size, float density,
	int days, float rate,
	int incubation, int duration,
	float fatality, float immunity,
	int medIntroduced, float medEffectiveness,
	int qIntroduced, float qEffectiveness
)
	: size(size), density(density), days(days), rate(rate), incubation(incubation), duration(duration),
	fatality(fatality), immunity(immunity), medIntroduced(medIntroduced), qIntroduced(qIntroduced),
	medEffectiveness(medEffectiveness), qEffectiveness(qEffectiveness)
{
	// Run();
}


Epidemic::~Epidemic()
{

}


void Epidemic::Generate()
{
	m_Cells.clear();
	m_InitialInfected.clear();

	for (int r = 0; r < size; r++)
	{
		std::vector<Cell> row;
		for (int c = 0; c < size; c++)
		{
			bool empty = Random::Rand() >= density;
			row.push_back(Cell({ (uint16_t)c, (uint16_t)r }, empty));
		}
		m_Cells.push_back(row);
	}
}


void Epidemic::Reset()
{
	t = 0;

	for (auto& row : m_Cells)
	{
		for (Cell& cell : row)
		{
			bool initialInfected = false;
			for (auto& position : m_InitialInfected)
				if (cell.position.x == position.x && cell.position.y == position.y)
					initialInfected = true;

			cell.Reset();
			if (initialInfected)
				cell.Infect(incubation, duration, infections);
		}
	}
}


void Epidemic::InitialInfect(sf::Vector2u position)
{
	m_InitialInfected.push_back(position);
	m_Cells[position.y][position.x].Infect(incubation, duration, infections);
}


void Epidemic::Step()
{
	if (t >= days || m_InitialInfected.size() == 0)
		running = false;

	if (!running)
		return;
	
	for (auto& row : m_Cells)
	{
		for (Cell& cell : row)
		{
			cell.Update(fatality, immunity, deaths, recovered);

			if (!cell.infected || cell.incubation > 0)
				continue;

			// Medicate
			if (!cell.medicated && t > medIntroduced)
			{
				if (cell.Medicate(medEffectiveness, medicated))
				{
					recovered++;
					cell.Recover(immunity, recovered);
					continue;
				}
			}

			// Quarantine
			if (!cell.quarantined && t > qIntroduced)
				if (cell.Quarantine(qEffectiveness, quarantined))
					continue;

			// Infect neighbors
			std::vector<std::reference_wrapper<Cell>> neighbors;

			if (cell.position.x > 0)
				neighbors.push_back(std::ref(m_Cells[cell.position.y][cell.position.x - 1]));	// Left
			if ((int)cell.position.x < size - 1)
				neighbors.push_back(std::ref(m_Cells[cell.position.y][cell.position.x + 1]));	// Right

			if (cell.position.y > 0)
				neighbors.push_back(std::ref(m_Cells[cell.position.y - 1][cell.position.x]));	// Above
			if ((int)cell.position.y < size - 1)
				neighbors.push_back(std::ref(m_Cells[cell.position.y + 1][cell.position.x]));	// Under

			for (Cell& neighbor : neighbors)
			{
				if (neighbor.infected || neighbor.empty)
					continue;

				if (Random::Rand() > neighbor.immunity && Random::Rand() < rate)
				{
					neighbor.Infect(incubation, duration, infections);
				}
			}
		}
	}
	t++;
}


void Epidemic::DrawGrid(sf::RenderWindow& window, float scale) const
{
	for (auto& row : m_Cells)
	{
		for (Cell cell : row)
		{
			sf::Color color = Epidemic::defaultColor;

			if (cell.empty)
				continue;
			if (cell.infected)
				color = Epidemic::infectedColor;
			if (cell.dead)
				color = Epidemic::deadColor;
			
			sf::RectangleShape rect({ scale, scale });
			rect.setPosition({ cell.position.x * scale, cell.position.y * scale });
			rect.setFillColor(color);

			window.draw(rect);
		}
	}
}