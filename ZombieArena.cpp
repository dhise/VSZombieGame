#include <sstream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include <iostream>
#include "Bullet.h"
#include "Pickup.h"


using namespace sf;


int main()
{
	//Instance of TextureHolder
	TextureHolder holder;

	//4 States the game will only ever be in
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

	//Start with GAME_OVER state
	State state = State::GAME_OVER;

	//Get Screen resolution and create SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombies", Style::Fullscreen);

	//Create an SFML View for main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//Clock for everything
	Clock clock;

	//How long has PLAYING state been active
	Time gameTimeTotal;

	//Mouse location in relation to world coordinates
	Vector2f mouseWorldPosition;
	//Mouse location in relation to screen coordinates
	Vector2i mouseScreenPosition;

	//Create player instance
	Player player;

	//Arena Boundaries
	IntRect arena;

	//Create the background
	VertexArray background;
	//Load the texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture("resources/graphics/background_sheet.png");

	//Prepare zombie horde

	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = NULL;
	//100 bullets should be fine
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	//When was the fire button pressed last
	Time lastPressed;
	//Hide the mouse pointer and replace it with a crosshair
	window.setMouseCursorVisible(true);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("resources/graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);
	Pickup healthPickup(1);
	Pickup ammoPickup(2);
	//About the game
	int score{ 0 };
	int hiScore{ 0 };
	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("resources/graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, 1920, 1080));
	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("resources/graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);
	// Load the font
	Font font;
	font.loadFromFile("resources/fonts/zombiecontrol.ttf");
	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");
	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");
	// LEVELING up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());
	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);
	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);
	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());
	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");
	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");
	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);
	//When did we last update HUD?
	int framesSinceLastHUDUpdate = 0;
	int fpsMeasurementFrameInterval = 1000;

	//Main game loop
	while (window.isOpen())
	{
		//Input shit
		//I think this is how you use polling i dont fucking know im tired dbl check next time game crashes
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				//Pause the game
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				//Restart while paused
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					//idk why but this is making it work so leave it so we dont get a frame jump
					clock.restart();
				}
				//Start a new game if we are in GAME_OVER state
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}
				if (state == State::PLAYING)
				{
					//Reloading
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							//Plent of ammo. Reload
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						}
						else if (bulletsSpare > 0)
						{
							//Only few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						}
						else
						{
							//Move here soon
						}
					}
				}
			}
		}//End event polling

		//Handle player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		//WASD handling
		if (state == State::PLAYING)
		{
			//Handle pressing and releasing of WASD
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			//Fire a bullet
			if (Mouse::isButtonPressed(sf::Mouse::Left))
			{
				//USE THIS FORMULAT FOR FURTHER SHOOT SPEED OF WEAPONS UI would say fire rate is = to 1000/fire rate   so if fire rate is 50,  1000 / 50 = 20. So the user would see Fire Rate: 20/second  

				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					//Pass the centre of the player and the center of the cross-hair to shoot function
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 90)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				}
			}
		}//End WASD input checking
		//Handle LEVELING_UP state
		if (state == State::LEVELING_UP)
		{
			//Handle the player leveling up
			if (event.key.code == Keyboard::Num1)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num2)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num3)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num4)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num5)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num6)
			{
				state = State::PLAYING;
			}
			if (state == State::PLAYING)
			{
				//Prepare the level
				//TODO change next two lines later
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;
				//Pass the vertex array by reference to the createBackground function
				int tileSize = createBackground(background, arena);
				//TODO change this later
				//int tileSize = 50;
				//Spawn player in middle of arena
				player.spawn(arena, resolution, tileSize);
				//Configure the pick-ups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);
				//Create a horde
				numZombies = 10;
				//Delete previously allocated memory
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				//reset clock to stop frame jumping idk if there is a way to do this other than always resetting clock
				clock.restart();
			}


		}//End leveling up

		//Update frame
		if (state == State::PLAYING)
		{
			//do delta time thing
			Time dt = clock.restart();

			//Update the total game time
			gameTimeTotal += dt;

			//Make a fraction of 1 from delta time
			float dtAsSeconds = dt.asSeconds();

			//Mouse pointer location
			mouseScreenPosition = Mouse::getPosition();
			//Now convert mouse position to world  based on coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
			//Set crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);

			//Update the player
			player.update(dtAsSeconds, Mouse::getPosition());
			//Make a note of new player position
			Vector2f playerPosition(player.getCenter());

			//Make the view centre around player
			mainView.setCenter(player.getCenter());

			//Loop through each zombie and update them
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}
			//Update any bullets that are in flight
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}
			//Update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);
			//Collision detection
			//Have any zombies been shot?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() && zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
						{
							//Stop the bullet
							bullets[i].stop();
							//Register the hit and see if it was a kill
							if (zombies[j].hit())
							{
								//Not just a hit but a kill too
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}
								numZombiesAlive--;
								//When all the zombies are dead(again)
								if (numZombiesAlive == 0)
								{
									state = State::LEVELING_UP;
								}

							}
						}
					}
				}
			}//End zombie being shot
			//Have any zombies touched player
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					if (player.hit(gameTimeTotal))
					{
						//More here later
					}
					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
					}
				}
			} //End player zombie collision
			//Has the player touched health pickup
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
			}
			//Has the player touched ammo pickup
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();
			}
			//Size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			//Increment the number of frames since previous update
			framesSinceLastHUDUpdate++;
			//recalculate every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				//Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				//Update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());
				//Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());
				//Update the high score text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());
				//Update the wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());
				//Update the high score text
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());
				framesSinceLastHUDUpdate = 0;
			}//End HUD update
			
		}//End updating the scene

		//Draw the scene
		if (state == State::PLAYING)
		{
			window.clear();
			//set the mainView to be displayed in the window and draw everything related
			window.setView(mainView);
			//Draw the background
			window.draw(background, &textureBackground);
			//Draw zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}
			//Draw player
			window.draw(player.getSprite());

			//Switch to the HUD view
			window.setView(hudView);
			//Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
			//Draw the pick ups if they are currently spawned
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}
			//Draw the crosshair
			window.draw(spriteCrosshair);
		}
		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);

		}
		window.display();
			

			
	
	}//End game loop
	//Delete any previously existing allocated memory
	delete[] zombies;


	return 0;
}