#include <iostream>
#include "board.h"
#include <vector>
#include <stdlib.h>
#include <windows.h>

int main()
{
	Board board;

	for (int i = 0; i < 100; ++i) {
		system("CLS");

		std::vector<Move> w;
		board.get_movelist<COLOR::WHITE>(w);
		board.make_move<COLOR::WHITE>(w[rand() % w.size()]);
		board.show_board();

		Sleep(1000);

		system("CLS");
		std::vector<Move> b;
		board.get_movelist<COLOR::BLACK>(b);
		board.make_move<COLOR::BLACK>(b[rand() % b.size()]);
		board.show_board();

		Sleep(1000);
	}
}