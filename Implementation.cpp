#include "pch.h"
#include "Implementation.h"
#include "Game.h"
#include "gamedrawer.h"

void myGame()
{
	//- obtain pBTR
	auto pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable* pBT = nullptr;
	pDb->getBlockTable(pBT, AcDb::kForWrite);
	if (!pBT) {
		return;
	}
	AcDbBlockTableRecord* pBTR = nullptr;
	pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite);
	if (!pBTR) {
		return;
	}
	pBT->close();

	//- Game initialization
	Game game;
	if (&game != nullptr) {
		acutPrintf(_T("\nGame Created!"));
	}
	GameDrawer* pGameDrawer = new GameDrawer();
	AcDbObjectId gameDrawerId;
	pBTR->appendAcDbEntity(gameDrawerId, pGameDrawer);

	//- game loop
	ACHAR* keyWord = nullptr;
	acedInitGet(RSG_NONULL, _T("W,w A,a S,s D,d"));
	auto rc = acedGetKword(_T("\nDecide direction to move [W/A/S/D]: "), keyWord);

	while ((rc == RTNORM) && (game.gameover() != true)) {
		if (keyWord == _T("W")) {
			game.move('w');
		}
		else if (keyWord == _T("A")) {
			game.move('a');
		}
		else if (keyWord == _T("S")) {
			game.move('s');
		}
		else {
			game.move('d');
		}

		game.generate();

		acedInitGet(RSG_NONULL, _T("W,w A,a S,s D,d"));
		rc = acedGetKword(_T("\nDecide direction to move [W/A/S/D]: "), keyWord);
	}

	//- game over
	acutPrintf(_T("\nGame Over."));
	pBTR->close();
}
