#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include <iostream>


namespace othello {

struct Move;
class Proxy;

class Board {
	int size;
	char max_player;
	std::vector<std::vector<char>> board;
public:
	Board();
	Board(int, char);
	Board(const Board&);
	virtual ~Board();
	void init();
	void reset();
	void display() const;
	std::vector<Move> legal_moves(char) const;
	bool is_legal(std::vector<Move>, int, int) const;
	void make_move(char, int, int);
	int get_score() const;
	int get_score1() const;
	int weight(int, int) const;
	bool is_end_of_game() const;
	int get_size() const;
	std::vector<char>& operator[](const int);
	char get_max_player() const;
	void show_final() const;
};

struct Move {
	int x;
	int y;
	Move() :
			x(-1), y(-1) {
	}
	Move(int i, int j) :
			x(i), y(j) {
	}
	Move(const Move & move) :
			x(move.x), y(move.y) {
	}
	friend std::ostream& operator<<(std::ostream&, const Move&);
};

} /* namespace othello */

#endif /* BOARD_H_ */
