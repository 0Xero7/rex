#pragma once
#include "symbols.h"
#include "bitboard.h"
#include <vector>
#include "move.h"
#include <intrin.h>

class Board
{
private:
	Bitboard pieces[12];

public:
	Board();
	//Board(const ull& board) : w_pawn(board) {}
	void show_board();

	template <COLOR> void get_movelist(std::vector<Move>&);
	void generate_move(std::vector<Move>&, Bitboard&, bool, SQUARE, PIECE);
	template <COLOR> void make_move(const Move& move);
};