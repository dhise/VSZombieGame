#include <SFML/Graphics.hpp>
#include "Player.h"
#include <iostream>


using namespace sf;


int main()
{

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

	//Main game loop
	while (window.isOpen())
	{
		//Input shit
		//I think this is how you use polling i dont fucking know im tired
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
				//TODO change this later
				int tileSize = 50;
				//Spawn player in middle of arena
				player.spawn(arena, resolution, tileSize);

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

			//Update the player
			player.update(dtAsSeconds, Mouse::getPosition());
			//Make a note of new player position
			Vector2f playerPosition(player.getCenter());

			//Make the view centre around player
			mainView.setCenter(player.getCenter());
		}//End updating 

		//Draw the scene
		if (state == State::PLAYING)
		{
			window.clear();
			//set the mainView to be displayed in the window and draw everything related
			window.setView(mainView);
			//Draw player
			window.draw(player.getSprite());
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



	return 0;
}