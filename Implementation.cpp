#include "pch.h"
#include "Implementation.h"
#include "Game.h"
#include "gamejig.h"
#include "gamedrawer.h"
#include<stack>

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
	std::stack<Game> gameUndo;
	bool isUndo = false;
	if (&game != nullptr) {
		acutPrintf(_T("\nGame Created!"));
	}
	GameDrawer* pGameDrawer = new GameDrawer();
	pGameDrawer->updataArray(game.getGridCopy());
	pGameDrawer->updataScore(game.getScore(), game.getRecordScore());
	AcDbObjectId gameDrawerId;
	pBTR->appendAcDbEntity(gameDrawerId, pGameDrawer);
	pGameDrawer->close();

	//- game loop
	/*ACHAR* keyWord = nullptr;
	acedInitGet(RSG_NONULL, _T("W,w A,a S,s D,d"));
	auto rc = acedGetKword(_T("\nDecide direction to move [W/A/S/D]: "), keyWord);*/
	AcString kword;
	auto rc = acedGetString(0, _T("\nDecide direction to move [W/A/S/D]: "), kword);

	gameUndo.push(game);
	while ((rc == RTNORM) && (game.gameover() != true)) {
		isUndo = false;


		if (kword == _T("w")) {
			game.move('w');
		}
		else if (kword == _T("a")) {
			game.move('a');
		}
		else if (kword == _T("s")) {
			game.move('s');
		}
		else if (kword == _T("d")) {
			game.move('d');
		}
		else if (kword == _T("u")) {
			if (gameUndo.size() == 1) {
				acutPrintf(_T("\nNo steps to undo"));
			}
			else {
				gameUndo.pop();
				game = gameUndo.top();
			}
			isUndo = true;

		}
		else {
			rc = acedGetString(0, _T("\nDecide direction to move [W/A/S/D]: "), kword);
			continue;
		}

		if (!isUndo) {
			game.generate();
			gameUndo.push(game);

		}

		pGameDrawer = nullptr;
		acdbOpenAcDbEntity((AcDbEntity*&)pGameDrawer, gameDrawerId, AcDb::kForWrite);
		pGameDrawer->updataArray(game.getGridCopy());
		pGameDrawer->updataScore(game.getScore(), game.getRecordScore());
		pGameDrawer->close();

		/*acedInitGet(RSG_NONULL, _T("W,w A,a S,s D,d"));*/
		rc = acedGetString(0, _T("\nDecide direction to move [W/A/S/D]: "), kword);
	}
	

	//- game over
	acutPrintf(_T("\nGame Over."));

	pBTR->close();
}

void gameJig()
{
	Gamejig* pjig = new Gamejig();
	GameDrawer* pGameDrawer = new GameDrawer();

	

	Game game;
	std::stack<Game> gameUndo;
	bool isUndo = false;
	if (&game != nullptr) {
		acutPrintf(_T("\nGame Created!"));
	}

	pjig->startJig(pGameDrawer, game.getGridCopy(), game.getScore() , game.getRecordScore());
	AcDbObjectId gameDrawerId = pGameDrawer->id();

	pGameDrawer->updataArray(game.getGridCopy());
	pGameDrawer->updataScore(game.getScore(), game.getRecordScore());

	pGameDrawer->close();

	//- game loop
	/*ACHAR* keyWord = nullptr;
	acedInitGet(RSG_NONULL, _T("W,w A,a S,s D,d"));
	auto rc = acedGetKword(_T("\nDecide direction to move [W/A/S/D]: "), keyWord);*/
	AcString kword;
	auto rc = acedGetString(0, _T("\nDecide direction to move [W/A/S/D]: "), kword);

	gameUndo.push(game);
	while ((rc == RTNORM) && (game.gameover() != true)) {
		isUndo = false;


		if (kword == _T("w")) {
			game.move('w');
		}
		else if (kword == _T("a")) {
			game.move('a');
		}
		else if (kword == _T("s")) {
			game.move('s');
		}
		else if (kword == _T("d")) {
			game.move('d');
		}
		else if (kword == _T("u")) {
			if (gameUndo.size() == 1) {
				acutPrintf(_T("\nNo steps to undo"));
			}
			else {
				gameUndo.pop();
				game = gameUndo.top();
			}
			isUndo = true;

		}
		else {
			rc = acedGetString(0, _T("\nDecide direction to move [W/A/S/D]: "), kword);
			continue;
		}

		if (!isUndo) {
			game.generate();
			gameUndo.push(game);

		}

		pGameDrawer = nullptr;
		acdbOpenAcDbEntity((AcDbEntity*&)pGameDrawer, gameDrawerId, AcDb::kForWrite);
		pGameDrawer->updataArray(game.getGridCopy());
		pGameDrawer->updataScore(game.getScore(), game.getRecordScore());
		pGameDrawer->close();

		/*acedInitGet(RSG_NONULL, _T("W,w A,a S,s D,d"));*/
		rc = acedGetString(0, _T("\nDecide direction to move [W/A/S/D]: "), kword);
	}

	//- game over
	acutPrintf(_T("\nGame Over."));
}
