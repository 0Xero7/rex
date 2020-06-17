#pragma once
#include "symbols.h"
#include "bitboard.h"
#include <vector>
#include "move.h"
#include <intrin.h>

class Board
{
private:
	Bitboard w_rook;
	Bitboard w_knight;
	Bitboard w_bishop;
	Bitboard w_king;
	Bitboard w_queen;
	Bitboard w_pawn;
			 
	Bitboard b_rook;
	Bitboard b_knight;
	Bitboard b_bishop;
	Bitboard b_king;
	Bitboard b_queen;
	Bitboard b_pawn;

public:
	Board();
	Board(const ull& board) : w_pawn(board) {}
	void show_board();

	template <COLOR> void get_movelist(std::vector<Move>&);
	void generate_move(std::vector<Move>&, Bitboard&, bool, SQUARE, PIECE);
	template <COLOR> void make_move(const Move& move);
};