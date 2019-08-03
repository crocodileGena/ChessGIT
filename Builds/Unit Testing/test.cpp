#include "pch.h"
#include "../../Source/board.h"

TEST(BoardTest, ResetBalance)
{
	Board myBoard;
	myBoard.ResetBoard();
	EXPECT_EQ(myBoard.GetBalance() , 0);
}