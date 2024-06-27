#include "pch.h"
#include "ai2048.h"

using namespace std;
typedef unsigned int ui;
typedef unsigned long long ull;


inline int getm(ull board, int x, int y) {
	int k = x * 16 + y * 4;
	return (board >> k) & 0xf;
}
inline ull setm(ull board, int x, int y, ull d) {
	int k = x * 16 + y * 4;
	return (board & ~(0xfULL << k)) | (d << k);
}

inline ui getr(ull board, int x) {
	return (board >> (x * 16)) & 0xffff;
}
inline ull setr(ull board, int x, ui d) {
	int k = x * 16;
	return (board & ~(0xffffULL << k)) | ((ull)(d) << k);
}

inline ui getc(ull board, int y) {
	ui r = 0;
	ui mask = 0xf;
	board >>= 4 * y;
	for (int i = 0; i < 4; i++) {
		r |= board & mask;
		mask <<= 4;
		board >>= 12;
	}
	return r;
}
inline ull setc(ull board, int y, ull d) {
	ull mask = 0xfULL << (y * 4);
	d <<= 4 * y;
	for (int i = 0; i < 4; i++) {
		board = (board & ~(mask)) | (d&mask);
		d <<= 12;
		mask <<= 16;
	}
	return board;
}


void init(unsigned int exihash[1 << 16], unsigned int slihash[1 << 16], unsigned int slibhash[1 << 16], unsigned int canhash[1 << 16]) {
	for (ull i = 0; i < (1 << 16); i++) {
		for (int j = 0; j < 4; j++) {
			if (getm(i, 0, j)) exihash[i]++;
		}
		int r = 0;
		for (int j = 0; j < 3; j++) {
			int u1 = getm(i, 0, j), u2 = getm(i, 0, j + 1);
			if (u1 && u1 == u2) r++;
		}
		canhash[i] = r;
		int c = 0;
		ull ii = i;
		for (int j = 1; j < 4; j++) {
			int d = getm(ii, 0, j);
			if (!d) continue;
			for (int k = j - 1; k >= c; k--) {
				int u = getm(ii, 0, k);
				if (!u) {
					ii = setm(ii, 0, k + 1, 0);
					ii = setm(ii, 0, k, d);
					continue;
				}
				if (d != u) break;
				ii = setm(ii, 0, k + 1, 0);
				ii = setm(ii, 0, k, d + 1);
				c = k + 1;
			}
		}
		slihash[i] = ii;
		c = 3;
		ii = i;
		for (int j = 2; j >= 0; j--) {
			int d = getm(ii, 0, j);
			if (!d) continue;
			for (int k = j + 1; k <= c; k++) {
				int u = getm(ii, 0, k);
				if (!u) {
					ii = setm(ii, 0, k - 1, 0);
					ii = setm(ii, 0, k, d);
					continue;
				}
				if (d != u) break;
				ii = setm(ii, 0, k - 1, 0);
				ii = setm(ii, 0, k, d + 1);
				c = k - 1;
			}
		}
		slibhash[i] = ii;
	}
}

inline ull slided(ull board, int dir, unsigned int slihash[1 << 16], unsigned int slibhash[1 << 16]) {
	switch (dir) {
	case 0:
		for (int i = 0; i < 4; i++) {
			board = setr(board, i, slihash[getr(board, i)]);
		}
		break;
	case 1:
		for (int j = 0; j < 4; j++) {
			board = setc(board, j, slihash[getc(board, j)]);
		}
		break;
	case 2:
		for (int i = 0; i < 4; i++) {
			board = setr(board, i, slibhash[getr(board, i)]);
		}
		break;
	case 3:
		for (int j = 0; j < 4; j++) {
			board = setc(board, j, slibhash[getc(board, j)]);
		}
		break;
	}
	return board;
}

bool is_fill(ull board) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (!getm(board, i, j)) return false;
		}
	}
	return true;
}

inline int num_exist(ull board, unsigned int exihash[1 << 16]) {
	int r = 0;
	for (int i = 0; i < 4; i++) {
		r += exihash[getr(board, i)];
	}
	return r;
}

inline int can_slide(ull board, unsigned int canhash[1 << 16]) {
	int r = 0;
	for (int i = 0; i < 4; i++) {
		r += canhash[getr(board, i)];
		r += canhash[getc(board, i)];
	}
	return r;
}

inline double calp(ull board, unsigned int exihash[1 << 16], unsigned int canhash[1 << 16]) {
	double u = (16 - num_exist(board, exihash));
	double u2 = can_slide(board, canhash);
	double u3 = (getm(board, 0, 0) >= 11 ? 1 : 0);
	double u4 = u3 * (getm(board, 0, 0) >= 10 ? 1 : 0);
	return u + u2 / 2 + u3 * 3 + u4 * 5;
}

double calc(ull board, int dps, unsigned int exihash[1 << 16], unsigned int slihash[1 << 16], unsigned int slibhash[1 << 16], unsigned int canhash[1 << 16]) {
	double p = 0.0;
	int u = 0;
	const static int k[] = { 0, 10, 1 };
	ull b, b2;
	double p2;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (getm(board, i, j)) continue;
			for (int d = 1; d <= 2; d++) {
				u++;
				b = setm(board, i, j, d);
				if (!dps) {
					p += calp(b, exihash, canhash)*k[d];
				}
				else {
					p2 = -10000;
					for (int k = 0; k < 4; k++) {
						b2 = slided(b, k, slihash,slibhash);
						if (b == b2) continue;
						p2 = max(p2, calc(b2, min(1 + num_exist(b2,exihash) / 4, dps - 1), exihash, slihash, slibhash, canhash));
					}
					p += p2 * k[d];
				}
			}
		}
	}
	return p / u;
}

int solve(ull board, unsigned int exihash[1 << 16], unsigned int slihash[1 << 16], unsigned int slibhash[1 << 16], unsigned int canhash[1 << 16]) {
	ull b;
	int r = 0;
	double p = -100000;
	for (int i = 0; i < 4; i++) {
		b = slided(board, i, slihash, slibhash);
		if (b == board) continue;
		double np = calc(b, 1 + num_exist(b, exihash) / 4, exihash, slihash, slibhash, canhash);
		if (p < np) {
			p = np;
			r = i;
		}
	}
	return r;
}

bool is_end(ull board) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (!getm(board, i, j)) return false;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if (getm(board, i, j) == getm(board, i, j + 1)) return false;
		}
	}
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 3; i++) {
			if (getm(board, i, j) == getm(board, i + 1, j)) return false;
		}
	}
	return true;
}


int ai2048(array<array<int, 4>, 4>data, unsigned int exihash[1 << 16], unsigned int slihash[1 << 16], unsigned int slibhash[1 << 16], unsigned int canhash[1 << 16]) {

	ull board = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int k;
			k = data[i][j];
			board = setm(board, i, j, k);
		}
	}
	if (is_end(board)) {
		return -1;
	}
	else {
		return solve(board, exihash, slihash, slibhash, canhash);
	}


}