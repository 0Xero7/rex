#pragma once
#include "symbols.h"
#include "bitboard.h"

class Move
{
public:
	ull move_board;
	bool isAttack;
	SQUARE source_square;
	PIECE from_piece;

	Move(const Bitboard& board, bool isAttack, SQUARE source, PIECE piece) :
		move_board(board.bboard), isAttack(isAttack), source_square(source), from_piece(piece)
	{}

	Move(const ull& board, bool isAttack, SQUARE source, PIECE piece) :
		move_board(board), isAttack(isAttack), source_square(source), from_piece(piece)
	{}
};

