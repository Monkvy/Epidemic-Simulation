#include "Cell.h"
#include "Random.h"


Cell::Cell(sf::Vector2u position, bool empty)
	: position(position), empty(empty)
{
	
}


Cell::~Cell()
{

}


void Cell::Reset()
{
	infected = false;
	dead = false;
	medicated = false;
	quarantined = false;
	incubation = 0;
	duration = 0;
	immunity = 0.f;
}


void Cell::Infect(uint16_t incubation, uint16_t duration, std::map<std::string, std::vector<int>>& stats, int t)
{
	stats.find("infections")->second[t]++;
	infected = true;
	this->incubation = incubation;
	this->duration = duration;
}


void Cell::Recover(float immunity, std::map<std::string, std::vector<int>>& stats, int t)
{	
	stats.find("recovered")->second[t]++;
	infected = false;
	incubation = 0;
	duration = 0;
	this->immunity += immunity;
}


void Cell::Die(std::map<std::string, std::vector<int>>& stats, int t)
{
	stats.find("deaths")->second[t]++;
	dead = true;
	infected = false;
	duration = 0;
}


bool Cell::Medicate(float medEffectiveness, std::map<std::string, std::vector<int>>& stats, int t)
{
	stats.find("medicated")->second[t]++;
	medicated = true;
	return Random::Rand() < medEffectiveness;
}


bool Cell::Quarantine(float qEffectiveness, std::map<std::string, std::vector<int>>& stats, int t)
{
	stats.find("quarantined")->second[t]++;
	quarantined = true;
	return Random::Rand() < qEffectiveness;
}


void Cell::Update(float fatality, float immunity, std::map<std::string, std::vector<int>>& stats, int t)
{
	if (!infected || empty)
		return;

	if (incubation > 0)
		incubation--;
	else
		if (duration > 0)
			duration--;
		else
			if (Random::Rand() > fatality)
				Recover(immunity, stats, t);
			else
				Die(stats, t);
}
