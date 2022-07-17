#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>


struct Cell
{
	Cell(sf::Vector2u position, bool empty);
	~Cell();

	void Reset();
	void Infect(uint16_t incubation, uint16_t duration, std::map<std::string, std::vector<int>>& stats, int t);
	void Recover(float immunity, std::map<std::string, std::vector<int>>& stats, int t);
	void Die(std::map<std::string, std::vector<int>>& stats, int t);
	bool Medicate(float medEffectiveness, std::map<std::string, std::vector<int>>& stats, int t);
	bool Quarantine(float qEffectiveness, std::map<std::string, std::vector<int>>& stats, int t);
	void Update(float fatality, float immunity, std::map<std::string, std::vector<int>>& stats, int t);

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

