/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CSharedGameData.h
* Description: Header for the CSharedGameData class.
* Stores all game data that is shared between threads.
* Contains method to lock/unlock a mutex to prevent
* race conditions between toon threads.
* ==============================================
*/

#pragma once

#include <vector>
#include <mutex>
#include "GameBoard.h"
#include "CToon.h"

class CTimeMachineGame;

static const int DEFUALT_TURN_WAIT = 500;
static const int DEFAULT_BOARD_SIZE = 5;

static const std::string MOUNTAIN_NAME = "Mountain";
static const std::string MOUNTAIN_TEXTURE = "resource_mountain.png";

static const std::string CARROT_NAME = "Carrot";
static const std::string CARROT_TEXTURE = "resource_carrot.png";

static const std::string BUNNY_NAME = "Bugs Bunny";
static const std::string BUNNY_TEXTURE = "resource_bunny.png";

static const std::string MARVIN_NAME = "Marvin Martian";
static const std::string MARVIN_TEXTURE = "resource_marvin.png";

static const std::string TAZ_NAME = "Taz";
static const std::string TAZ_TEXTURE = "resource_taz.png";

static const std::string TWEETY_NAME = "Tweety";
static const std::string TWEETY_TEXTURE = "resource_tweety.png";

class CSharedGameData
{
private:
	CTimeMachineGame* game;
	GameBoard gameBoard;
	int turnWaitTimeMs;

	std::mutex objMutex;
	std::mutex dataMutex;
	std::vector<std::unique_ptr<CToon>> charRosterVector;
	std::unique_ptr<CGameObject> mountainObj;
	std::unique_ptr<CGameObject> carrotOneObj;
	std::unique_ptr<CGameObject> carrotTwoObj;
	
	bool gameActive;
	CToon* winner;
public:
	CSharedGameData(CTimeMachineGame* game);
	~CSharedGameData();

	void lockMutex();
	void unlockMutex();

	void clear();
	CTimeMachineGame* getGame();
	GameBoard* getGameBoard();

	void setTurnWait(int newWait);
	int getTurnWait();

	int getNumToons();
	CToon* getToon(int index);
	void addToon(std::unique_ptr<CToon>& newChar);

	CGameObject* getMountain();
	void setMountain(std::unique_ptr<CGameObject>& newMtn);
	CGameObject* getCarrot(int index);
	void setCarrot(int index, std::unique_ptr<CGameObject>& newCarrot);

	bool getGameActive();
	void setGameActive(bool active);
	CToon* getWinner();
	void setWinner(CToon* winner);
};

/* =========================================
*  End of File
*  =========================================
*/