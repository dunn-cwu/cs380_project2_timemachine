/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CToon.h
* Description: Header for the CToon class.
* Represents one of the four toons. Serves as a
* base class for CMarvinMartian.
* ==============================================
*/

#pragma once

#include <random>
#include <string>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include "CGameObject.h"

class CSharedGameData;

class CToon : public CGameObject
{
private:
	std::thread charThread;
	bool killThread;
	sf::Clock clock;

	// Random number generator stuff
	std::random_device randDevice;
	std::mt19937 randEngine;
	std::uniform_int_distribution<> randDis;
	void run();
protected:
	std::mutex runningThreadMutex;
	std::mutex killThreadMutex;
	sf::Time elapsedTime;
	bool hasCarrot;

	// SFML graphics stuff
	sf::Texture carrotTexture;
	sf::RectangleShape carrotSpriteRect;

	void initRandom();
	int getRandomDirection();
	sf::Vector2i getRandomNewPos();
	bool isValidBoardPosition(sf::Vector2i newPos);
	virtual void tick();

	virtual bool tryTakeCarrot(GameBoard* gameBoard, CGameObject* otherObj);
	virtual bool tryClimbMountain(GameBoard* gameBoard, CGameObject* otherObj);
public:
	CToon();
	CToon(CSharedGameData* sharedData, std::string name, std::string spriteFile);
	virtual ~CToon();

	virtual bool loadResource();
	virtual void draw(sf::RenderWindow* renderWindow);

	bool getHasCarrot();
	bool isRunning();
	bool startThread();
	bool stopAndJoinThread();
	bool getKillThread();
	void setKillThread(bool kill);
};

/* =========================================
*  End of File
*  =========================================
*/