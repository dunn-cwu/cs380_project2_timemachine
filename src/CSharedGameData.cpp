/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CSharedGameData.cpp
* Description: Implementation for the CSharedGameData class.
* Stores all game data that is shared between threads.
* Contains method to lock/unlock a mutex to prevent
* race conditions between toon threads.
* ==============================================
*/

#include "CSharedGameData.h"

CSharedGameData::CSharedGameData(CTimeMachineGame* game) : gameBoard(DEFAULT_BOARD_SIZE)
{
	this->game = game;
	this->turnWaitTimeMs = DEFUALT_TURN_WAIT;
	this->gameActive = false;
	this->winner = nullptr;
}


CSharedGameData::~CSharedGameData()
{
	clear();
}

void CSharedGameData::lockMutex()
{
	objMutex.lock();
}

void CSharedGameData::unlockMutex()
{
	objMutex.unlock();
}

// Clears game data/releases memory
void CSharedGameData::clear()
{
	gameActive = false;
	winner = nullptr;
	charRosterVector.clear();

	mountainObj.reset();
	carrotOneObj.reset();
	carrotTwoObj.reset();
}

CTimeMachineGame* CSharedGameData::getGame()
{
	return game;
}

GameBoard* CSharedGameData::getGameBoard()
{
	return &gameBoard;
}

// Sets the amount of time in miliseconds that a toon
// will wait between turns
void CSharedGameData::setTurnWait(int newWait)
{
	std::lock_guard<std::mutex> guard(dataMutex);

	turnWaitTimeMs = newWait;
}

// Gets the amount of time in miliseconds that a toon
// will wait between turns
int CSharedGameData::getTurnWait()
{
	std::lock_guard<std::mutex> guard(dataMutex);

	return turnWaitTimeMs;
}

// Returns the number of toons in the character roster
int CSharedGameData::getNumToons()
{
	return charRosterVector.size();
}

// Returns pointer to the character at the given index
CToon* CSharedGameData::getToon(int index)
{
	return charRosterVector.at(index).get();
}

// Adds a toon to the character roster
void CSharedGameData::addToon(std::unique_ptr<CToon>& newChar)
{
	charRosterVector.push_back(std::move(newChar));
}

// Returns a pointer to the mountain CGameObject
CGameObject* CSharedGameData::getMountain()
{
	return mountainObj.get();
}

// Transfers ownership of a given mountain CGameObject to the shared data
// class
void CSharedGameData::setMountain(std::unique_ptr<CGameObject>& newMtn)
{
	if (mountainObj) { mountainObj.reset(); }
	mountainObj = std::move(newMtn);
}

// Returns a pointer to one of the carrot CGameObjects
CGameObject* CSharedGameData::getCarrot(int index)
{
	if (index == 0) { return carrotOneObj.get(); }
	if (index == 1) { return carrotTwoObj.get(); }

	return nullptr;
}

// Transfers ownership of a given carrot CGameObject to the shared data
// class
void CSharedGameData::setCarrot(int index, std::unique_ptr<CGameObject>& newCarrot)
{
	if (index == 0)
	{
		if (carrotOneObj) { carrotOneObj.reset(); }
		carrotOneObj = std::move(newCarrot);
	}
	else if (index == 1)
	{
		if (carrotTwoObj) { carrotTwoObj.reset(); }
		carrotTwoObj = std::move(newCarrot);
	}
	else
	{
		return;
	}
}

// Returns true if a game is currently in session
bool CSharedGameData::getGameActive()
{
	std::lock_guard<std::mutex> guard(dataMutex);
	return gameActive;
}

// Sets game active flag
void CSharedGameData::setGameActive(bool active)
{
	std::lock_guard<std::mutex> guard(dataMutex);
	gameActive = active;
}

// Returns a pointer to the winning toon, or 
// a nullptr if no toon has won yet
CToon* CSharedGameData::getWinner()
{
	std::lock_guard<std::mutex> guard(dataMutex);
	return winner;
}

// Sets winning toon
void CSharedGameData::setWinner(CToon* winner)
{
	std::lock_guard<std::mutex> guard(dataMutex);
	this->winner = winner;
}

/* =========================================
*  End of File
*  =========================================
*/