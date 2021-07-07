#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <filesystem>
#include <fstream>

#include "../Buttons/button.hpp"
#include "../IniOptions/IniOptions.h"

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
	void UpdateClock();

	void FpsCounter();
	void SpawnEnemy();

	//Renderer
	void Render();
	void RenderGameScreens(sf::RenderTarget& target);
	void RenderParticles (sf::RenderTarget& target);
	void RenderBackground(sf::RenderTarget& target);
	void RenderEnemies(sf::RenderTarget& target);
	void RenderText(sf::RenderTarget& target);

	sf::Vector2f MousePosView;
	
	//Returns location of appdata and stores it in the char
	char* appdata = getenv("APPDATA");
private:
	//Variables
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event ev;

	//Recources
	sf::Font Font;
	sf::Text uiText;
	sf::Text SplashText;

	//Textures
	sf::Texture CubeTexture;
	sf::Texture ParticleTexture;
	sf::Texture PauseTexture;
	sf::Texture BackgroundTexture;

	//Sounds
	sf::SoundBuffer SoundBuffer;
	sf::Sound Sound;
	
	//Particle System
	sf::Clock clock;
	thor::ParticleSystem PSystem;
	thor::UniversalEmitter emitter;

	//Game Objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;
	sf::RectangleShape Pause;
	sf::RectangleShape BackgroundShape;

	//Mouse Position
	sf::Vector2i MousePosWindow;

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

	void SaveGame();

	//Game Logic
	unsigned Score;
	unsigned HighestPoints;

	int Health;
	int fps;

	float EnemySpawnTimer;
	float EnemySpawnTimerMax;
	int MaxEnemies;

	bool MouseHeld;
	bool EndGame;
	bool GamePaused;
	bool EnemyDeleted;
};

