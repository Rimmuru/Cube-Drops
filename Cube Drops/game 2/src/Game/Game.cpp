#include "Game.h"

#include "../Discord Integration/CubeDiscord.h"

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

	//if game isnt paused and hasnt ended update 
	if (!EndGame && !GamePaused)
		UpdateEnemies();

	//Endgame on 0 health and only update our points then
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

	RenderBackground(*window);
	RenderEnemies(*window);
	RenderText(*window);
	RenderPause(*window);
	window->display();
}

void Game::RenderPause(RenderTarget& target)
{
	if (GamePaused) {

		target.draw(Pause);
	}

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
		enemy.setSize(Vector2f(22.5f, 22.5f));
		break;
	case 1:
		enemy.setFillColor(Color::Cyan);
		enemy.setSize(Vector2f(25.f, 25.f));
		break;
	case 2:
		enemy.setFillColor(Color(132, 205, 235, 255));
		enemy.setSize(Vector2f(30.f, 30.f));
		break;
	case 3:
		enemy.setFillColor(Color(60, 178, 225, 255));
		enemy.setSize(Vector2f(40.f, 40.f));
		break;
	case 4:
		enemy.setFillColor(Color(234, 132, 220, 255));
		enemy.setSize(Vector2f(50.f, 50.f));
		break;
	default:
		enemy.setFillColor(Color::Red);
		enemy.setSize(Vector2f(50.f, 50.f));
		break;

	}
	//enemy.setFillColor(Color::Magenta);

	//Spawns enemy
	enemies.push_back(enemy);
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
	if (Points < INT_MAX && MaxEnemies < 100)
		MaxEnemies += 0.5;

	//Move the enemy
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].move(0.f, 2.f);

		/*
		Checks if the "enenmy" is past bottom of the window
		if player misses the block they lose 25 health.
		"health -25"
		*/
		if (enemies[i].getPosition().y > window->getSize().y - 47) {
			enemies.erase(enemies.begin() + i);
			Health -= 25;
		}
	}

	//Check for mouse click
	//if left mouse button click
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		if (!MouseHeld) {
			bool deleted = false;
			MouseHeld = true;
			for (int i = 0; i < enemies.size() && !deleted; i++)
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
					deleted = true;
					enemies.erase(enemies.begin() + i);
				}
			}

		}
	}
	else
		MouseHeld = false;
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

void Game::RenderButton(float x, float y, float w, float h, std::string& text, sf::RenderTarget& target)
{

}

//Update the text when needed eg health change or point change
void Game::UpdateText()
{
	std::stringstream ss;
	std::stringstream splash;
	ss << "Highest Points: " << HighestPoints << std::endl;
	ss << "Points: " << Points << std::endl;
	ss << "Health: " << Health << std::endl;
	ss << "\n\nTesting:\nMaxEnemies: " << MaxEnemies << std::endl;
	ss << "Enemies: " << enemies.size() << std::endl;

	if (GetEndGame())
	{
		splash << "Aww Shucks you failed\nPress R to restart\nPress Q to quit";
		GamePaused = false;
	}

	uiText.setString(ss.str());
	SplashText.setString(splash.str());
}

void Game::InitButton()
{
	if (ButtonTexture.loadFromFile("Textures/pauseBack.png"))
		std::cout << "Found font file\n";
	else
		throw "ERROR::FAILED_TO_LOAD_pauseBack\n";

	if (ButtonTexture.loadFromFile("Textures/pauseBack.png"))
		std::cout << "Found font file\n";
	else
		throw "ERROR::FAILED_TO_LOAD_pauseBack\n";

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
	MaxEnemies = 20;
	EndGame = false;
	MouseHeld = false;
	GamePaused = false;
	Music = true;
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
	enemy.setOutlineColor(Color::White);
	enemy.setOutlineThickness(1.5f);
}

void Game::InitBackground()
{
	BackgroundShape.setPosition(Vector2f(videomode.width - videomode.width, 0.f));
	BackgroundShape.setSize(Vector2f(200.f, 190.f));
	BackgroundShape.setScale(Vector2f(4.f, 4.f));
}

void Game::InitFonts()
{
	if (Font.loadFromFile("Fonts/Caveat-VariableFont_wght.ttf"))
		std::cout << "Found font file\n";
	else
		throw "ERROR::FAILED_TO_LOAD_Caveat-VariableFont_wght\n";
}

void Game::InitText()
{
	uiText.setFont(Font);
	uiText.setFillColor(Color::Black);
	uiText.setCharacterSize(23);
	uiText.setString("n/a");

	SplashText.setFont(Font);
	SplashText.setFillColor(Color::Black);
	SplashText.setCharacterSize(39);
	SplashText.setPosition(videomode.width / 2, videomode.height / 3);
	SplashText.setString("n/a");
}

void Game::InitTextures()
{
	//Cube texture
	if (CubeTexture.loadFromFile("Textures/Cube.jpg"))
		std::cout << "Found cube image\n";
	else
		throw "ERROR::FAILED_TO_LOAD_Textures/Cube\n";

	//Pause screen texture
	if (PauseTexture.loadFromFile("Textures/pause.png"))
		std::cout << "Found pause image\n";
	else
		throw "ERROR::FAILED_TO_LOAD_Textures/pause\n";

	//Background
	if (BackgroundTexture.loadFromFile("Textures/background.jpg"))
		std::cout << "Found Background image\n";
	else
		throw "ERROR::FAILED_TO_LOAD_Textures/Cube\n";

	BackgroundShape.setTexture(&BackgroundTexture);
	Pause.setTexture(&PauseTexture);
	enemy.setTexture(&CubeTexture);
}

void Game::InitSounds()
{
	if (SoundBuffer.loadFromFile("Sounds/NightShade.wav"))
		std::cout << "Found sound file" << std::endl;
	else
		throw "ERROR::FAILED_TO_LOAD_Sounds/NightShade\n";

	if (Music)
		Sound.setVolume(70);
	else
		Sound.setVolume(0);

	Sound.setBuffer(SoundBuffer);
	Sound.play();
	Sound.setLoop(true);
}


