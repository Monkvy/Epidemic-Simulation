#pragma once
#include <stdint.h>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Cell.h"


class Epidemic
{
public:
	static sf::Color defaultColor;
	static sf::Color infectedColor;
	static sf::Color deadColor;

	Epidemic(
		uint16_t size, float density,
		int days, float rate,
		int incubation, int duration,
		float fatality, float immunity,
		int medIntroduced, float medEffectiveness,
		int qIntroduced, float qEffectiveness
	);
	~Epidemic();

	void Generate();
	void Reset();
	void Step();
	void DrawGrid(sf::RenderWindow& window, float scale) const;

	void InitialInfect(sf::Vector2u position);
	inline size_t getCurrentSize() { return m_Cells.size(); }

	uint16_t size;
	int days;
	float density, rate;
	int incubation, duration;
	float fatality, immunity;
	int medIntroduced, qIntroduced;
	float medEffectiveness, qEffectiveness;

	bool running = false;
	int t = 0;
	int infections = 0;
	int deaths = 0;
	int recovered = 0; 
	int medicated = 0;
	int quarantined = 0;

private:
	std::vector<std::vector<Cell>> m_Cells;
	std::vector<sf::Vector2u> m_InitialInfected;
};

