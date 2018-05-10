/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CTimeMachineGame.h
* Description: Header for the CTimeMachineGame class.
* Represents the Time Machine game and contains all
* related game objects. Also draws the game board
* and grid, as well as calling draw() on all
* game objects.
* ==============================================
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "GameBoard.h"
#include "CSharedGameData.h"

static const int BOARD_WINDOW_PADDING = 40;
static const int BOARD_GRID_THICKNESS = 2;

static const sf::Keyboard::Key INPUT_SLOWDOWN = sf::Keyboard::Subtract;
static const sf::Keyboard::Key INPUT_SPEEDUP = sf::Keyboard::Add;

class CTimeMachineGame
{
private:
	CSharedGameData sharedData;
	sf::FloatRect boardRect;
	int boardSize;

	// User input tracking
	bool slowDownPressed;
	bool speedUpPressed;

	// Random number generator stuff
	std::random_device randDevice;
	std::mt19937 randEngine;
	std::uniform_int_distribution<> randDis;

	bool matchActive;
	void initRandom();
	bool createGameObjects();
	bool createToons();
	bool tryLoadResource(CGameObject* obj);
	void startToonThreads();
	void stopToonThreads();
	void drawGameBoard(sf::RenderWindow* renderWindow);
	void drawGameObjects(sf::RenderWindow* renderWindow);
	void processUserInput();
public:
	CTimeMachineGame(int windowWidth, int windowHeight);
	~CTimeMachineGame();

	// Game board render positioning stuff
	float getGridCellWidth();
	sf::FloatRect getBoardRect();
	sf::FloatRect getGridCellRect(int x, int y);

	sf::Vector2i getRandomBoardPos();
	void placeOnBoardRandom(CGameObject* object);
	void resizeBoardRect(int windowWidth, int windowHeight);
	bool isMatchActive();
	CSharedGameData* getSharedData();
	bool initNewGame();
	void update(sf::RenderWindow* renderWindow);
};

/* =========================================
*  End of File
*  =========================================
*/