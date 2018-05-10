/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CMarvinMartian.h
* Description: Header for the CMarvinMartian class.
* Represents the Marvin toon. Derives the CToon class.
* Extends CToon functionality to include Marvin's
* unique actions such as killing another toon,
* and teleporting the mountain.
* ==============================================
*/

#pragma once

#include "CToon.h"

class CMarvinMartian : public CToon
{
protected:
	int mountainMoveCounter;

	virtual void tick();
	virtual bool tryKillToon(GameBoard* gameBoard, CGameObject* otherObj);
public:
	CMarvinMartian();
	CMarvinMartian(CSharedGameData* sharedData, std::string name, std::string spriteFile);
	~CMarvinMartian();
};

/* =========================================
*  End of File
*  =========================================
*/