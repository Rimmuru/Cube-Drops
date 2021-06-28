#include "Game.h"
#include "../Discord Integration/CubeDiscord.h"
#include <Thor/Math/Distributions.hpp>
#include <chrono>

//using namespaces because im lazy
using namespace sf;

//Constructor
Game::Game()
{
	InitVariables();
	InitWindow();
	InitFonts();
	InitText();
	InitSounds();
	InitTextures();
	InitPause();
	InitBackground();
	InitEnemy();

	SetUpDiscord();
}

//Destructor
Game::~Game()
{
	delete window;
}

//Accessor
//Check if window is running
const bool Game::GetWindowIsOpen() const
{
	return window->isOpen();
}

const bool Game::GetEndGame() const
{
	return EndGame;
}

void Game::ResetGame()
{
	/*
@return void

if GetEndGame is true
-resets the game to default values
-clears all enenmies on screen
*/

	if (GetEndGame()) {
		EndGame = false;
		EnemySpawnTimer = 0;
		Health = 100;
		Points = 0;
		enemies.clear();
		system.clearEmitters();
		system.clearParticles();
	}
}

void Game::UpdateEvents()
{
	while (window->pollEvent(ev)) {
		switch (ev.type)
		{
			//If window close button is pressed close window and shutdown the discord presence
		case Event::Closed:
			window->close();
			Discord_Shutdown();
			break;
			//Checks for key being pressed wasd ect
		case Event::KeyPressed:
			//If key is escape close window
			if (ev.key.code == Keyboard::Escape)
				GamePaused = !GamePaused;

			if (ev.key.code == Keyboard::R)
				ResetGame();
			if (ev.key.code == Keyboard::Q)
				window->close();
			break;
		}

	}
}

//Update Game window
void Game::Update()
{
	UpdateEvents();
	UpdateText();
	UpdateMousePos();
	UpdateClock();
	FpsCounter();

	//if game isnt paused and hasnt ended update enemies
	if (!GetEndGame() && !GamePaused)
		UpdateEnemies();

	//Endgame on 0 health and the update points
	if (Health <= 0) {
		EndGame = true;
		UpdatePoints();
	}
}

//Tracker for highest points and maybe more in future
void Game::UpdatePoints()
{
	if (Points > HighestPoints)
		HighestPoints = Points;
}

void Game::UpdateMousePos()
{
	/*
	@return void

	Updates positions on the window
	-Reletive to window (vect2i)
	*/

	MousePosWindow = Mouse::getPosition(*window);
	MousePosView = window->mapPixelToCoords(MousePosWindow);
}

void Game::UpdateClock()
{
	// Update particle system every .5 seconds
	if (clock.getElapsedTime().asMilliseconds() > 350 && !GamePaused) {

		system.update(clock.getElapsedTime());
		clock.restart();
	}

}

//Render stuffs to window
void Game::Render()
{
	/*
 @return void

 Renders the game objects that ill add later
 - clear old frame
 -render new object
 - Display frame in window

	  renders the game objects
  */
	window->clear();

	//Draw Game objects
	window->draw(enemy);
	window->draw(system);

	RenderBackground(*window);
	//RenderParticles(*window);
	RenderEnemies(*window);
	RenderText(*window);
	RenderGameScreens(*window);

	window->display();
}

void Game::UpdateEnemies()
{
	/*
	@return void

	Updates timer for enemy spawn and spawns enemeies,
	if the amount of enemies is less than the max allowed.
	if the spawtimer is less than the max reset the value.
	if the timer is less than max increase it in icraments of 5.f
	moves the enemies
	removes enemy thats add edge of window
	*/
	if (enemies.size() < MaxEnemies)
	{
		if (EnemySpawnTimer >= EnemySpawnTimerMax)
		{
			SpawnEnemy();
			EnemySpawnTimer = 0.f;
		}
		else
			EnemySpawnTimer += 5.f;
	}

	//Increase max enemies  TODO: when adding ini allow the option to change this value
	if (MaxEnemies < 100)
		MaxEnemies += 1;

	//Move the enemy
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].move(0.f, 2.f);

		/*
		Checks if the "enenmy" is past bottom of the window
		if player misses the block they lose 25 health.
		"health -25"
		*/
		if (enemies[i].getPosition().y > window->getSize().y - 51) {
			enemies.erase(enemies.begin() + i);
			Health -= 25;
		}
	}

	//Check for mouse click
	//if left mouse button click
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		if (!MouseHeld) {
			EnemyDeleted = false;
			MouseHeld = true;
			for (int i = 0; i < enemies.size() && !EnemyDeleted; i++)
			{
				if (enemies[i].getGlobalBounds().contains(MousePosView))
				{
					//Point system
					if (enemies[i].getFillColor() == Color::White)
						Points += 10;
					else if (enemies[i].getFillColor() == Color::Cyan)
						Points += 8;
					else if (enemies[i].getFillColor() == Color(132, 205, 235, 255))
						Points += 5;
					else if (enemies[i].getFillColor() == Color(60, 178, 225, 255))
						Points += 3;
					else if (enemies[i].getFillColor() == Color(234, 132, 220, 255))
						Points += 1;

					//Delete the enemy
					EnemyDeleted = true;
					enemies.erase(enemies.begin() + i);
				}
			}

		}
	}
	else
		MouseHeld = false;
}

void Game::RenderGameScreens(RenderTarget& target)
{
	//Pause Screen
	if (GamePaused)
	{
		gui::button MusicButton("Mute Music", Font, sf::Vector2f(videomode.width / 2, 150.f), gui::style::clean);
		gui::button Restart("Restart Game", Font, sf::Vector2f(videomode.width / 2, 200.f), gui::style::clean);
		gui::button Quit("Quit Game", Font, sf::Vector2f(videomode.width / 2, 250.f), gui::style::cancel);

		Restart.setColorHover(Color::Green);
		Restart.setColorTextClicked(Color::Green);

		target.draw(MusicButton);
		target.draw(Restart);
		target.draw(Quit);
		target.draw(Pause);

		MusicButton.update(ev, *window);
		Restart.update(ev, *window);
		Quit.update(ev, *window);

		system.clearEmitters();
		system.clearParticles();
	}

	if (EndGame) 
	{
		GamePaused = false;
		gui::button GameOver("Game Over", Font, sf::Vector2f(videomode.width / 2, 30.f), gui::style::cancel);
		gui::button Quit("Restart Game", Font, sf::Vector2f(videomode.width / 2, 250.f), gui::style::clean);

		target.draw(GameOver);
		target.draw(Quit);
	}
}

void Game::RenderParticles(sf::RenderTarget& target)
{
	emitter.setParticleColor(sf::Color(255, 0, 255));
	emitter.setEmissionRate(0.5);
	emitter.setParticlePosition(MousePosView);
	emitter.setParticleLifetime(sf::seconds(1));
	emitter.setParticleScale(sf::Vector2f(0.25, 0.25));
	emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(5), sf::seconds(7)));

	system.addEmitter(emitter);

	target.draw(system);
}

void Game::FpsCounter()
{
	float currentTime = clock.restart().asSeconds();
	fps = 1.f / currentTime;
}

void Game::RenderBackground(RenderTarget& target)
{
	target.draw(BackgroundShape);
}

//Spawn all the bad guys
void Game::SpawnEnemy()
{
	/*
	@return void

	Spawns enemies and sets their positions randomly on the window.
	-sets a random position on the window.
	-sets a random colour for the enenmy.
	-Adds the enemy to the vector of enemies
	*/

	enemy.setPosition(static_cast<float>(rand() % static_cast<int>(window->getSize().x - enemy.getSize().x)), 0.f);

	//Randomise enemy type
	int Type = rand() % 5;

	switch (Type)
	{
	case 0:
		enemy.setFillColor(Color::White);
		enemy.setSize(Vector2f(23.5f, 23.5f));
		break;
	case 1:
		enemy.setFillColor(Color(198, 125, 232));
		enemy.setSize(Vector2f(26.f, 26.f));
		break;
	case 2:
		enemy.setFillColor(Color(207, 148, 235));
		enemy.setSize(Vector2f(31.f, 31.f));
		break;
	case 3:
		enemy.setFillColor(Color(167, 123, 189));
		enemy.setSize(Vector2f(41.f, 41.f));
		break;
	case 4:
		enemy.setFillColor(Color(170, 120, 191));
		enemy.setSize(Vector2f(51.f, 51.f));
		break;
	default:
		enemy.setFillColor(Color::Red);
		enemy.setSize(Vector2f(51.f, 51.f));
		break;
	}

	//Spawns enemy
	enemies.push_back(enemy);
}

//Render the enemies on the screen
void Game::RenderEnemies(RenderTarget& target)
{
	for (auto& i : enemies)
	{
		target.draw(i);
	}
}

//Render the text on the screen
void Game::RenderText(RenderTarget& target)
{
	target.draw(uiText);
	target.draw(SplashText);
}

//Update the text when needed eg health change or point change
void Game::UpdateText()
{
	std::stringstream ss;
	std::stringstream splash;

	ss << "Fps: " << fps << std::endl;
	ss << "Highest Score: " << HighestPoints << std::endl;
	ss << "Score: " << Points << std::endl;
	ss << "Health: " << Health << "|100" << std::endl;

	uiText.setString(ss.str());
	SplashText.setString(splash.str());
}

//Initialise all the varibales
void Game::InitVariables()
{
	window = nullptr;

	Health = 100;
	Points = 0;
	HighestPoints = 0;
	StoredPoints = Points;
	EnemySpawnTimerMax = 170.f;
	EnemySpawnTimer = EnemySpawnTimerMax;
	MaxEnemies = 2;
	EndGame = false;
	MouseHeld = false;
	GamePaused = false;
	Music = true;
	EnemyDeleted = false;
}

//Initilise the window
void Game::InitWindow()
{
	videomode.height = 600;
	videomode.width = 800;

	window = new RenderWindow(VideoMode(videomode.width, videomode.height), "Cube Drops", Style::Titlebar | Style::Resize | Style::Close);

	window->setFramerateLimit(60);
}

void Game::InitPause()
{
	Pause.setPosition(Vector2f(videomode.width / 2.5, 10.f));
	Pause.setSize(Vector2f(90.f, 90.f));
	Pause.setScale(Vector2f(2.5f, 2.5f));
}

//Initilise the enemies
void Game::InitEnemy()
{
	enemy.setPosition(Vector2f(10.f, 10.f));
	enemy.setSize(Vector2f(50.f, 50.f));
}

void Game::InitBackground()
{
	BackgroundShape.setPosition(Vector2f(videomode.width - videomode.width, 0.f));
	BackgroundShape.setSize(Vector2f(200.f, 190.f));
	BackgroundShape.setScale(Vector2f(4.f, 4.f));
}

void Game::InitText()
{
	uiText.setFont(Font);
	uiText.setFillColor(Color::Black);
	uiText.setCharacterSize(23);
	uiText.setString("n/a");
	uiText.setPosition(videomode.width - videomode.width + 5, 5);

	SplashText.setFont(Font);
	SplashText.setFillColor(Color::Black);
	SplashText.setCharacterSize(39);
	SplashText.setPosition(videomode.width / 2, videomode.height / 3);
	SplashText.setString("n/a");
}

void Game::InitFonts()
{
	if (Font.loadFromFile("Fonts/Caveat-VariableFont_wght.ttf"))
		std::cout << "FOUND::Fonts/Caveat-VariableFont_wght\n";
	else
		throw std::exception("ERROR::FAILED_TO_LOAD_Caveat-VariableFont_wght\n");
}

void Game::InitTextures()
{
	//Cube texture
	if (CubeTexture.loadFromFile("Textures/cube2.png"))
		std::cout << "FOUND::Textures/Cube\n";
	else
		throw std::exception("ERROR::FAILED_TO_LOAD_Textures/Cube\n");

	//Particle texture
	if (ParticleTexture.loadFromFile("Textures/cloud.png"))
		std::cout << "FOUND::Textures/cloud\n";
	else
		throw std::exception("ERROR::FAILED_TO_LOAD_Textures/cloud\n");

	//Pause screen texture
	if (PauseTexture.loadFromFile("Textures/pause.png"))
		std::cout << "FOUND::Textures/Pause\n";
	else
		throw std::exception("ERROR::FAILED_TO_LOAD_Textures/pause\n");

	//Background
	if (BackgroundTexture.loadFromFile("Textures/background.jpg"))
		std::cout << "FOUND::Textures/Background\n";
	else
		throw std::exception("ERROR::FAILED_TO_LOAD_Textures/Cube\n");

	BackgroundShape.setTexture(&BackgroundTexture);
	Pause.setTexture(&PauseTexture);
	enemy.setTexture(&CubeTexture);
	system.setTexture(ParticleTexture);
}

void Game::InitSounds()
{
	if (SoundBuffer.loadFromFile("Sounds/NightShade.wav"))
		std::cout << "FOUND::Sounds/NightShade" << std::endl;
	else
		throw std::exception("ERROR::FAILED_TO_LOAD_Sounds/NightShade\n");

	if (Music)
		Sound.setVolume(70);
	else
		Sound.setVolume(0);

	Sound.setBuffer(SoundBuffer);
	Sound.play();
	Sound.setLoop(true);
}


