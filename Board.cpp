#include <iostream>
#include "Board.h"


namespace othello {

using namespace std;

Board::Board() :
		size(0), max_player('W'), board() {
}
Board::Board(int sz, char mp) :
		size(sz), max_player(mp), board(sz, vector<char>(sz)) {
}

Board::Board(const Board& bd) {
	this->size = bd.size;
	this->max_player = bd.max_player;
	this->board = bd.board;
}

Board::~Board() {
	this->board.clear();
}

vector<char>& Board::operator[](const int row) {
	return board[row];
}

int Board::get_size() const {
	return size;
}

void Board::init() {
	/* Blank all the board squares */
	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++)
			board[row][col] = '.';

	/* Place the initial four counters in the center */
	board[size / 2 - 1][size / 2 - 1] = board[size / 2][size / 2] = 'W';
	board[size / 2 - 1][size / 2] = board[size / 2][size / 2 - 1] = 'B';
}

void Board::reset() {
	/* Blank all the board squares */
	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++)
			board[row][col] = '.';
}

void Board::display() const {

	int row = 0; /* Row index      */
	int col = 0; /* Column index   */
	cout << "#\n# Score=" << get_score();
	for (row = 0; row < size; row++) {
		cout << "\n#";
		for (col = 0; col < size; col++) {
			cout << ' ' << board[row][col];
		}
	}
	cout << endl;
}

vector<Move> Board::legal_moves(char player) const {
	int rowdelta = 0;
	int coldelta = 0;
	int row = 0;
	int col = 0;
	int x = 0;
	int y = 0;
	vector<Move> result;
	/* Set the opponent            */
	char opponent = (player == 'W') ? 'B' : 'W';
	bool flag = false;
	for (row = 0; row < size; row++) {
		for (col = 0; col < size; col++) {
			if (board[row][col] != '.') /* Is it a blank square?  */
				continue; /* No - so on to the next */
			flag = false;
			/* Check all the squares around the blank square  */
			/* for the opponents counter                      */
			for (rowdelta = -1; rowdelta <= 1; rowdelta++) {
				for (coldelta = -1; coldelta <= 1; coldelta++) {
					/* Don't check outside the array, or the current square */
					if (row + rowdelta < 0 || row + rowdelta >= size
							|| col + coldelta < 0 || col + coldelta >= size
							|| (rowdelta == 0 && coldelta == 0))
						continue;

					/* Now check the square */
					if (board[row + rowdelta][col + coldelta] == opponent) {
						/* If we find the opponent, move in the delta direction  */
						/* over opponent counters searching for a player counter */
						x = row + rowdelta; /* Move to          */
						y = col + coldelta; /* opponent square  */

						/* Look for a player square in the delta direction */
						for (;;) {
							x += rowdelta; /* Go to next square */
							y += coldelta; /* in delta direction*/

							/* If we move outside the array, give up */
							if (x < 0 || x >= size || y < 0 || y >= size)
								break;

							/* If we find a blank square, give up */
							if (board[x][y] == '.')
								break;
							if (board[x][y] == player) {
								result.push_back(Move(row, col));
								flag = true;
								break;
							}
						}
						if (flag) {
							break;
						}
					}
				}
				if (flag) {
					break;
				}
			}
		}
	} // end first for loop
	return result;
}

bool Board::is_legal(vector<Move> legalMoves, int x, int y) const {
	for (auto move : legalMoves) {
		if (move.x == x && move.y == y) {
			return true;
		}
	}
	return false;
}

void Board::make_move(char player, int row, int col) {
	int rowdelta = 0;
	int coldelta = 0;
	int x = 0;
	int y = 0; /* Column index for searching */
	char opponent = (player == 'W') ? 'B' : 'W'; /* Identify opponent */

	board[row][col] = player; /* Place the player counter   */
	/* Check all the squares around this square */
	/* for the opponents counter                */
	for (rowdelta = -1; rowdelta <= 1; rowdelta++)
		for (coldelta = -1; coldelta <= 1; coldelta++) {
			/* Don't check off the board, or the current square */
			if (row + rowdelta < 0 || row + rowdelta >= size
					|| col + coldelta < 0 || col + coldelta >= size
					|| (rowdelta == 0 && coldelta == 0))
				continue;

			/* Now check the square */
			if (board[row + rowdelta][col + coldelta] == opponent) {
				/* If we find the opponent, search in the same direction */
				/* for a player counter                                  */
				x = row + rowdelta; /* Move to opponent */
				y = col + coldelta; /* square           */

				for (;;) {
					x += rowdelta; /* Move to the      */
					y += coldelta; /* next square      */

					/* If we are off the board give up */
					if (x < 0 || x >= size || y < 0 || y >= size)
						break;

					/* If the square is blank give up */
					if (board[x][y] == '.')
						break;

					/* If we find the player counter, go backwards from here */
					/* changing all the opponents counters to player         */
					if (board[x][y] == player) {
						while (board[x -= rowdelta][y -= coldelta] == opponent) {
							board[x][y] = player;
						}
						break;
					}
				}
			}

		}

}

int Board::get_score() const {
	int score = 0;
	int row = 0;
	int col = 0;
	char player = max_player;
	char opponent = player == 'W' ? 'B' : 'W';
	int w = 1;
	for (row = 0; row < size; row++) {
		for (col = 0; col < size; col++) {
			w = weight(row, col);
			score -= (board[row][col] == opponent) * w;
			score += (board[row][col] == player) * w;
		}
	}
	return score;
}

int Board::get_score1() const {
	int score = 0;
	int row = 0;
	int col = 0;
	char player = max_player;
	char opponent = player == 'W' ? 'B' : 'W';
	for (row = 0; row < size; row++) {
		for (col = 0; col < size; col++) {
			score -= (board[row][col] == opponent);
			score += (board[row][col] == player);
		}
	}
	return score;
}

int Board::weight(int row, int col) const {
	if ((row == 0 && col == 0) ||
		(row == 0 && col == size - 1) ||
		(row == size - 1 && col == 0) ||
		(row == size - 1 && col == size - 1)) {
		return 50;
	} else if ((row == 1 && col == 1) ||
		(row == 1 && col == size - 2) ||
		(row == size - 2 && col == 1) ||
		(row == size - 2 && col == size - 2)) {
		return -10;
	} else if ((row == 0 && col == 1) ||
		(row == 0 && col == size - 2) ||
		(row == 1 && col == 0) ||
		(row == 1 && col == size - 1) ||
		(row == size - 2 && col == 0) ||
		(row == size - 1 && col == 1) ||
		(row == size - 2  && col == size - 1) ||
		(row == size - 1 && col == size - 2)) {
		return -1;
	} else if (row == 0 || col == 0 || row == size -1 || col == size - 1){
		return 3;
	} else {
	    return 1;
	}
}

bool Board::is_end_of_game() const {
	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++)
			if (board[row][col] == '.') {
				return false;
			}
	return true;
}

char Board::get_max_player() const {
	return max_player;
}

void Board::show_final() const {
	/* Get final scores and display them */
	int B_score = 0, W_score = 0;
	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++) {
			B_score += board[row][col] == 'B';
			W_score += board[row][col] == 'W';
		}
	cout << "# The final score is:" << endl;
	cout << "# The 'B' player: " << B_score << endl << "# The 'W' player: "
			<< W_score << endl;
}

std::ostream& operator<<(std::ostream& os, const Move& move) {
	os << '(' << move.x << ',' << move.y << ')';
	return os;
}

} /* namespace othello */
