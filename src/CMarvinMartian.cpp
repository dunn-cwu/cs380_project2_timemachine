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
// to move
void CMarvinMartian::tick()
{
	sharedData->lockMutex();
	std::lock_guard<std::mutex> guard(objMutex);

	if (getKillThread() || sharedData->getWinner())
	{
		sharedData->unlockMutex();
		return;
	}

	mountainMoveCounter++;
	if (mountainMoveCounter >= 3)
	{
		// Teleport mountain
		mountainMoveCounter = 0;

		CGameObject* mtn = sharedData->getMountain();
		CTimeMachineGame* game = sharedData->getGame();

		game->placeOnBoardRandom(mtn);

		std::cout << name << " has moved the mountain.\n";
	}
	else
	{
		// Randomly move and then kill toon if space
		// was occupied.

		int randDir = getRandomDirection();
		GameBoard* gameBoard = sharedData->getGameBoard();
		int boardSize = gameBoard->getSize();

		sf::Vector2i newPos = getRandomNewPos();

		if (!gameBoard->isOccupied(newPos.x, newPos.y))
		{
			setGridPosition(newPos);
		}
		else
		{
			CGameObject* otherObj = gameBoard->get(newPos.x, newPos.y);

			bool killSuccess = tryKillToon(gameBoard, otherObj);
			bool carrotSuccess = tryTakeCarrot(gameBoard, otherObj);
			bool mountainSuccess = tryClimbMountain(gameBoard, otherObj);

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
		CToon* toon = dynamic_cast<CToon*>(otherObj);

		if (toon->getHasCarrot()) 
		{ 
			std::cout << name << " stole the carrot from " << toon->getName() << std::endl;
			this->hasCarrot = true; 
		}
		
		std::cout << name << " has killed " << toon->getName() << std::endl;

		toon->setKillThread(true);

		return true;
	}

	return false;
}

/* =========================================
*  End of File
*  =========================================
*/