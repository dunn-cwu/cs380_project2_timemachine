/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CTimeMachineGame.cpp
* Description: Implementation for the CTimeMachineGame class.
* Represents the Time Machine game and contains all
* related game objects. Also draws the game board
* and grid, as well as calling draw() on all
* game objects.
* ==============================================
*/

#include <iostream>
#include "CTimeMachineGame.h"
#include "CMarvinMartian.h"

CTimeMachineGame::CTimeMachineGame(int windowWidth, int windowHeight) : sharedData(this), matchActive(false)
{
	boardSize = DEFAULT_BOARD_SIZE;

	// Ensures game board is the correct size for the render window
	resizeBoardRect(windowWidth, windowHeight);

	// Initialize random number generator for
	// randomizing toon/object positions on the
	// game board
	initRandom();
}


CTimeMachineGame::~CTimeMachineGame()
{
	stopToonThreads();

	sharedData.clear();
}

void CTimeMachineGame::initRandom()
{
	randEngine = std::mt19937(randDevice());
	randDis = std::uniform_int_distribution<>(0, boardSize - 1);
}

// Returns true if a game is currently being played
bool CTimeMachineGame::isMatchActive()
{
	return matchActive;
}

// Returns a pointer to the CSharedGameData object
// that is accessed by the toon threads.
CSharedGameData* CTimeMachineGame::getSharedData()
{
	return &sharedData;
}

// Returns the pixel rectangle of the game board
// on the render window
sf::FloatRect CTimeMachineGame::getBoardRect()
{
	return boardRect;
}

// Prepares a new game and starts toon threads
bool CTimeMachineGame::initNewGame()
{
	std::cout << "........ Initializing new game ...\n";

	sharedData.clear();

	if (!createGameObjects()) { return false; }
	
	sharedData.setGameActive(true);

	startToonThreads();

	return true;
}

// Creates the mountain, carrots, and toon objects
// then adds them randomly to the game board
bool CTimeMachineGame::createGameObjects()
{
	bool toonSucess = createToons();
	bool objSuccess = true;

	std::cout << "........ Creating mountain and carrots ...\n";

	std::unique_ptr<CGameObject> newMtn = std::make_unique<CGameObject>(&sharedData, MOUNTAIN_NAME, GameObjectType::Mountain, MOUNTAIN_TEXTURE);
	if (tryLoadResource(newMtn.get()))
	{
		sharedData.setMountain(newMtn);
		placeOnBoardRandom(sharedData.getMountain());
	}

	std::unique_ptr<CGameObject> newCarrotOne = std::make_unique<CGameObject>(&sharedData, CARROT_NAME, GameObjectType::Carrot, CARROT_TEXTURE);
	if (tryLoadResource(newCarrotOne.get()))
	{
		sharedData.setCarrot(0, newCarrotOne);
		placeOnBoardRandom(sharedData.getCarrot(0));
	}

	std::unique_ptr<CGameObject> newCarrotTwo = std::make_unique<CGameObject>(&sharedData, CARROT_NAME, GameObjectType::Carrot, CARROT_TEXTURE);
	if (tryLoadResource(newCarrotTwo.get()))
	{
		sharedData.setCarrot(1, newCarrotTwo);
		placeOnBoardRandom(sharedData.getCarrot(1));
	}

	return toonSucess && objSuccess;
}

// Creates the toon objects and places them randomly
// on the game board
bool CTimeMachineGame::createToons()
{
	std::cout << "........ Creating toons ...\n";

	std::unique_ptr<CToon> newBunny = std::make_unique<CToon>(&sharedData, BUNNY_NAME, BUNNY_TEXTURE);
	if (tryLoadResource(newBunny.get()))
	{
		sharedData.addToon(newBunny);
	}

	std::unique_ptr<CToon> newMarvin = std::make_unique<CMarvinMartian>(&sharedData, MARVIN_NAME, MARVIN_TEXTURE);
	if (tryLoadResource(newMarvin.get()))
	{
		sharedData.addToon(newMarvin);
	}

	std::unique_ptr<CToon> newTaz = std::make_unique<CToon>(&sharedData, TAZ_NAME, TAZ_TEXTURE);
	if (tryLoadResource(newTaz.get()))
	{
		sharedData.addToon(newTaz);
	}

	std::unique_ptr<CToon> newTweety = std::make_unique<CToon>(&sharedData, TWEETY_NAME, TWEETY_TEXTURE);
	if (tryLoadResource(newTweety.get()))
	{
		sharedData.addToon(newTweety);
	}

	int numToons = sharedData.getNumToons();

	for (int i = 0; i < numToons; i++)
	{
		placeOnBoardRandom(sharedData.getToon(i));
	}

	return true;
}

// Attempts to load the sprites for the given CGameObject
bool CTimeMachineGame::tryLoadResource(CGameObject* obj)
{
	if (!obj->loadResource())
	{
		std::cout << "........ Error loading resource for " << obj->getName() << std::endl;
		return false;
	}

	return true;
}

// Places the given CGameObject on a random unoccupied square
// on the game board
void CTimeMachineGame::placeOnBoardRandom(CGameObject* object)
{
	sf::Vector2i randPos;
	GameBoard* gameBoard = sharedData.getGameBoard();

	do
	{
		randPos = getRandomBoardPos();
	} while (gameBoard->get(randPos.x, randPos.y) != nullptr);

	object->setGridPosition(randPos);
}

// Returns a random x and y position contained on the game board
sf::Vector2i CTimeMachineGame::getRandomBoardPos()
{
	return sf::Vector2i(randDis(randEngine), randDis(randEngine));
}

// Starts all toon logic threads
void CTimeMachineGame::startToonThreads()
{
	std::cout << "........ Starting toon threads ...\n";

	int numToons = sharedData.getNumToons();

	for (int i = 0; i < numToons; i++)
	{
		sharedData.getToon(i)->startThread();
	}
}

// Stops all toon logic threads and joins them
void CTimeMachineGame::stopToonThreads()
{
	std::cout << "........ Stopping toon threads ...\n";

	int numToons = sharedData.getNumToons();

	for (int i = 0; i < numToons; i++)
	{
		sharedData.getToon(i)->stopAndJoinThread();
	}
}

// Resizes the game board pixel rectangle to fit within
// the game window
void CTimeMachineGame::resizeBoardRect(int windowWidth, int windowHeight)
{
	int boardSize = windowHeight - (BOARD_WINDOW_PADDING * 2);
	boardRect = sf::FloatRect((float)BOARD_WINDOW_PADDING, (float)BOARD_WINDOW_PADDING, (float)boardSize, (float)boardSize);
}

// Called by main game loop.
// Processes user input, waits for game winnder,
// and draws the game board and toons to the render window
void CTimeMachineGame::update(sf::RenderWindow* renderWindow)
{
	sharedData.lockMutex();

	processUserInput();

	CToon* winner = sharedData.getWinner();

	if (winner && sharedData.getGameActive())
	{
		sharedData.setGameActive(false);
		stopToonThreads();
		std::cout << winner->getName() << " has won the game.\n";
	}

	sharedData.unlockMutex();

	drawGameBoard(renderWindow);
	drawGameObjects(renderWindow);

	
}

// Draws the game board and grid lines
void CTimeMachineGame::drawGameBoard(sf::RenderWindow* renderWindow)
{
	sf::RectangleShape backRect = sf::RectangleShape(sf::Vector2f(boardRect.width, boardRect.height));
	backRect.setPosition(sf::Vector2f(boardRect.left, boardRect.top));
	backRect.setFillColor(sf::Color::White);

	renderWindow->draw(backRect);

	sf::RectangleShape horizontalLine = sf::RectangleShape(sf::Vector2f(boardRect.width, (float)BOARD_GRID_THICKNESS));
	horizontalLine.setFillColor(sf::Color::Black);
	
	sf::RectangleShape verticalLine = sf::RectangleShape(sf::Vector2f((float)BOARD_GRID_THICKNESS, boardRect.height));
	verticalLine.setFillColor(sf::Color::Black);

	float cellWidth = getGridCellWidth();

	for (int i = 0; i <= boardSize; i++)
	{
		horizontalLine.setPosition(boardRect.left, boardRect.top + (i * cellWidth));
		renderWindow->draw(horizontalLine);
	}

	for (int i = 0; i <= boardSize; i++)
	{
		verticalLine.setPosition(boardRect.left + (i * cellWidth), boardRect.top);
		renderWindow->draw(verticalLine);
	}
}

// Draws all toons/game objects on the game board
void CTimeMachineGame::drawGameObjects(sf::RenderWindow* renderWindow)
{
	GameBoard* gameBoard = sharedData.getGameBoard();
	int boardSize = gameBoard->getSize();

	for (int y = 0; y < boardSize; y++)
	{
		for (int x = 0; x < boardSize; x++)
		{
			CGameObject* gridObj = gameBoard->get(x, y);
			if (gridObj) { gridObj->draw(renderWindow); }
		}
	}
}

// Gets the width in pixels of a single grid square
// on the game board
float CTimeMachineGame::getGridCellWidth()
{
	return boardRect.width / boardSize;
}

// Returns the pixel rectangle of a specific grid square
// on the game board
sf::FloatRect CTimeMachineGame::getGridCellRect(int x, int y)
{
	float cellWidth = getGridCellWidth();

	return sf::FloatRect(boardRect.left + (x * cellWidth), boardRect.top + (y * cellWidth), cellWidth, cellWidth);
}

// Checks for user input to speed up/slow down the game
void CTimeMachineGame::processUserInput()
{
	if (!slowDownPressed && sf::Keyboard::isKeyPressed(INPUT_SLOWDOWN))
	{
		int newTurnWait = sharedData.getTurnWait();
		newTurnWait = (int)((float)newTurnWait * 1.5f);
		if (newTurnWait > 2000) { newTurnWait = 2000; }

		sharedData.setTurnWait(newTurnWait);
		slowDownPressed = true;

		std::cout << "........ Slowing down game\n";
	}
	else if (slowDownPressed && !sf::Keyboard::isKeyPressed(INPUT_SLOWDOWN))
	{
		slowDownPressed = false;
	}

	if (!speedUpPressed && sf::Keyboard::isKeyPressed(INPUT_SPEEDUP))
	{
		int newTurnWait = sharedData.getTurnWait();
		newTurnWait = (int)((float)newTurnWait / 1.5f);
		if (newTurnWait < 10) { newTurnWait = 10; }

		sharedData.setTurnWait(newTurnWait);
		speedUpPressed = true;

		std::cout << "........ Speeding up game\n";
	}
	else if (speedUpPressed && !sf::Keyboard::isKeyPressed(INPUT_SPEEDUP))
	{
		speedUpPressed = false;
	}
}

/* =========================================
*  End of File
*  =========================================
*/