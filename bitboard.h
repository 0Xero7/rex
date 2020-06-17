#pragma once
#include "symbols.h"
#include "stdio.h"

class Bitboard 
{
private:
public:
	ull bboard = (0ull);
	Bitboard() : bboard(0ull) {}
	Bitboard(ull board) : bboard(board) {}

	Bitboard operator |  (Bitboard& arg) { return Bitboard(this->bboard | arg.bboard); }
	Bitboard operator &  (Bitboard& arg) { return Bitboard(this->bboard & arg.bboard); }
	Bitboard operator &  (ull arg)		 { return Bitboard(this->bboard & arg); }
	Bitboard operator << (int&      arg) { return Bitboard(this->bboard << arg); }
	Bitboard operator >> (int&      arg) { return Bitboard(this->bboard >> arg); }
	Bitboard operator ~  ()              { return Bitboard(~this->bboard); }

	void	 operator &= (Bitboard& arg) { this->bboard &= arg.bboard; }
	void	 operator &= (ull arg)		 { this->bboard &= arg; }
	void	 operator ^= (ull arg)		 { this->bboard ^= arg; }

	void set  (const SQUARE& sq) {
		bboard |= (1ull << (int)(sq));
	}

	void set(ull& arg) { bboard = arg; }

	bool test (const SQUARE& sq) {
		return ((bboard >> ((int)sq)) & 1ull);
	}
};

