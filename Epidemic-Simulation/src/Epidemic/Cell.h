#pragma once
#include <SFML/Graphics.hpp>


struct Cell
{
	Cell(sf::Vector2u position, bool empty);
	~Cell();

	void Reset();
	void Infect(uint16_t incubation, uint16_t duration, int& total_infections);
	void Recover(float immunity, int& total_recovered);
	void Die(int& total_deaths);
	bool Medicate(float medEffectiveness, int& total_medicated);
	bool Quarantine(float qEffectiveness, int& total_quarantined);
	void Update(float fatality, float immunity, int& total_deaths, int& total_recovered);

	sf::Vector2u position;

	bool empty;
	bool infected = false;
	bool dead = false; 
	bool medicated = false;
	bool quarantined = false;

	uint16_t incubation = 0;
	uint16_t duration = 0;
	float immunity = 0.f;
};

