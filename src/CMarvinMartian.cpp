/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CMarvinMartian.cpp
* Description: Implementation for the CMarvinMartian class.
* Represents the Marvin toon. Derives the CToon class.
* Extends CToon functionality to include Marvin's
* unique actions such as killing another toon,
* and teleporting the mountain.
* ==============================================
*/

#include <iostream>
#include "CMarvinMartian.h"
#include "CSharedGameData.h"
#include "CTimeMachineGame.h"

CMarvinMartian::CMarvinMartian() : CToon(), mountainMoveCounter(0)
{
}

CMarvinMartian::CMarvinMartian(CSharedGameData* sharedData, std::string name, std::string spriteFile) 
	: CToon(sharedData, name, spriteFile), mountainMoveCounter(0)
{
}

CMarvinMartian::~CMarvinMartian()
{
}

// Exectutes a single turn for Marvin
// Either teleports the mountain, or attempts
// to move to another square
void CMarvinMartian::tick()
{
	sharedData->lockMutex();
	std::lock_guard<std::mutex> guard(objMutex);

	// If toon's thread is set to end or game has finished,
	// return before executing turn
	if (getKillThread() || sharedData->getWinner())
	{
		sharedData->unlockMutex();
		return;
	}

	mountainMoveCounter++;

	// Is it time to move the mountain?
	if (mountainMoveCounter >= 3)
	{
		// Reset move counter and teleport mountain
		mountainMoveCounter = 0;

		CGameObject* mtn = sharedData->getMountain();
		CTimeMachineGame* game = sharedData->getGame();

		game->placeOnBoardRandom(mtn);

		std::cout << name << " has moved the mountain.\n";
	}
	else
	{
		// Randomly move to another sqaure and then kill toon/pick up carrot/climb mountain
		// if space was occupied.

		GameBoard* gameBoard = sharedData->getGameBoard();
		int boardSize = gameBoard->getSize();

		// Get a random adjacent position on the game board
		sf::Vector2i newPos = getRandomNewPos();

		// Is there a Mountain/Carrot/Toon at the selected position?
		if (!gameBoard->isOccupied(newPos.x, newPos.y))
		{
			// The selected position is empty, so move toon
			setGridPosition(newPos);
		}
		else
		{
			// Selected position is not empty.
			// Get a pointer to the object at selected position.
			CGameObject* otherObj = gameBoard->get(newPos.x, newPos.y);

			// Attempt to kill toon at selected position.
			// Returns false if obj is not a toon.
			// Steals carrot if obj is a toon.
			bool killSuccess = tryKillToon(gameBoard, otherObj);

			// Attempt to pick up carrot at selected position.
			// Returns false if obj is not a carrot.
			bool carrotSuccess = tryTakeCarrot(gameBoard, otherObj);

			// Attempt to climb mountain with a carrot.
			// Returns false if obj is not the mountain, or toon does not
			// have a carrot.
			bool mountainSuccess = tryClimbMountain(gameBoard, otherObj);

			// If one of those actions was successful, move toon
			// to the square and remove the old obj from the board.
			if (carrotSuccess || mountainSuccess || killSuccess)
			{
				otherObj->removeFromBoard();
				setGridPosition(newPos);
			}
		}
	}

	sharedData->unlockMutex();
}

// Checks the given CGameObject to see if it's a toon, and kills/steals their
// carrot of so.
bool CMarvinMartian::tryKillToon(GameBoard* gameBoard, CGameObject* otherObj)
{
	if (otherObj && otherObj->getType() == GameObjectType::Toon)
	{
		// Cast CGameObject pointer to CToon type to expose
		// Toon methods
		CToon* toon = dynamic_cast<CToon*>(otherObj);

		// Does the toon have a carrot?
		if (toon->getHasCarrot()) 
		{ 
			// Steal their carrot
			std::cout << name << " stole the carrot from " << toon->getName() << std::endl;
			this->hasCarrot = true; 
		}
		
		std::cout << name << " has killed " << toon->getName() << std::endl;

		// End the murdered toon's logic thread.
		toon->setKillThread(true);

		return true;
	}

	return false;
}

/* =========================================
*  End of File
*  =========================================
*/