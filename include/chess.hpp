//
// Created by Milosz on 19.08.2020.
//

#ifndef SANDBOX_CHESS_HPP
#define SANDBOX_CHESS_HPP

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <map>
#include <queue>

enum Figures{
    pawn = 1, knight = 2, bishop = 3, rook = 4, queen = 5, king = 6
};

enum Colour{
    black = 0, white = 1
};

enum Directions{
    N, NE, E, SE, S, SW, W, NW, HORSE
};

class Board;

class Figure{
public:
    //Constructor
    Figure(std::vector<std::size_t> pos , bool is_white): pos_(std::move(pos)), is_white_(is_white) {};

    //non-const methods
    void change_pos(std::vector<std::size_t>& pos) {pos_ = pos; moves_ += 1;}
    void add_board(Board* board_ptr) {board = board_ptr;}

    //const methods
    std::vector<std::size_t> get_pos() const {return pos_;}
    bool get_colour() const {return is_white_;}
    std::size_t get_y_pos() const {return pos_[0];}
    std::size_t get_x_pos() const {return pos_[1];}
    std::size_t how_many_moves() const {return moves_;}
    Board* get_board() const {return board;}
    bool try_to_strike(std::vector<std::size_t> pos) const;

    // virtual methods
    virtual bool move(std::vector<size_t> pos);
    virtual std::queue<std::vector<std::size_t>> create_path_of_movement(const std::vector<size_t>&) const{
        return std::queue<std::vector<std::size_t>>();
    };
    virtual std::size_t get_id() const = 0;

    ~Figure() = default;

private:
    std::vector<std::size_t> pos_;
    const bool is_white_;
    std::size_t moves_ = 0;
    Board* board = nullptr;
};

class Pawn : public Figure{
public:
    //constructor
    Pawn(std::vector<std::size_t> pos, bool is_white): Figure(std::move(pos), is_white){}

    //non-const methods
    bool move(std::vector<size_t> pos) override;

    //const methods
    std::size_t get_id() const override {return id_;}
    std::queue<std::vector<std::size_t>> create_path_of_movement(const std::vector<size_t>& pos) const override;

    ~Pawn() = default;

private:
    Figures id_ = pawn;
};

class Knight : public Figure{
public:
    // Constructor
    Knight(std::vector<std::size_t> pos, bool is_white): Figure(std::move(pos), is_white){}

    // non-const methods
    bool move(std::vector<size_t> pos) override;

    // const methods
    std::size_t get_id() const override {return id_;}

    ~Knight() = default;

private:
    const Figures id_ = knight;
};

class Rook : public Figure{
public:
    // Constructor
    Rook(std::vector<std::size_t> pos, bool is_white): Figure(std::move(pos), is_white){}

    // const methods
    std::queue<std::vector<std::size_t>> create_path_of_movement(const std::vector<size_t>& pos) const override;
    std::size_t get_id() const override {return id_;}

private:
    const Figures id_ = rook;
};

class Bishop : public Figure{
public:
    // Constructor
    Bishop(std::vector<std::size_t> pos, bool is_white): Figure(std::move(pos), is_white){}

    // const methods
    std::queue<std::vector<std::size_t>> create_path_of_movement(const std::vector<size_t>& pos) const override;
    std::size_t get_id() const override {return id_;}

    ~Bishop() = default;

private:
    const Figures id_ = bishop;
};

class Queen : public Figure{
public:
    // Constructor
    Queen(std::vector<std::size_t> pos, bool is_white): Figure(std::move(pos), is_white){}

    // const methods
    std::queue<std::vector<std::size_t>> create_path_of_movement(const std::vector<size_t>& pos) const override;
    std::size_t get_id() const override {return id_;}

    ~Queen() = default;

private:
    const Figures id_ = queen;
};

class King : public Figure{
public:
    // Constructor
    King(std::vector<std::size_t> pos, bool is_white): Figure(std::move(pos), is_white){}

    // overridden move()
    bool move(std::vector<size_t> pos) override;

    // const methods
    std::queue<std::vector<std::size_t>> create_path_of_movement(const std::vector<size_t>& pos) const override;
    std::size_t get_id() const override {return id_;}
    bool is_checked(std::vector<std::size_t> pos = {}) const;

    ~King() = default;
private:
    const Figures id_ = king;
};

class Board{
public:
    //non-const methods
    void add_to_board(Figure& figure) {
        set_of_figures_.push_back(&figure);
        figure.add_board(this);
    }
    void remove_figure(std::vector<std::size_t>& pos);

    //const methods
    std::vector<Figure*> get_set() const {return set_of_figures_;}
    bool is_occupied(std::vector<std::size_t> &pos) const;
    void print_board() const;
    Figure* find_figure(std::vector<std::size_t> &pos) const;

private:
    std::vector<Figure*> set_of_figures_;
};

void print_error(const std::string &error);

inline bool check_if_position_is_on_the_board(const std::vector<std::size_t>& pos);

std::queue<std::vector<std::size_t>> create_path(const std::vector<std::size_t>& pos, Directions direction);

void print_path(std::queue<std::vector<std::size_t>> path);

void game(Board& board);

Directions determine_direction_of_movement(const std::vector<std::size_t>& start, const std::vector<std::size_t>& end);
#endif //SANDBOX_CHESS_HPP
