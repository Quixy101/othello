#include "player.h"

// Sierra Lincoln othello project - no partner.
// currently this is an attempt at minimax

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = true;
	mySide = side;
	Board myBoard();
	//mostRecentMove (0,0);
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

void Player::setBoardData(char boardData[])
{
	this->myBoard.setBoard(boardData);
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
	enum Side oppSide;
	if(this->mySide == WHITE) 
	{
		this->myBoard.doMove(opponentsMove, BLACK);
		oppSide = BLACK;
	}
	else
	{
		this->myBoard.doMove(opponentsMove, WHITE);
		oppSide = WHITE;
	}
	bool isMovePossible = this->myBoard.hasMoves(this->mySide);
	Move * highestWorstScoreMoveLevel1 = new Move(0,0);
	Move * possibleMoveLevel2 = new Move(0,0);
	//Move * bestMoveLevel2 = new Move(0,0);
	bool moveFound = false;
	int highestWorstScoreLevel2 = -64;
	int worstScoreThisRound = 64;
	//Board boardClone;
	if (isMovePossible)
	{
		for( int i = 0; i < 8; i++)
		{
			for (int j = 0; j <8; j++)
			{
				this->mostRecentMove.setX(i);
				this->mostRecentMove.setY(j);
				bool isThisMoveValid = this->myBoard.checkMove(& this->mostRecentMove, this->mySide);
				if(isThisMoveValid)
				{	
					moveFound = true;
					Board boardClone = * this->myBoard.copy();
					boardClone.doMove(& this->mostRecentMove, this->mySide);
					//Go 1 layer deeper
					for( int k = 0; k < 8; k++)
					{
						for (int l = 0; l <8; l++)
						{
							possibleMoveLevel2->setX(k);
							possibleMoveLevel2->setY(l);
							bool isLevel2MoveValid = this->myBoard.checkMove(possibleMoveLevel2, oppSide);
							if(isLevel2MoveValid)
							{
								boardClone.doMove(possibleMoveLevel2, oppSide);
								int thisScore = boardClone.getScore(this->mySide);
								if(worstScoreThisRound > thisScore)
								{
									worstScoreThisRound = thisScore;
								}
								// reset the board to not have the opponent move
								boardClone = * this->myBoard.copy();
								boardClone.doMove(& this->mostRecentMove, this->mySide);
							}
						}
					}

					if(worstScoreThisRound > highestWorstScoreLevel2)
					{
						highestWorstScoreLevel2 = worstScoreThisRound;
						highestWorstScoreMoveLevel1->setX(i);
						highestWorstScoreMoveLevel1->setY(j);
					}
					worstScoreThisRound = 64;
				}

			}
		}
	}
	if(moveFound == true)
	{
		this->myBoard.doMove(highestWorstScoreMoveLevel1, this->mySide);
		return highestWorstScoreMoveLevel1;
	}
	else
	{		
    		return NULL;
	}
}

/** NON-MINIMAX CODE
bool isMovePossible = this->myBoard.hasMoves(this->mySide);
	Move * bestMove = new Move(0,0);
	bool moveFound = false;
	int bestScore = -64;
	//Board boardClone;
	if (isMovePossible)
	{
		for( int i = 0; i < 8; i++)
		{
			for (int j = 0; j <8; j++)
			{
				this->mostRecentMove.setX(i);
				this->mostRecentMove.setY(j);
				bool isThisMoveValid = this->myBoard.checkMove(& this->mostRecentMove, this->mySide);
				if(isThisMoveValid)
				{	
					if(moveFound == false)
					{
						moveFound = true;
						Board boardClone = * this->myBoard.copy();
						boardClone.doMove(& this->mostRecentMove, this->mySide);
						int newScore = boardClone.getScore(this->mySide);
						bestScore = newScore;
						bestMove->setX(i);
						bestMove->setY(j);
					} 
					else
					{
						Board boardClone = * this->myBoard.copy();
						boardClone.doMove(& this->mostRecentMove, this->mySide);
						int newScore = boardClone.getScore(this->mySide);
						if(newScore > bestScore)
						{
							bestScore = newScore;
							bestMove->setX(i);
							bestMove->setY(j);
						}
					}
					//Code from the must basic possible player
					//this->myBoard.doMove(& this->mostRecentMove, this->mySide);
					//Move * newMove = new Move(i , j);
					//return newMove;
				}
			}
		}
	}
	if(moveFound == true)
	{
		this->myBoard.doMove(bestMove, this->mySide);
		return bestMove;
	}
	else
	{		
    		return NULL;
	}

*/





