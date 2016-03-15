#include "board.h"
#include <vector>

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

int Board::getCornerNumber(Side side) {
	int num = 0;
	if( get(side, 0, 0) ) {
		num += 1;
	}
	if( get(side, 0, 7) ) {
		num += 1;
	}
	if( get(side, 7, 0) ) {
		num += 1;
	}
	if( get(side, 7, 7) ) {
		num += 1;
	}
	return num;
}

int Board::getEdgeNumber(Side side) {
	int num = 0;
	for( int i = 0; i < 8; i++) 
	{
		if( get(side, 0, i) ) 
		{
			num += 1;
		}
		if( get(side, 7, i) ) 
		{
			num += 1;
		}
		if( get(side, i, 0) ) 
		{
			num += 1;
		}
		if( get(side, i, 7) ) 
		{
			num += 1;
		}
	}
	return num;
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

std::vector<Move> Board::getMoves(Side side) {
	std::vector<Move> theMoves;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Move move(i, j);
           		if ( checkMove(&move, side) ){
				theMoves.push_back(move);
			}
		}
	}
	return theMoves;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Returns the position score heuristic for the board
 */
int Board::getScore(Side mySide)
{
	int score = 0;
	if (mySide == BLACK)
	{
		score = countBlack() - countWhite();
	}
	else
	{
		score = countWhite() - countBlack();
	}
	return score;
}

/*
 * Returns the position score heuristic for the board
 */
int Board::getHeuristic(Side mySide)
{
	int score = 0;
	if (mySide == BLACK)
	{
		score = countBlack() - countWhite();
		score += getMoves(BLACK).size() - getMoves(WHITE).size();
		score += 7 * getCornerNumber(BLACK) - 7 * getCornerNumber(WHITE);
		score += 3 * getEdgeNumber(BLACK) - 3 * getEdgeNumber(WHITE);
	}
	else
	{
		score = countWhite() - countBlack();
		score += getMoves(WHITE).size() - getMoves(BLACK).size();
		score += 7 * getCornerNumber(WHITE) - 7 * getCornerNumber(BLACK);
		score += 3 * getEdgeNumber(WHITE) - 3 * getEdgeNumber(BLACK);
	}
	return score;
}

int Board::countEmpty() {
	int count = 64 - countBlack() - countWhite();
	return count;
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}
