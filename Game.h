#pragma once
#include<array>
#ifndef GAME_H
#define GAME_H

#define GRID_SIZE 4
#define FILE_DIRECTORY "ScoreRecord.dat"
class Game
{
private:
	std::array<std::array<int, GRID_SIZE>, GRID_SIZE> grid;
	int score;
	int highestScore;

public:
	Game();
	~Game();
	void generate();
	void move(char direct);
	bool canMove();
	bool gameover();
	void print();
	
	//- method to get grid
	int getElement(int row, int col);
	std::array<std::array<int, GRID_SIZE>, GRID_SIZE> getGridCopy();

	//- method to access score
	int getScore();
	int getRecordScore();
};

#endif // !GAME_H