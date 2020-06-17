#include "board.h"
#include <stdio.h>
#include <iostream>
#include "movegen.h"

Board::Board() {
	this->w_rook.set(SQUARE::A1); this->w_rook.set(SQUARE::H1);
	this->b_rook.set(SQUARE::A8); this->b_rook.set(SQUARE::H8);

	this->w_knight.set(SQUARE::B1); this->w_knight.set(SQUARE::G1);
	this->b_knight.set(SQUARE::B8); this->b_knight.set(SQUARE::G8);

	this->w_bishop.set(SQUARE::C1); this->w_bishop.set(SQUARE::F1);
	this->b_bishop.set(SQUARE::C8); this->b_bishop.set(SQUARE::F8);

	this->w_queen.set(SQUARE::D1); this->b_queen.set(SQUARE::D8);

	this->w_king.set(SQUARE::E1); this->b_king.set(SQUARE::E8);

	for (int i = 0; i < 8; ++i) {
		this->w_pawn.set((SQUARE)((int)SQUARE::A2 + i));
		this->b_pawn.set((SQUARE)((int)SQUARE::A7 + i));
	}
}

template <> void Board::make_move<COLOR::WHITE>(const Move& move) {
	ull mask = (1ull << (int)move.source_square);
	
	if (move.from_piece == PIECE::ROOK)		this->w_rook	^= (mask | move.move_board);
	if (move.from_piece == PIECE::KNIGHT)	this->w_knight	^= (mask | move.move_board);
	if (move.from_piece == PIECE::BISHOP)	this->w_bishop	^= (mask | move.move_board);
	if (move.from_piece == PIECE::QUEEN)	this->w_queen	^= (mask | move.move_board);
	if (move.from_piece == PIECE::KING)		this->w_king	^= (mask | move.move_board);
	if (move.from_piece == PIECE::PAWN)		this->w_pawn	^= (mask | move.move_board);

	if (b_rook.bboard & move.move_board)	b_rook ^= move.move_board;
	if (b_knight.bboard & move.move_board)	b_knight ^= move.move_board;
	if (b_bishop.bboard & move.move_board)	b_bishop ^= move.move_board;
	if (b_queen.bboard & move.move_board)	b_queen ^= move.move_board;
	if (b_king.bboard & move.move_board)	b_king ^= move.move_board;
	if (b_pawn.bboard & move.move_board)	b_pawn ^= move.move_board;
}

template <> void Board::make_move<COLOR::BLACK>(const Move& move) {
	ull mask = (1ull << (int)move.source_square);
	
	if (move.from_piece == PIECE::ROOK)		this->b_rook	^= (mask | move.move_board);
	if (move.from_piece == PIECE::KNIGHT)	this->b_knight	^= (mask | move.move_board);
	if (move.from_piece == PIECE::BISHOP)	this->b_bishop	^= (mask | move.move_board);
	if (move.from_piece == PIECE::QUEEN)	this->b_queen	^= (mask | move.move_board);
	if (move.from_piece == PIECE::KING)		this->b_king	^= (mask | move.move_board);
	if (move.from_piece == PIECE::PAWN)		this->b_pawn	^= (mask | move.move_board);

	if (w_rook.bboard & move.move_board)	w_rook ^= move.move_board;
	if (w_knight.bboard & move.move_board)	w_knight ^= move.move_board;
	if (w_bishop.bboard & move.move_board)	w_bishop ^= move.move_board;
	if (w_queen.bboard & move.move_board)	w_queen ^= move.move_board;
	if (w_king.bboard & move.move_board)	w_king ^= move.move_board;
	if (w_pawn.bboard & move.move_board)	w_pawn ^= move.move_board;
}

void Board::generate_move(std::vector<Move>& gather, Bitboard& board, bool isAttack, SQUARE square, PIECE piece) {
	ull temp = board.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		gather.push_back(Move(1ull << pos, isAttack, square, piece));
		temp ^= (1ull << pos);
	}
}

template <> void Board::get_movelist<COLOR::WHITE>(std::vector<Move>& gather) {
	gather.reserve(128);

	Bitboard quiets, attacks;
	const Bitboard our	= (w_rook | w_knight | w_bishop | w_king | w_queen | w_pawn);
	const Bitboard opp	= (b_rook | b_knight | b_bishop | b_king | b_queen | b_pawn);
	
	// Rook
	ull temp = w_rook.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::ROOK>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets , false, square, PIECE::ROOK);
		generate_move(gather, attacks, true , square, PIECE::ROOK);
	}

	// Knights
	temp = w_knight.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::KNIGHT>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets, false, square, PIECE::KNIGHT);
		generate_move(gather, attacks, true, square, PIECE::KNIGHT);
	}

	// Queens
	temp = w_queen.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::QUEEN>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets, false, square, PIECE::QUEEN);
		generate_move(gather, attacks, true, square, PIECE::QUEEN);
	}

	// King
	temp = w_king.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::KING>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets, false, square, PIECE::KING);
		generate_move(gather, attacks, true, square, PIECE::KING);
	}

	// Bishops
	temp = w_bishop.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::BISHOP>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets, false, square, PIECE::BISHOP);
		generate_move(gather, attacks, true, square, PIECE::BISHOP);
	}

	// Pawns
	temp = w_pawn.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::PAWN, COLOR::WHITE>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets, false, square, PIECE::PAWN);
		generate_move(gather, attacks, true, square, PIECE::PAWN);
	}
}

template <> void Board::get_movelist<COLOR::BLACK>(std::vector<Move>& gather) {
	gather.reserve(32);

	Bitboard quiets, attacks;
	const Bitboard our	= (b_rook | b_knight | b_bishop | b_king | b_queen | b_pawn);
	const Bitboard opp	= (w_rook | w_knight | w_bishop | w_king | w_queen | w_pawn);
	
	// Rook
	ull temp = b_rook.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::ROOK>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets, false, square, PIECE::ROOK);
		generate_move(gather, attacks, true, square, PIECE::ROOK);
	}

	// Knights
	temp = b_knight.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::KNIGHT>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets, false, square, PIECE::KNIGHT);
		generate_move(gather, attacks, true, square, PIECE::KNIGHT);
	}

	// Queens
	temp = b_queen.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::QUEEN>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets, false, square, PIECE::QUEEN);
		generate_move(gather, attacks, true, square, PIECE::QUEEN);
	}

	// King
	temp = b_king.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::KING>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets, false, square, PIECE::KING);
		generate_move(gather, attacks, true, square, PIECE::KING);
	}

	// Bishops
	temp = b_bishop.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::BISHOP>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets, false, square, PIECE::BISHOP);
		generate_move(gather, attacks, true, square, PIECE::BISHOP);
	}

	// Pawns
	temp = b_pawn.bboard;
	while (temp) {
		int pos = 63 - __lzcnt64(temp);
		SQUARE square = static_cast<SQUARE>(pos);
		temp ^= (1ull << pos);

		make_moves<PIECE::PAWN, COLOR::BLACK>(quiets, attacks, our, opp, square);
		generate_move(gather, quiets, false, square, PIECE::PAWN);
		generate_move(gather, attacks, true, square, PIECE::PAWN);
	}
}


void Board::show_board() {
	char _board[8][8]{ ' ' };

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			const SQUARE square = (SQUARE)(i * 8 + j);

			if (w_rook.test(square))			_board[i][j] = 'R';
			else if (b_rook.test(square))		_board[i][j] = 'r';
			else if (w_knight.test(square))		_board[i][j] = 'N';
			else if (b_knight.test(square))		_board[i][j] = 'n';
			else if (w_bishop.test(square))		_board[i][j] = 'B';
			else if (b_bishop.test(square))		_board[i][j] = 'b';
			else if (w_king.test(square))		_board[i][j] = 'K';
			else if (b_king.test(square))		_board[i][j] = 'k';
			else if (w_queen.test(square))		_board[i][j] = 'Q';
			else if (b_queen.test(square))		_board[i][j] = 'q';
			else if (w_pawn.test(square))		_board[i][j] = 'P';
			else if (b_pawn.test(square))		_board[i][j] = 'p';
		}
	}

	printf("+---+---+---+---+---+---+---+---+\n");
	for (int i = 7; i >= 0; --i) {
		printf("|");
		for (int j = 0; j < 8; ++j)
			printf(" %c |", _board[i][j]);
		printf("\n+---+---+---+---+---+---+---+---+\n");
	}
}
