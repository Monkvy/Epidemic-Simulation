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


void Cell::Infect(uint16_t incubation, uint16_t duration, int& total_infections)
{
	total_infections++;
	infected = true;
	this->incubation = incubation;
	this->duration = duration;
}


void Cell::Recover(float immunity, int& total_recovered)
{	
	total_recovered++;
	infected = false;
	incubation = 0;
	duration = 0;
	this->immunity += immunity;
}


void Cell::Die(int& total_deaths)
{
	total_deaths++;
	dead = true;
	infected = false;
	duration = 0;
}


bool Cell::Medicate(float medEffectiveness, int& total_medicated)
{
	total_medicated++;
	medicated = true;
	return Random::Rand() < medEffectiveness;
}


bool Cell::Quarantine(float qEffectiveness, int& total_quarantined)
{
	total_quarantined++;
	quarantined = true;
	return Random::Rand() < qEffectiveness;
}


void Cell::Update(float fatality, float immunity, int& total_deaths, int& total_recovered)
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
				Recover(immunity, total_recovered);
			else
				Die(total_deaths);
}
