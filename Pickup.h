#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;


class Pickup
{
private:
	//Star value for health pickups
	const int HEALTH_START_VALUE{ 50 };
	const int AMMO_START_VALUE{ 12 };
	const int START_WAIT_TIME{ 10 };
	const int START_SECONDS_TO_LIVE{ 5 };

	//The sprite that represents the pickup
	Sprite m_Sprite;
	//The arena it exists in
	IntRect m_Arena;
	//How much is the pickup worth
	int m_Value;

	//What type of pick ups this
	//1 = health, 2 = ammo
	int m_Type;
	//Handle spawning and disappearing
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;
	
public:
	Pickup(int type);
	//Prepare a new pickup
	void setArena(IntRect arena);
	void spawn();
	//Check position of pickup
	FloatRect getPosition();
	//Get the sprite for drawing
	Sprite getSprite();
	//Let the pickup update itself each frame
	void update(float elapsedTime);
	//Is this pick up currently spawned
	bool isSpawned();
	//Get the item from pickup
	int gotIt();
	//Upgrade pick up value
	void upgrade();
};