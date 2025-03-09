#pragma once
#include <SFML/Graphics.hpp>


using namespace sf;

class Player
{
private:
	const float START_SPEED{ 200 };
	const float START_HEALTH{ 100 };

	//Player location
	Vector2f m_Position;
	//The sprite
	Sprite m_Sprite;
	//Texture for sprite
	Texture m_Texture;
	//Screen Resolution
	Vector2f m_Resolution;
	//Size of current arena
	IntRect m_Arena;
	//Size of each tile in arena
	int m_TileSize;
	//Which direction player is moving in
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	//Player health
	int m_Health;
	//Player max health
	int m_MaxHealth;
	//When was player last hit
	Time m_LastHit;
	//Speed in pixels per second
	float m_Speed;
	
public:
	Player();
	void spawn(IntRect arena, Vector2f Resolution, int tileSize);
	//Called at the end of every game
	void resetPlayerStats();

	//Handle player getting hit
	bool hit(Time timeHit);
	//How long ago was player last hit
	Time getLastHitTime();
	//Player location
	FloatRect getPosition();
	//Center of player
	Vector2f getCenter();
	//What angle is player facing
	float getRotation();
	//Send a copy of the sprite to main function
	Sprite getSprite();
	//Four move functions for player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	//Stop player moving in specific direction
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();
	//Call this function every frame
	void update(float elapsedTime, Vector2i mousePosition);
	//Give player speed boost
	void upgradeSpeed();
	//Give player health
	void upgradeHealth();
	//Increase max health of player
	void increaseHealthLevel(int amount);
	//How much health does player currently have
	int getHealth();


};