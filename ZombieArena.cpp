#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include <iostream>
#include "Bullet.h"



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
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("resources/graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 5);
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
		}//End updating 

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
			//Draw the crosshair
			window.draw(spriteCrosshair);
		}
		if (state == State::LEVELING_UP)
		{

		}
		if (state == State::PAUSED)
		{

		}
		if (state == State::GAME_OVER)
		{

		}
		window.display();
			

			
	
	}//End game loop
	//Delete any previously existing allocated memory
	delete[] zombies;


	return 0;
}