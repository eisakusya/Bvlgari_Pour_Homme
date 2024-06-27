#include "pch.h"
#include "Implementation.h"
#include "Game.h"
#include "gamejig.h"
#include "gamedrawer.h"
#include<stack>
#include "ai2048.h"

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
	std::stack<Game> gameUndo;	//used for undo grid
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

		rc = acedGetString(0, _T("\nDecide direction to move [W/A/S/D]: "), kword);
	}

	//- game over
	acutPrintf(_T("\nGame Over."));
	pBTR->close();
}

void gameJig()
{
	//- Jig command
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

void aiGame()
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
	std::stack<Game> gameUndo;	//used for undo grid
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

		rc = acedGetString(0, _T("\nDecide direction to move [W/A/S/D]: "), kword);
	}

	//- game over
	acutPrintf(_T("\nGame Over."));
	pBTR->close();
}

unsigned int exihash[1 << 16];
unsigned int slihash[1 << 16];
unsigned int slibhash[1 << 16];
unsigned int canhash[1 << 16];
void yourGame() {
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
	pGameDrawer->updataArray(game.getGridCopy());
	pGameDrawer->updataScore(game.getScore(), game.getRecordScore());
	AcDbObjectId gameDrawerId;
	pBTR->appendAcDbEntity(gameDrawerId, pGameDrawer);
	pGameDrawer->close();

	//- game loop

	acutPrintf(_T("\n正在加载AI模块 "));

	int judge = 0;

	init(exihash, slihash, slibhash, canhash);

	AcString kword;
	auto rc = acedGetString(0, _T("\n预加载成功: "), kword);

	while ((judge != -1) && (game.gameover() != true)) {
		judge = ai2048(game.getGridCopy(), exihash, slihash, slibhash, canhash);

		if (judge == 1) {
			game.move('w');
		}
		else if (judge == 0) {
			game.move('a');
		}
		else if (judge == 3) {
			game.move('s');
		}
		else if (judge == 2) {
			game.move('d');
		}

		game.generate();
		pGameDrawer = nullptr;
		acdbOpenAcDbEntity((AcDbEntity*&)pGameDrawer, gameDrawerId, AcDb::kForWrite);
		pGameDrawer->updataArray(game.getGridCopy());
		pGameDrawer->updataScore(game.getScore(), game.getRecordScore());
		pGameDrawer->close();

		acutPrintf(_T("\n方向:%d "),judge);
		rc = acedGetString(0, _T("\nNext"), kword);
	}

	//- game over
	acutPrintf(_T("\nGame Over."));
	pBTR->close();

	//	左 : 0
	//	上 : 1
	//	右 : 2
	//	下 : 3
}