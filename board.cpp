#include "board.h"
#include <stdio.h>
#include <iostream>
#include "movegen.h"

Board::Board() {
	pieces[ROOK].set(A1);		pieces[ROOK].set(H1);
	pieces[6 + ROOK].set(A8);	pieces[6 + ROOK].set(H8);

	pieces[KNIGHT].set(B1);     pieces[KNIGHT].set(G1);
	pieces[6 + KNIGHT].set(B8); pieces[6 + KNIGHT].set(G8);

	pieces[BISHOP].set(C1);     pieces[BISHOP].set(F1);
	pieces[6 + BISHOP].set(C8); pieces[6 + BISHOP].set(F8);

	pieces[KING].set(E1);
	pieces[6 + KING].set(E8);

	pieces[QUEEN].set(D1);
	pieces[6 + QUEEN].set(D8);

	for (int i = 0; i < 8; ++i) {
		pieces[PAWN].set((SQUARE)((int)SQUARE::A2 + i));
		pieces[6 + PAWN].set((SQUARE)((int)SQUARE::A7 + i));
	}
}

template <> void Board::make_move<COLOR::WHITE>(const Move& move) {
	ull mask = (1ull << (int)move.source_square);
	
	pieces[move.from_piece] ^= (mask | move.move_board);

	if (pieces[6].bboard & move.move_board)  pieces[6] ^= move.move_board;
	if (pieces[7].bboard & move.move_board)	 pieces[7] ^= move.move_board;
	if (pieces[8].bboard & move.move_board)	 pieces[8] ^= move.move_board;
	if (pieces[9].bboard & move.move_board)	 pieces[9] ^= move.move_board;
	if (pieces[10].bboard & move.move_board) pieces[10] ^= move.move_board;
	if (pieces[11].bboard & move.move_board) pieces[11] ^= move.move_board;
}

template <> void Board::make_move<COLOR::BLACK>(const Move& move) {
	ull mask = (1ull << (int)move.source_square);

	pieces[6 + move.from_piece] ^= (mask | move.move_board);

	if (pieces[0].bboard & move.move_board) pieces[0] ^= move.move_board;
	if (pieces[1].bboard & move.move_board) pieces[1] ^= move.move_board;
	if (pieces[2].bboard & move.move_board) pieces[2] ^= move.move_board;
	if (pieces[3].bboard & move.move_board) pieces[3] ^= move.move_board;
	if (pieces[4].bboard & move.move_board) pieces[4] ^= move.move_board;
	if (pieces[5].bboard & move.move_board) pieces[5] ^= move.move_board;
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
	const Bitboard our = pieces[(int)PIECE::ROOK] | pieces[(int)PIECE::BISHOP] | pieces[(int)PIECE::KNIGHT] |
		pieces[(int)PIECE::KING] | pieces[(int)PIECE::QUEEN] | pieces[(int)PIECE::PAWN];

	const Bitboard opp = pieces[6 + (int)PIECE::ROOK] | pieces[6 + (int)PIECE::BISHOP] | pieces[6 + (int)PIECE::KNIGHT] |
		pieces[6 + (int)PIECE::KING] | pieces[6 + (int)PIECE::QUEEN] | pieces[6 + (int)PIECE::PAWN];


	for (int i = 0; i < 6; ++i) {
		ull temp = pieces[i].bboard;
		while (temp) {
			int pos = 63 - __lzcnt64(temp);
			SQUARE square = static_cast<SQUARE>(pos);
			temp ^= (1ull << pos);

			if (i == 0) make_moves<(PIECE)0>(quiets, attacks, our, opp, square);
			if (i == 1) make_moves<(PIECE)1>(quiets, attacks, our, opp, square);
			if (i == 2) make_moves<(PIECE)2>(quiets, attacks, our, opp, square);
			if (i == 3) make_moves<(PIECE)3>(quiets, attacks, our, opp, square);
			if (i == 4) make_moves<(PIECE)4>(quiets, attacks, our, opp, square);
			if (i == 5) make_moves<(PIECE)5, COLOR::WHITE>(quiets, attacks, our, opp, square);

			generate_move(gather, quiets, false, square, static_cast<PIECE>(i));
			generate_move(gather, attacks, true, square, static_cast<PIECE>(i));
		}
	}
}

template <> void Board::get_movelist<COLOR::BLACK>(std::vector<Move>& gather) {
	gather.reserve(128);

	Bitboard quiets, attacks;
	const Bitboard our = pieces[6 + (int)PIECE::ROOK] | pieces[6 + (int)PIECE::BISHOP] | pieces[6 + (int)PIECE::KNIGHT] | 
		pieces[6 + (int)PIECE::KING] | pieces[6 + (int)PIECE::QUEEN] | pieces[6 + (int)PIECE::PAWN];

	const Bitboard opp = pieces[(int)PIECE::ROOK] | pieces[(int)PIECE::BISHOP] | pieces[(int)PIECE::KNIGHT] | 
		pieces[(int)PIECE::KING] | pieces[(int)PIECE::QUEEN] | pieces[(int)PIECE::PAWN];
	
	for (int i = 0; i < 6; ++i) {
		ull temp = pieces[6 + i].bboard;
		while (temp) {
			int pos = 63 - __lzcnt64(temp);
			SQUARE square = static_cast<SQUARE>(pos);
			temp ^= (1ull << pos);

			if (i == 0) make_moves<(PIECE)0>(quiets, attacks, our, opp, square);
			if (i == 1) make_moves<(PIECE)1>(quiets, attacks, our, opp, square);
			if (i == 2) make_moves<(PIECE)2>(quiets, attacks, our, opp, square);
			if (i == 3) make_moves<(PIECE)3>(quiets, attacks, our, opp, square);
			if (i == 4) make_moves<(PIECE)4>(quiets, attacks, our, opp, square);
			if (i == 5) make_moves<(PIECE)5, COLOR::BLACK>(quiets, attacks, our, opp, square);

			generate_move(gather, quiets, false, square, static_cast<PIECE>(i));
			generate_move(gather, attacks, true, square, static_cast<PIECE>(i));
		}
	}
}


void Board::show_board() {
	char _board[8][8]{ ' ' };

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			const SQUARE square = (SQUARE)(i * 8 + j);

			if      (pieces[(int)PIECE::ROOK].test(square))			_board[i][j] = 'R';
			else if (pieces[6 + (int)PIECE::ROOK].test(square))		_board[i][j] = 'r';

			else if (pieces[(int)PIECE::KNIGHT].test(square))		_board[i][j] = 'N';
			else if (pieces[6 + (int)PIECE::KNIGHT].test(square))	_board[i][j] = 'n';

			else if (pieces[(int)PIECE::BISHOP].test(square))		_board[i][j] = 'B';
			else if (pieces[6 + (int)PIECE::BISHOP].test(square))	_board[i][j] = 'b';

			else if (pieces[(int)PIECE::KING].test(square))			_board[i][j] = 'K';
			else if (pieces[6 + (int)PIECE::KING].test(square))		_board[i][j] = 'k';

			else if (pieces[(int)PIECE::QUEEN].test(square))		_board[i][j] = 'Q';
			else if (pieces[6 + (int)PIECE::QUEEN].test(square))	_board[i][j] = 'q';

			else if (pieces[(int)PIECE::PAWN].test(square))			_board[i][j] = 'P';
			else if (pieces[6 + (int)PIECE::PAWN].test(square))		_board[i][j] = 'p';
		}
	}

	printf("+---+---+---+---+---+---+---+---+\n");
	for (int i = 7; i >= 0; --i) {
		printf("|");
		for (int j = 0; j < 8; ++j)
			printf(" %c |", _board[i][j]);
		printf("\n+---+---+---+---+---+---+---+---+\n");
	}
	printf("+---+---+---+---+---+---+---+---+\n");
	printf("| a | b | c | d | e | f | g | h |\n");
}
