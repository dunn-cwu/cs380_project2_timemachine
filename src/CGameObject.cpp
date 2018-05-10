/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CGameObject.cpp
* Description: Implementation for the CGameObject class.
* Represents a drawable game object that can be
* on the game board. Used to represent the mountain
* and carrots. Used as a base class for CToon and
* CMarvinMartian. Stores basic information about the
* game object and has a method to draw it to the
* game board at the correct location.
* ==============================================
*/

#include <iostream>
#include "CSharedGameData.h"
#include "CGameObject.h"
#include "CTimeMachineGame.h"
#include "GameBoard.h"

CGameObject::CGameObject(CSharedGameData* sharedData, std::string name, GameObjectType type, std::string textureFile)
{
	this->sharedData = sharedData;
	this->name = name;
	this->type = type;
	this->textureFile = textureFile;
	this->gridPos = sf::Vector2i(0, 0);
}


CGameObject::~CGameObject()
{
}

std::string CGameObject::getName()
{
	return name;
}

GameObjectType CGameObject::getType()
{
	return type;
}

// Removes the GameObject from the game board
void CGameObject::removeFromBoard(GameBoard* gameBoard)
{
	if (!gameBoard) { gameBoard = sharedData->getGameBoard(); }

	CGameObject* obj = gameBoard->get(gridPos.x, gridPos.y);

	if (obj == this)
	{
		gameBoard->set(gridPos.x, gridPos.y, nullptr);
	}
}

// Retruns the game objects x and y position on the game board
sf::Vector2i CGameObject::getGridPosition()
{
	return gridPos;
}

// Sets the game objects position on the game board
void CGameObject::setGridPosition(sf::Vector2i newPos, bool updateBoard)
{
	GameBoard* gameBoard = sharedData->getGameBoard();
	
	if (updateBoard)
	{
		// Clear old pos
		removeFromBoard(gameBoard);

		// Set new pos
		gameBoard->set(newPos.x, newPos.y, this);
	}

	gridPos = newPos;
}

// Loads and prepares the character sprite for the game object
bool CGameObject::loadResource()
{
	if (textureFile.length() <= 0)
	{
		std::cout << "Error loading resource for " << name << ": Resource file string empty." << std::endl;
		return false;
	}

	if (!texture.loadFromFile(textureFile))
	{
		return false;
	}

	spriteRect.setTexture(&texture, true);

	return true;
}

// Draws the game object sprite to the game board at the 
// correct location
void CGameObject::draw(sf::RenderWindow* renderWindow)
{
	std::lock_guard<std::mutex> guard(objMutex);

	sf::FloatRect drawRect = sharedData->getGame()->getGridCellRect(gridPos.x, gridPos.y);

	spriteRect.setPosition(drawRect.left, drawRect.top);
	spriteRect.setSize(sf::Vector2f(drawRect.width, drawRect.height));

	if (sharedData->getWinner() == this) { spriteRect.setFillColor(sf::Color::Green); }

	renderWindow->draw(spriteRect);
}

/* =========================================
*  End of File
*  =========================================
*/