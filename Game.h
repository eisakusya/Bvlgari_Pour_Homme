#pragma once
#include<array>
#ifndef GAME_H
#define GAME_H

#define GRID_SIZE 4
class Game
{
private:
	std::array<std::array<int, GRID_SIZE>, GRID_SIZE> grid;

public:
	Game();
	~Game();
	void generate();
	void move(char direct);
	bool canMove();
	bool gameover();

};

#endif // !GAME_H