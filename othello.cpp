#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>


#include "Board.h"
using namespace std;
using namespace othello;

void usage(char* prog);
void get_input(vector<string> &);
Move minimax_move(Board board, char player, int limit);
int min_value(Board board, char player, int depth, int a, int b);
int max_value(Board board, char player, int depth, int a, int b);

int main(int argc, char *argv[]) {
	if (argc != 4) {
		usage(argv[0]);
		return 1;
	}
	int size = atoi(argv[1]);
	char max_player = argv[2][0];
	int limit = atoi(argv[3]);
	if (limit <= 0) {
		usage(argv[0]);
		return 1;
	}
	Board board(size, max_player);

	char player; // current player
	int x = 0, y = 0; // row and column
	bool not_valid = false;

	vector<string> tokens;
	time_t start = time(0);
	while (true) {
		cout << "> " << flush;
		tokens.clear();
		get_input(tokens);
		if (tokens.empty()) {
			continue;
		}
		if (tokens.size() == 1 && tokens[0] == "init") {
			board.init();
		} else if (tokens.size() == 4 && tokens[0] == "put") {
			player = tokens[1][0];
			vector<Move> moves = board.legal_moves(player);
			if (!moves.empty()) {
				string xx = tokens[2];
				string yy = tokens[3];
				x = atoi(xx.c_str());
				y = atoi(yy.c_str());
				if (x >= 0 && y >= 0 && x < size && y < size
						   && board.is_legal(moves, x, y)) {
					board.make_move(player, x, y);

					not_valid = false;
				} else {
					cout << "\n# Not a valid move, try again." << endl;
				}
			} else {
				cout << "\n# No legal moves available, so " << player
						<< "'s turn is forfeited" << endl;
				if (not_valid) {
					break;
				}
				not_valid = true;
			}
		} else if (tokens.size() == 2 && tokens[0] == "move") {
			player = tokens[1][0];
			vector<Move> moves = board.legal_moves(player);
			if (!moves.empty()) {
				Move move = minimax_move(board, player, limit);
				cout << move << endl;
				board.make_move(player, move.x, move.y);
				not_valid = false;
			} else {
				cout << "\n# No legal moves available, so " << player
						<< "'s turn is forfeited" << endl;
				if (not_valid) {
					break;
				}
				not_valid = true;
			}
		} else if (tokens.size() == 1 && tokens[0] == "quit") {
			cout << "Thank you! Hope you had fun!" << endl;
			return 0;
		} else if (tokens.size() == 1 && tokens[0] == "reset") {
			board.reset();
		} else {
			cout << "# Your command is not supported in this version of Othello\n"
			     << "# Only the following commands are supported:\n"
				 << "#     \"init\" - places 4 pieces in the middle (standard starting configuration)\n"
				 << "#     \"put <P> <X> <Y>\" - place a stone of type P at coords X,Y, and flip\n" 
				 << "#                           any pieces that can be flipped\n"
				 << "#     \"move <P>\" - ask the program to choose the next move for player P\n"
				 << "#                     using minimax (and update the state based on this move)\n"
				 << "#     \"reset\" - removes all stones, leaving an empty board\n"
				 << "#     \"quit\"- exit the program\n" 
				 << "# \"init\" must be used as an initial command" << endl;
		}
		if (board.is_end_of_game()) {
			break;
		}
		board.display();
	}
	/* Game is over */
	board.display();
	board.show_final();
	time_t end = time(0);
	cout << "\n# game over" << endl;
	cout << "\n# Time used in seconds: " << (end - start) << endl;
	return 0;
}

void usage(char* prog) {
	cout << "Usage: " << prog << " <N> <C> <D>" << endl
			<< "- N is the size of the (square) board" << endl
			<< "- C is the color to be controlled by the program" << endl
			<< "- D is the search depth limit, which should a positive number"
			<< endl;
}

void get_input(vector<string> & tokens) {
	 string mode;
	 string buf;
	 getline(cin, mode);
	 stringstream ss(mode); // Insert the string into a stream
	 while (ss >> buf) {
	 	 tokens.push_back(buf);
	 }
 }

Move minimax_move(Board board, char player, int limit) {
	char opponent = player == 'W' ? 'B' : 'W';
	int a = -1000;
	int b = 1000;
	cout << "# making move for " << player << "..." << endl;
	if (player == board.get_max_player()) {
		int max =  -1000;
		vector<Move> possible_moves = board.legal_moves(player);
		Move max_move;
		for (auto move : possible_moves) {
			Board tmp_board = board;
			tmp_board.make_move(player, move.x, move.y);
			int next = min_value(tmp_board, opponent, limit, max, b);
			cout << "# considering: " << move << ", mm=" << next << endl;
			if (next > max) {
				max = next;
				max_move.x = move.x;
				max_move.y = move.y;
			}
		}
		return max_move;
	} else {
		vector<Move> possible_moves = board.legal_moves(player);
		int min = 1000;
		Move min_move;
		for (auto move : possible_moves) {
			Board tmp_board = board;
			tmp_board.make_move(player, move.x, move.y);
			int next = max_value(tmp_board, opponent, limit, a, min);
			cout << "# considering: " << move << ", mm=" << next << endl;
			if (next < min) {
				min = next;
				min_move.x = move.x;
				min_move.y = move.y;
			}
		}
		return min_move;
	}
}

int max_value(Board board, char player, int depth, int a, int b) {
	if (depth == 0 || board.is_end_of_game()) {
		return board.get_score();
	}
	char opponent = player == 'W' ? 'B' : 'W';
	vector<Move> possible_moves = board.legal_moves(player);
	if (possible_moves.empty()) {
		return min_value(board, opponent, depth - 1, a, b);
	}
	for (auto move : possible_moves) {
		Board tmp_board = board;
		tmp_board.make_move(player, move.x, move.y);

		a = max(a, min_value(tmp_board, opponent, depth - 1, a, b));

		if (b <= a)
			return b; // β cutoff.
	}
	return a;
}

int min_value(Board board, char player, int depth, int a, int b) {
	if (depth == 0 || board.is_end_of_game()) {
		return board.get_score();
	}
	char opponent = player == 'W' ? 'B' : 'W';
	vector<Move> possible_moves = board.legal_moves(player);
	if (possible_moves.empty()) {
		return max_value(board, opponent, depth - 1, a, b);
	}
	for (auto move : possible_moves) {
		Board tmp_board = board;
		tmp_board.make_move(player, move.x, move.y);

		b = min(b, max_value(tmp_board, opponent, depth - 1, a, b));

		if (b <= a)
			return a; // α cutoff.
	}
	return b;
}