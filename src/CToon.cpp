/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CToon.cpp
* Description: Implementation for the CToon class.
* Represents one of the four toons. Serves as a
* base class for CMarvinMartian.
* ==============================================
*/

#include <system_error>
#include <iostream>
#include "CSharedGameData.h"
#include "CToon.h"
#include "CTimeMachineGame.h"

CToon::CToon() :
	CGameObject(nullptr, "(none)", GameObjectType::Toon, ""), charThread(), killThread(true), clock(), elapsedTime(), hasCarrot(false)
{
	// Prepare random number gen for toon movements
	initRandom();
}

CToon::CToon(CSharedGameData* sharedData, std::string name, std::string spriteFile) :
	CGameObject(sharedData, name, GameObjectType::Toon, spriteFile), charThread(), killThread(true), clock(), elapsedTime(), hasCarrot(false)
{
	// Prepare random number gen for toon movements
	initRandom();
}

CToon::~CToon()
{
	stopAndJoinThread();
}

void CToon::initRandom()
{
	randEngine = std::mt19937(randDevice());
	randDis = std::uniform_int_distribution<>(0, 3);
}

// Loads required sprites for the toon
bool CToon::loadResource()
{
	bool success = CGameObject::loadResource();

	if (success)
	{
		// Load carrot image. Displayed when toon has carrot
		success = carrotTexture.loadFromFile(CARROT_TEXTURE);
		carrotSpriteRect.setTexture(&carrotTexture, true);
	}

	return success;
}

// Draws toon to the correct position on the render window
void CToon::draw(sf::RenderWindow* renderWindow)
{
	CGameObject::draw(renderWindow);

	// If toon has carrot, draw carrot on top left of toon image
	if (hasCarrot)
	{
		sf::FloatRect carrotDrawRect = sharedData->getGame()->getGridCellRect(gridPos.x, gridPos.y);
		carrotDrawRect.width /= 2;
		carrotDrawRect.height /= 2;

		carrotSpriteRect.setPosition(carrotDrawRect.left, carrotDrawRect.top);
		carrotSpriteRect.setSize(sf::Vector2f(carrotDrawRect.width, carrotDrawRect.height));

		renderWindow->draw(carrotSpriteRect);
	}
}

// Returns a random direction integer from [0-3]
int CToon::getRandomDirection()
{
	return randDis(randEngine);
}

// Returns a random adjacent position to the toon
// for the next potential move. Position is validated
// to be on the board
sf::Vector2i CToon::getRandomNewPos()
{
	sf::Vector2i newPos;

	do
	{
		newPos = sf::Vector2i(gridPos);

		switch (getRandomDirection())
		{
		case 0:
			newPos.x -= 1;
			break;
		case 1:
			newPos.y -= 1;
			break;
		case 2:
			newPos.x += 1;
			break;
		case 3:
			newPos.y += 1;
			break;
		}
	} while (!isValidBoardPosition(newPos));

	return newPos;
}

// Checks if the given position is contained on the game board
bool CToon::isValidBoardPosition(sf::Vector2i newPos)
{
	int boardSize = sharedData->getGameBoard()->getSize();

	return ((newPos.x >= 0 && newPos.x < boardSize) &&
		(newPos.y >= 0 && newPos.y < boardSize));
}

// Returns true if the toon has the carrot
bool CToon::getHasCarrot()
{
	return hasCarrot;
}

// Returns true of the toon's thread is running
// or has not been joined yet
bool CToon::isRunning()
{
	return charThread.joinable();
}

// Starts the toon's independent logic thread
bool CToon::startThread()
{
	std::lock_guard<std::mutex> guard(runningThreadMutex);

	if (isRunning()) { return false; }

	try
	{
		killThread = false;

		// Create new thread that executes run() method
		charThread = std::thread(&CToon::run, this);

		return true;
	}
	catch (std::system_error ex)
	{
		return false;
	}
}

// Notifies toon thread to stop and waits for it to join
bool CToon::stopAndJoinThread()
{
	std::lock_guard<std::mutex> guard(runningThreadMutex);

	if (!isRunning())
	{
		return false;
	}

	setKillThread(true);

	std::cout << "........ Joining thread for " << name << " ...\n";
	charThread.join();
	return true;
}

// Returns true if the killThread flag has been set
bool CToon::getKillThread()
{
	std::lock_guard<std::mutex> guard(killThreadMutex);

	return killThread;
}

// Sets the killThread flag. Notifies toon thread
// that it should finish
void CToon::setKillThread(bool kill)
{
	std::lock_guard<std::mutex> guard(killThreadMutex);

	killThread = kill;
}

// Executed by the spawned thread.
// Runs the toon's main loop.
void CToon::run()
{
	// Loop until killThread == true or the game has ended
	while (getKillThread() == false && sharedData->getGameActive())
	{
		elapsedTime = clock.getElapsedTime();

		// Wait a number of miliseconds between each turn
		if (elapsedTime.asMilliseconds() >= sharedData->getTurnWait())
		{
			clock.restart();
			tick();
		}

		// Lower cpu usage, give other threads a chance to lock mutex
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::cout << "........ Thread for " << name << " has completed.\n";
}

// Executes a toons turn.
// Moves toon, picks up carrot, climbs mountain with carrot.
void CToon::tick()
{
	// Lock shared data mutex and toon mutex
	sharedData->lockMutex();
	std::lock_guard<std::mutex> guard(objMutex);

	// If toon's thread is set to end or game has finished,
	// return before executing turn
	if (getKillThread() || sharedData->getWinner())
	{
		sharedData->unlockMutex();
		return;
	}

	GameBoard* gameBoard = sharedData->getGameBoard();
	int boardSize = gameBoard->getSize();

	// Get random new position for toon to move to
	sf::Vector2i newPos = getRandomNewPos();

	if (!gameBoard->isOccupied(newPos.x, newPos.y))
	{
		// Selected position is empty, move toon
		setGridPosition(newPos);
	}
	else
	{
		// Selected position is not empty.
		// Get ptr to object at selected position.
		CGameObject* otherObj = gameBoard->get(newPos.x, newPos.y);

		// Attempt to pick up carrot at selected position.
		// Returns false if obj is not a carrot.
		bool carrotSuccess = tryTakeCarrot(gameBoard, otherObj);

		// Attempt to pick climb mountain with carrot.
		// Returns false if obj is not the mountain or toon does not
		// have a carrot.
		bool mountainSuccess = tryClimbMountain(gameBoard, otherObj);

		// If one of those actions was successful, move toon
		// to the square and remove the old obj from the board.
		if (carrotSuccess || mountainSuccess)
		{
			otherObj->removeFromBoard(gameBoard);
			setGridPosition(newPos);
		}
	}

	// Unlock shared data mutex
	sharedData->unlockMutex();
}

// Checks if the given CGameObject pointer is a carrot,
// and attempts to pick it up if so.
bool CToon::tryTakeCarrot(GameBoard* gameBoard, CGameObject* otherObj)
{
	if (otherObj && !hasCarrot && otherObj->getType() == GameObjectType::Carrot)
	{
		this->hasCarrot = true;

		std::cout << name << " picked up a carrot.\n";

		return true;
	}

	return false;
}

// Checks if the given CGameObject pointer is the mountain,
// and attempts to climb it if the toon has a carrot.
bool CToon::tryClimbMountain(GameBoard* gameBoard, CGameObject* otherObj)
{
	if (otherObj && hasCarrot && otherObj->getType() == GameObjectType::Mountain && sharedData->getWinner() == nullptr)
	{
		sharedData->setWinner(this);

		std::cout << name << " climbed the mountain with a carrot.\n";

		return true;
	}

	return false;
}

/* =========================================
*  End of File
*  =========================================
*/