#include "chess.hpp"

int main() {

    Board board;
    Rook rook1({2,2}, white);
    King king1({5,5}, black);

    board.add_to_board(rook1);
    board.add_to_board(king1);

    game(board);

    return EXIT_SUCCESS;
}
