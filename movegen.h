#pragma once
#include "symbols.h"
#include "direction.cpp"

template <DIRECTION> ull shift(const ull& board) { assert(0); }
template <> ull shift<DIRECTION::NORTH>(const ull& board) { return ( (board & ~RANK_8) << 8ull ); }
template <> ull shift<DIRECTION::SOUTH>(const ull& board) { return ( (board & ~RANK_1) >> 8ull ); }
template <> ull shift<DIRECTION::WEST >(const ull& board) { return ( (board & ~FILE_A) >> 1ull ); }
template <> ull shift<DIRECTION::EAST >(const ull& board) { return ( (board & ~FILE_H) << 1ull ); }

template <> ull shift<DIRECTION::NORTHEAST>(const ull& board) { return ( (board & ~(FILE_H | RANK_8)) << 9ull ); }
template <> ull shift<DIRECTION::NORTHWEST>(const ull& board) { return ( (board & ~(FILE_A | RANK_8)) << 7ull ); }
template <> ull shift<DIRECTION::SOUTHEAST>(const ull& board) { return ( (board & ~(FILE_H | RANK_1)) >> 7ull ); }
template <> ull shift<DIRECTION::SOUTHWEST>(const ull& board) { return ( (board & ~(FILE_A | RANK_1)) >> 9ull ); }


template<PIECE> void make_moves(Bitboard&, Bitboard&, const Bitboard&, const Bitboard&, SQUARE) { assert(0); }

template <> void make_moves<PIECE::KNIGHT>(Bitboard& quiets, Bitboard& attacks, 
  const Bitboard& our, const Bitboard& opponent, SQUARE square) {

	ull board(1ull << (int)square);
	ull res(0ull);

	ull ne = shift<DIRECTION::NORTHEAST>(board);
	ull nw = shift<DIRECTION::NORTHWEST>(board);
	ull se = shift<DIRECTION::SOUTHEAST>(board);
	ull sw = shift<DIRECTION::SOUTHWEST>(board);

	res |=	shift<DIRECTION::NORTH>(ne) | shift<DIRECTION::EAST>(ne) |
			shift<DIRECTION::NORTH>(nw) | shift<DIRECTION::WEST>(nw) |
			shift<DIRECTION::SOUTH>(se) | shift<DIRECTION::EAST>(se) |
			shift<DIRECTION::SOUTH>(sw) | shift<DIRECTION::WEST>(sw);
	res &= ~our.bboard;

	quiets = res & ~opponent.bboard;
	attacks = quiets.bboard ^ res;
}

template <> void make_moves<PIECE::KING>(Bitboard& quiets, Bitboard& attacks,
  const Bitboard& our, const Bitboard& opponent, SQUARE square) {

	ull board(1ull << (int)square);
	ull res = shift<DIRECTION::NORTH>(board) | shift<DIRECTION::NORTHEAST>(board) | shift<DIRECTION::EAST>(board) |
		shift<DIRECTION::SOUTHEAST>(board) | shift<DIRECTION::SOUTH>(board) | shift<DIRECTION::SOUTHWEST>(board) |
		shift<DIRECTION::WEST>(board) | shift<DIRECTION::NORTHWEST>(board);
	res &= ~our.bboard;

	quiets = res & ~opponent.bboard;
	attacks = quiets.bboard ^ res;
}

template <> void make_moves<PIECE::ROOK>(Bitboard& quiets, Bitboard& attacks,
  const Bitboard& our, const Bitboard& opponent, SQUARE square) {

	ull board(1ull << (int)square);
	ull fullBoared = (our.bboard | opponent.bboard);

	ull res = 0ull;

	// top
	while (1) {
		board = shift<DIRECTION::NORTH>(board);
		if (board == 0ull) break;
		if ((board & our.bboard)) break;
		res |= board;
		if ((board & opponent.bboard)) break;
	}

	board = (1ull << (int)square);
	while (1) {
		board = shift<DIRECTION::EAST>(board);
		if (board == 0ull) break;
		if ((board & our.bboard)) break;
		res |= board;
		if ((board & opponent.bboard)) break;
	}

	board = (1ull << (int)square);
	while (1) {
		board = shift<DIRECTION::SOUTH>(board);
		if (board == 0ull) break;
		if ((board & our.bboard)) break;
		res |= board;
		if ((board & opponent.bboard)) break;
	}

	board = (1ull << (int)square);
	while (1) {
		board = shift<DIRECTION::WEST>(board);
		if (board == 0ull) break;
		if ((board & our.bboard)) break;
		res |= board;
		if ((board & opponent.bboard)) break;
	}

	ull Q = res & ~opponent.bboard;
	quiets.set(Q);
	Q = res & opponent.bboard;
	attacks.set(Q);
}

template <> void make_moves<PIECE::BISHOP>(Bitboard& quiets, Bitboard& attacks,
  const Bitboard& our, const Bitboard& opponent, SQUARE square) {

	ull board(1ull << (int)square);
	ull fullBoared = (our.bboard | opponent.bboard);

	ull res = 0ull;

	while (1) {
		board = shift<DIRECTION::NORTHEAST>(board);
		if (board == 0ull) break;
		if ((board & our.bboard)) break;
		res |= board;
		if ((board & opponent.bboard)) break;
	}

	board = (1ull << (int)square);
	while (1) {
		board = shift<DIRECTION::NORTHWEST>(board);
		if (board == 0ull) break;
		if ((board & our.bboard)) break;
		res |= board;
		if ((board & opponent.bboard)) break;
	}

	board = (1ull << (int)square);
	while (1) {
		board = shift<DIRECTION::SOUTHEAST>(board);
		if (board == 0ull) break;
		if ((board & our.bboard)) break;
		res |= board;
		if ((board & opponent.bboard)) break;
	}

	board = (1ull << (int)square);
	while (1) {
		board = shift<DIRECTION::SOUTHWEST>(board);
		if (board == 0ull) break;
		if ((board & our.bboard)) break;
		res |= board;
		if ((board & opponent.bboard)) break;
	}

	ull Q = res & ~opponent.bboard;
	quiets.set(Q);
	Q = res & opponent.bboard;
	attacks.set(Q);
}

template <> void make_moves<PIECE::QUEEN>(Bitboard& quiets, Bitboard& attacks,
  const Bitboard& our, const Bitboard& opponent, SQUARE square) {

	make_moves<PIECE::ROOK>(quiets, attacks, our, opponent, square);
	Bitboard temp_q, temp_a;
	make_moves<PIECE::BISHOP>(temp_q, temp_a, our, opponent, square);
	quiets  = quiets | temp_q;
	attacks = attacks | temp_a;
}

template <PIECE, COLOR> void make_moves(Bitboard&, Bitboard&, const Bitboard&, const Bitboard&, SQUARE);

template<> void make_moves<PIECE::PAWN, COLOR::WHITE>(Bitboard& quiets, Bitboard& attacks,
  const Bitboard& our, const Bitboard& opponent, SQUARE square) {

	ull _board(1ull << (int)square);
	ull board = our.bboard | opponent.bboard;

	// white pawns go north
	ull res = shift<DIRECTION::NORTH>(_board) & ~board;
	if (_board & RANK_2) {
		res |= shift<DIRECTION::NORTH>(res) & ~board;
	}

	quiets.set(res);

	res = shift<DIRECTION::NORTHEAST>(_board) | shift<DIRECTION::NORTHWEST>(_board);
	res &= opponent.bboard;
	attacks.set(res);
}

template<> void make_moves<PIECE::PAWN, COLOR::BLACK>(Bitboard& quiets, Bitboard& attacks,
  const Bitboard& our, const Bitboard& opponent, SQUARE square) {

	ull _board(1ull << (int)square);
	ull board = our.bboard | opponent.bboard;

	// black pawns go south
	ull res = shift<DIRECTION::SOUTH>(_board) & ~board;
	if (_board & RANK_7) {
		res |= shift<DIRECTION::SOUTH>(res) & ~board;
	}

	quiets.set(res);

	res = shift<DIRECTION::SOUTHEAST>(_board) | shift<DIRECTION::SOUTHWEST>(_board);
	res &= opponent.bboard;
	attacks.set(res);
}
