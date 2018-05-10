/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: GameBoard.h
* Description: Header file for GameBoard data
* structure. Contains the game board grid and
* all of the squares. A square is either a null
* pointer, or a pointer to a CGameObject.
* ==============================================
*/

#pragma once

#include <vector>
#include "CGameObject.h"

class GameBoard
{
private:
	std::vector<std::vector<CGameObject*>> grid;
public:
	GameBoard(unsigned int size);
	virtual ~GameBoard();

	CGameObject* get(unsigned int x, unsigned int y);
	void set(unsigned int x, unsigned int y, CGameObject* object);
	bool isOccupied(unsigned int x, unsigned int y);
	int getSize();
};

/* =========================================
*  End of File
*  =========================================
*/