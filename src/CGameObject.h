/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CGameObject.h
* Description: Header for the CGameObject class.
* Represents a drawable game object that can be
* on the game board. Used to represent the mountain
* and carrots. Used as a base class for CToon and
* CMarvinMartian. Stores basic information about the
* game object and has a method to draw it to the
* game board at the correct location.
* ==============================================
*/

#pragma once

#include <mutex>
#include <string>
#include <SFML/Graphics.hpp>

class CSharedGameData;
class GameBoard;

enum GameObjectType
{
	Carrot,
	Mountain,
	Toon
};

class CGameObject
{
protected:
	CSharedGameData * sharedData;
	std::string name;
	GameObjectType type;
	std::mutex objMutex;

	// Graphics stuff
	std::string textureFile;
	sf::Texture texture;
	sf::RectangleShape spriteRect;
	sf::Vector2i gridPos;
public:
	CGameObject(CSharedGameData* sharedData, std::string name, GameObjectType type, std::string textureFile);
	virtual ~CGameObject();

	std::string getName();
	GameObjectType getType();
	sf::Vector2i getGridPosition();
	void setGridPosition(sf::Vector2i newPos, bool updateBoard = true);
	virtual void removeFromBoard(GameBoard* gameBoard = nullptr);
	virtual bool loadResource();
	virtual void draw(sf::RenderWindow* renderWindow);
};

/* =========================================
*  End of File
*  =========================================
*/