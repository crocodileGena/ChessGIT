#pragma once

#define kIllegalSquare 8

const int staticButtonHeight = 20;
const int moveButtonHeight = 20;
const int staticButtonWidth = 30;
const int moveButtonWidth = 50;
const int buttonSpacing = 5;

enum PieceLocation
{
	eFile = 0,
	eRank
};

enum CastlingFlags
{
	whiteShort = 0,
	whiteLong,
	blackShort,
	blackLong,
	numCastlingOptions
};

enum BoardPosition
{
	A = 0,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	One = 0,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight
};

const int BoardSize = 8;
const int EmptySquare = -1;

const int A1[2] = { A, One };
const int A2[2] = { A, Two };
const int A3[2] = { A, Three };
const int A4[2] = { A, Four };
const int A5[2] = { A, Five };
const int A6[2] = { A, Six };
const int A7[2] = { A, Seven };
const int A8[2] = { A, Eight };

const int B1[2] = { B, One };
const int B2[2] = { B, Two };
const int B3[2] = { B, Three };
const int B4[2] = { B, Four };
const int B5[2] = { B, Five };
const int B6[2] = { B, Six };
const int B7[2] = { B, Seven };
const int B8[2] = { B, Eight };

const int C1[2] = { C, One };
const int C2[2] = { C, Two };
const int C3[2] = { C, Three };
const int C4[2] = { C, Four };
const int C5[2] = { C, Five };
const int C6[2] = { C, Six };
const int C7[2] = { C, Seven };
const int C8[2] = { C, Eight };

const int D1[2] = { D, One };
const int D2[2] = { D, Two };
const int D3[2] = { D, Three };
const int D4[2] = { D, Four };
const int D5[2] = { D, Five };
const int D6[2] = { D, Six };
const int D7[2] = { D, Seven };
const int D8[2] = { D, Eight };

const int E1[2] = { E, One };
const int E2[2] = { E, Two };
const int E3[2] = { E, Three };
const int E4[2] = { E, Four };
const int E5[2] = { E, Five };
const int E6[2] = { E, Six };
const int E7[2] = { E, Seven };
const int E8[2] = { E, Eight };

const int F1[2] = { F, One };
const int F2[2] = { F, Two };
const int F3[2] = { F, Three };
const int F4[2] = { F, Four };
const int F5[2] = { F, Five };
const int F6[2] = { F, Six };
const int F7[2] = { F, Seven };
const int F8[2] = { F, Eight };

const int G1[2] = { G, One };
const int G2[2] = { G, Two };
const int G3[2] = { G, Three };
const int G4[2] = { G, Four };
const int G5[2] = { G, Five };
const int G6[2] = { G, Six };
const int G7[2] = { G, Seven };
const int G8[2] = { G, Eight };

const int H1[2] = { H, One };
const int H2[2] = { H, Two };
const int H3[2] = { H, Three };
const int H4[2] = { H, Four };
const int H5[2] = { H, Five };
const int H6[2] = { H, Six };
const int H7[2] = { H, Seven };
const int H8[2] = { H, Eight };

enum Color
{
	eWhite = 0,
	eBlack
};

enum Worth
{
	ePawn = 1,
	eBishop = 3,
	eKnight = 3,
	eRook = 5,
	eQueen = 9,
	eKing = 0
};


