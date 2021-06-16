#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

//Game engine class 
class Game
{
public:
	//Constructor
	Game();
	//Destructor
	virtual	~Game();

	//Accessors
	const bool GetWindowIsOpen() const;
	const bool GetEndGame() const;

	//Public Game Logic
	unsigned StoredPoints;

	//Functions
	void ResetGame();
	void UpdateEvents();
	void Update();
	void UpdatePoints();
	void UpdateMousePos();
	void UpdateText();
	void UpdateEnemies();

	void SpawnEnemy();

	void Render();
	void RenderPause(sf::RenderTarget& target);
	void RenderBackground(sf::RenderTarget& target);
	void RenderEnemies(sf::RenderTarget& target);
	void RenderText(sf::RenderTarget& target);

	void RenderButton(float x, float y, float w, float h, std::string& text, sf::RenderTarget& target);
	void InitButton();

	sf::Vector2f MousePosView;
private:
	//Varibles
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event ev;

	//Recources
	sf::Font Font;
	sf::Text uiText;
	sf::Text SplashText;

	//Textures
	sf::Texture CubeTexture;
	sf::Texture PauseTexture;
	sf::Texture BackgroundTexture;
	sf::Texture ButtonTexture;

	sf::SoundBuffer SoundBuffer;
	sf::Sound Sound;

	//Non visible functions
	void InitVariables();
	void InitWindow();
	void InitPause();
	void InitEnemy();
	void InitBackground();
	void InitFonts();
	void InitText();
	void InitTextures();
	void InitSounds();

	//Mouse positions
	sf::Vector2i MousePosWindow;

	//Game Logic
	unsigned Points;
	unsigned HighestPoints;

	int Health;

	float EnemySpawnTimer;
	float EnemySpawnTimerMax;
	int MaxEnemies;

	bool MouseHeld;
	bool EndGame;
	bool GamePaused;
	bool Music;

	//Game Objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;
	sf::RectangleShape Pause;
	sf::RectangleShape BackgroundShape;
	sf::RectangleShape ButtonShape;
};

