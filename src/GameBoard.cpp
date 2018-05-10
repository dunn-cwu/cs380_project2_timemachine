/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: GameBoard.cpp
* Description: Implementation for GameBoard data
* structure. Contains the game board grid and
* all of the squares. A square is either a null
* pointer, or a pointer to a CGameObject.
* ==============================================
*/

#include "GameBoard.h"

GameBoard::GameBoard(unsigned int size)
{
	// Create two dimensional Vector for game grid
	grid = std::vector<std::vector<CGameObject*>>(size, std::vector<CGameObject*>(size));
}

GameBoard::~GameBoard()
{
	grid.clear();
}

// Gets the CGameObject pointer at the specified board pos
CGameObject* GameBoard::get(unsigned int x, unsigned int y)
{
	return grid[x][y];
}

// Sets the CGameObject pointer at the specified board pos
void GameBoard::set(unsigned int x, unsigned int y, CGameObject* object)
{
	grid[x][y] = object;
}

// Returns true if the specified board position has a valid
// pointer to a CGameObject
bool GameBoard::isOccupied(unsigned int x, unsigned int y)
{
	return grid[x][y] != nullptr;
}

// Gets the width of the game board
int GameBoard::getSize()
{
	return grid.size();
}

/* =========================================
*  End of File
*  =========================================
*/