#include "pch.h"
#include "Game.h"
#include<array>
#include<cstdlib>
#include<ctime>
#include<vector>

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
				/*��ʹ��push_back������������������Ԫ��ʱ������Ҫ��һ���ö����ʵ������
				��emplace_back���Բ��ã�������ֱ�Ӵ������Ĺ��캯������ֱ�ӽ��й��죬����һ�ο����͸�ֵ����*/

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
	case 's':
		//- move downward
		for (col = 0; col < GRID_SIZE; ++col) {
			//- eliminate zero grid
			writePos = GRID_SIZE - 1;
			for (row = GRID_SIZE - 1; row > 0; --row) {
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
			for (row = GRID_SIZE - 1; row > 0; --row) {
				if (grid[row][col] != 0) {
					if (writePos != row) {
						grid[writePos][col] = grid[row][col];
						grid[row][col] = 0;
					}
					writePos--;
				}
			}
		}
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
	case 'd':
		//- move rightward
		for (row = 0; row < GRID_SIZE; ++row) {
			//- eliminate zero grid
			writePos = GRID_SIZE - 1;
			for (col = GRID_SIZE - 1; col > 0; --col) {
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
			for (col = GRID_SIZE - 1; col > 0; --col) {
				if (grid[row][col] != 0) {
					if (writePos != col) {
						grid[row][writePos] = grid[row][col];
						grid[row][col] = 0;
					}
					writePos--;
				}
			}
		}
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
	bool flag = false;
	std::array<std::array<int, GRID_SIZE>, GRID_SIZE> pre = grid;
	if (emptyGrid.empty()) {
		char directions[4] = { 'w','a','s','d' };
		for (char direction : directions) {
			this->move(direction);
			//- there is still way to move
			if (grid != pre) flag = true;
			grid = pre;
		}
	}

	return flag;
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

