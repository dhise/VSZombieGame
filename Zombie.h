#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Zombie
{
private:
	//How fast is each zombie type
	const float BLOATER_SPEED{ 40 };
	const float CHASER_SPEED{ 80 };
	const float CRAWLER_SPEED{ 20 };
	//How tough is each zombie type
	const float BLOATER_HEALTH{ 5 };
	const float CHASER_HEALTH{ 1 };
	const float CRAWLER_HEALTH{ 3 };
	//Vary zombie speed slightly
	const int MAX_VARRIANCE{ 30 };
	const int OFFSET{ 101 - MAX_VARRIANCE };
	//Zombie location
	Vector2f m_Position;
	//How fast can this one run
	float m_Speed;
	//How much health
	float m_Health;
	//Is it alive
	bool m_Alive;

	//Public todo
};