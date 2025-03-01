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
	//Sprite for zombie
	Sprite m_Sprite;
	//How fast can this one run
	float m_Speed;
	//How much health
	float m_Health;
	//Is it alive
	bool m_Alive;

public:
	//Handle bullet hitting zombie
	bool hit();
	//Find out if zombie is alive
	bool isAlive();
	//Spawn a new zombie
	void spawn(float startX, float startY, int type, int seed);
	//Return a rectangle that is the position in the world
	FloatRect getPosition();
	//Get a copy of the sprite to draw
	Sprite getSprite();
	//Update the zombie each frame
	void update(float elapsedTime, Vector2f playerLocation);
};



