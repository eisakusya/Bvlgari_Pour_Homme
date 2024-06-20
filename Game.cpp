#include"pch.h"
#include "Game.h"
#include<array>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<iostream>

Game::Game()
{
	for (auto& row : grid) {
		row.fill(0);
	}
	std::srand(std::time(0));
	generate();
}

Game::~Game()
{
}

void Game::generate()
{
	std::vector<std::pair<int, int>> emptyGrid;
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			if (grid[i][j] == 0) {
				emptyGrid.emplace_back(i, j);
				/*在使用push_back函数往容器中增加新元素时，必须要有一个该对象的实例才行
				而emplace_back可以不用，它可以直接传入对象的构造函数参数直接进行构造，减少一次拷贝和赋值操作*/

			}
		}
	}

	if (!emptyGrid.empty())
	{
		//- generate randomly 2 or 4 in a random grid
		int index = std::rand() % emptyGrid.size();
		grid[emptyGrid[index].first][emptyGrid[index].second] = (std::rand() % 2 + 1) * 2;
	}
}

void Game::move(char direct)
{
	int row, col, writePos = 0;
	switch (direct)
	{
	case 'w':
		//- move upward
		for (col = 0; col < GRID_SIZE; ++col) {
			//- eliminate zero grid
			writePos = 0;
			for (row = 0; (row < GRID_SIZE); ++row) {
				if (grid[row][col] != 0) {
					if (writePos != row) {
						grid[writePos][col] = grid[row][col];
						grid[row][col] = 0;
					}
					writePos++;
				}
			}
			//- emerge
			for (row = 0; row < GRID_SIZE - 1; ++row) {
				if (grid[row][col] != 0 && grid[row][col] == grid[row + 1][col]) {
					grid[row][col] *= 2;
					grid[row + 1][col] = 0;
				}
			}
			//- eliminate zero grid again
			writePos = 0;
			for (row = 0; (row < GRID_SIZE); ++row) {
				if (grid[row][col] != 0) {
					if (writePos != row) {
						grid[writePos][col] = grid[row][col];
						grid[row][col] = 0;
					}
					writePos++;
				}
			}
		}
		break;
	case 's':
		//- move downward
		for (col = 0; col < GRID_SIZE; ++col) {
			//- eliminate zero grid
			writePos = GRID_SIZE - 1;
			for (row = GRID_SIZE - 1; row >= 0; --row) {
				if (grid[row][col] != 0) {
					if (writePos != row) {
						grid[writePos][col] = grid[row][col];
						grid[row][col] = 0;
					}
					writePos--;
				}
			}
			//- emerge
			for (row = GRID_SIZE - 1; row > 0; --row) {
				if (grid[row][col] != 0 && grid[row][col] == grid[row - 1][col]) {
					grid[row][col] *= 2;
					grid[row - 1][col] = 0;
				}
			}
			//- eliminate zero grid again
			writePos = GRID_SIZE - 1;
			for (row = GRID_SIZE - 1; row >= 0; --row) {
				if (grid[row][col] != 0) {
					if (writePos != row) {
						grid[writePos][col] = grid[row][col];
						grid[row][col] = 0;
					}
					writePos--;
				}
			}
		}
		break;
	case 'a':
		//- move leftward
		for (row = 0; row < GRID_SIZE; ++row) {
			//- eliminate zero grid
			writePos = 0;
			for (col = 0; (col < GRID_SIZE); ++col) {
				if (grid[row][col] != 0) {
					if (writePos != col) {
						grid[row][writePos] = grid[row][col];
						grid[row][col] = 0;
					}
					writePos++;
				}
			}
			//- emerge
			for (col = 0; col < GRID_SIZE - 1; ++col) {
				if (grid[row][col] != 0 && grid[row][col] == grid[row][col + 1]) {
					grid[row][col] *= 2;
					grid[row][col + 1] = 0;
				}
			}
			//- eliminate zero grid again
			writePos = 0;
			for (col = 0; (col < GRID_SIZE); ++col) {
				if (grid[row][col] != 0) {
					if (writePos != col) {
						grid[row][writePos] = grid[row][col];
						grid[row][col] = 0;
					}
					writePos++;
				}
			}
		}
		break;
	case 'd':
		//- move rightward
		for (row = 0; row < GRID_SIZE; ++row) {
			//- eliminate zero grid
			writePos = GRID_SIZE - 1;
			for (col = GRID_SIZE - 1; col >= 0; --col) {
				if (grid[row][col] != 0) {
					if (writePos != col) {
						grid[row][writePos] = grid[row][col];
						grid[row][col] = 0;
					}
					writePos--;
				}
			}
			//- emerge
			for (col = GRID_SIZE - 1; col > 0; --col) {
				if (grid[row][col] != 0 && grid[row][col] == grid[row][col-1]) {
					grid[row][col] *= 2;
					grid[row][col-1] = 0;
				}
			}
			//- eliminate zero grid again
			writePos = GRID_SIZE - 1;
			for (col = GRID_SIZE - 1; col >= 0; --col) {
				if (grid[row][col] != 0) {
					if (writePos != col) {
						grid[row][writePos] = grid[row][col];
						grid[row][col] = 0;
					}
					writePos--;
				}
			}
		}
		break;
	default:
		break;
	}
}

bool Game::canMove()
{
	std::vector<std::pair<int, int>> emptyGrid;
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			if (grid[i][j] == 0) {
				emptyGrid.emplace_back(i, j);

			}
		}
	}
	//- no empty grid
	std::array<std::array<int, GRID_SIZE>, GRID_SIZE> pre = grid;
	if (!emptyGrid.empty()) {
		return true;
	}
	char directions[4] = { 'w','a','s','d' };
	for (char direction : directions) {
		this->move(direction);
		//- there is still way to move
		if (grid != pre) {
			grid = pre;
			return true;
		}
	}
	return false;
}

bool Game::gameover()
{
	bool over = false;
	//- 2048 has been generated
	for (auto row : grid) {
		for (auto unit : row) {
			if (unit==2048)
			{
				over = true;
			}
		}
	}
	//- no next move
	over = !canMove();
	return over;
}

void Game::print()
{
	for (auto row : grid) {
		for (int unit : row) {
			std::cout << unit << " ";
		}

		std::cout << std::endl;
	}
}

int Game::getElement(int row, int col)
{
	try {
		if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE) {
			throw std::out_of_range("Index out of range");
		}
		return grid[row][col];
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}
}

