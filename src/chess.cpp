//
// Created by Milosz on 21.08.2020.
//
#include "chess.hpp"
#include <algorithm>

// FIGURE DEFINITIONS
bool Figure::move(std::vector<size_t> pos) {

    //Check if position is in range of the board
    if(!check_if_position_is_on_the_board(pos)) {
        print_error("Position is not on the board!");
        return false;
    }

    std::queue<std::vector<std::size_t>> queue_of_path = create_path_of_movement(pos);

    if(queue_of_path.empty()){
        print_error("This movement is not available");
        return false;
    }

    bool final_square = false;
    for(;!queue_of_path.empty(); queue_of_path.pop()){
        if(queue_of_path.front() == pos) final_square = true;

        // Check if the square is occupied
        if(get_board()->is_occupied(queue_of_path.front())){
            // Check if the square is occupied by an enemy and player wants to actually strike
            if(final_square && get_board()->find_figure(queue_of_path.front())->get_colour() != get_colour()){
                get_board()->remove_figure(pos);
                change_pos(pos);
                return true;
            }
                // The figure that occupies the square is white or the square is not final
            else{
                print_error("The way is blocked");
                return false;
            }
        }
            // If the square is not occupied
        else if(final_square){
            change_pos(pos);
            return true;
        }
    }
    // If none of elements in the path matches the desired position
    print_error("The move is not possible");
    return false;
}

bool Figure::try_to_strike(std::vector<std::size_t> pos) const{

    //Check if position is in range of the board
    if (!check_if_position_is_on_the_board(pos)) {
        print_error("Position is not on the board!");
        return false;
    }

    std::queue<std::vector<std::size_t>> available_path;

    switch (get_id()) {
        case pawn: {
            //Create the possible path of movement based on chosen destination position
            std::queue<std::vector<std::size_t>> queue = create_path_of_movement(pos);

            //Queue is empty when the destination position is out of the range of the pawn on X axis
            if (queue.empty()) return false;

            // The pawn wants to move diagonal when x coord in dest_position is different from the x coord in actual position
            bool is_striking = false;
            if (pos[1] != get_x_pos()) is_striking = true;

            bool final_square = false;
            // Trace the path in order to asses the validity of movement
            while (!queue.empty()) {
                if (queue.front() == pos) final_square = true;
                std::vector<std::size_t> path_elem = queue.front();
                // Is striking?
                if (is_striking) {
                    // Is there anything to strike?
                    if (get_board()->is_occupied(path_elem)) {
                        // The square is occupied by an enemy and the destination is final?
                        if (final_square && get_board()->find_figure(path_elem)->get_colour() != get_colour()) {
                            return true;
                        }
                            // The square is occupied by an ally or the square is not the desired destination
                        else {
                            print_error("The way is blocked!");
                            return false;
                        }
                    }
                        // The place is not occupied so u cannot go diagonal with a pawn
                    else {
                        print_error("You cannot go diagonal with a pawn w/o striking a figure of opposite colour!");
                        return false;
                    }
                }
                    // The pawn is moving forward
                    // Check if the way is blocked
                else if (get_board()->is_occupied(path_elem)) {
                    print_error("The way is blocked!");
                    return false;
                }
                    // Check if the current path element is the final destination
                else if (final_square) {
                    return false;
                }
                queue.pop();
            }
            print_error("This move is not possible!");
            return false;
        }
        case rook: case bishop: case queen: {
            std::queue<std::vector<std::size_t>> queue_of_path = create_path_of_movement(pos);

            if (queue_of_path.empty()) {
                return false;
            }

            bool final_square = false;
            for(;!queue_of_path.empty(); queue_of_path.pop()) {
                if (queue_of_path.front() == pos) final_square = true;

                // Check if the square is occupied
                if (get_board()->is_occupied(queue_of_path.front())) {
                    // Check if the square is occupied by an enemy and player wants to actually strike
                    if (final_square && get_board()->find_figure(queue_of_path.front())->get_colour() != get_colour()) {
                        return true;
                    }
                    // The figure that occupies the square is white or the square is not final
                    else {
                        return false;
                    }
                }
                    // If the square is not occupied
                else if (final_square) {
                    return false;
                }
            }
            // If none of elements in the path matches the desired position
            return false;
        }
        case knight: {
            //Create a queue of available positions
            available_path = create_path(get_pos(), HORSE);

            for(; !available_path.empty(); available_path.pop()){
                // Check if the position is the one that's wanted AND
                // there's actually something to strike AND
                // it is of opposite colour
                if(available_path.front() == pos &&
                        get_board()->is_occupied(pos) &&
                        get_board()->find_figure(pos)->get_colour() != this->get_colour()){
                    return true;
                }
            }
            return false;
        }
    }
    return  false;
}


// PAWN DEFINITIONS
bool Pawn::move(std::vector<size_t> pos) {
    //Check if position is in range of the board
    if(!check_if_position_is_on_the_board(pos)) {
        print_error("Position is not on the board!");
        return false;
    }

    //Create the possible path of movement based on chosen destination position
    std::queue<std::vector<std::size_t>> queue = create_path_of_movement(pos);

    //Queue is empty when the destination position is out of the range of the pawn on X axis
    if(queue.empty()) return false;

    // The pawn wants to move diagonal when x coord in dest_position is different from the x coord in actual position
    bool is_striking = false;
    if(pos[1] != get_x_pos()) is_striking = true;

    bool final_square = false;
    // Trace the path in order to asses the validity of movement
    while(!queue.empty()){
        if(queue.front() == pos) final_square = true;
        std::vector<std::size_t> path_elem = queue.front();
        // Is striking?
        if(is_striking){
            // Is there anything to strike?
            if(get_board()->is_occupied(path_elem)){
                // The square is occupied by an enemy and the destination is final?
                if(final_square && get_board()->find_figure(path_elem)->get_colour() != get_colour()) {
                    get_board()->remove_figure(pos);
                    change_pos(pos);
                    return true;
                }
                // The square is occupied by an ally or the square is not the desired destination
                else {
                    print_error("The way is blocked!");
                    return false;
                }
            }
            // The place is not occupied so u cannot go diagonal with a pawn
            else {
                print_error("You cannot go diagonal with a pawn w/o striking a figure of opposite colour!");
                return false;
            }
        }
        // The pawn is moving forward
        // Check if the way is blocked
        else if(get_board()->is_occupied(path_elem)){
            print_error("The way is blocked!");
            return false;
        }
        // Check if the current path element is the final destination
        else if(final_square){
            change_pos(pos);
            return true;
        }
        queue.pop();
    }
    print_error("This move is not possible!");
    return false;
}

std::queue<std::vector<std::size_t>> Pawn::create_path_of_movement (const std::vector<size_t>& pos) const {

    std::queue<std::vector<std::size_t>> queue_of_paths;
    const int relative_x = static_cast<int>(get_x_pos()) - static_cast<int>(pos[1]);
    const std::size_t y_pos = get_y_pos();
    const std::size_t x_pos = get_x_pos();

    switch (relative_x) {
        case 1:

            if(get_colour()) queue_of_paths.push({y_pos - 1, x_pos - 1});
            else queue_of_paths.push({y_pos + 1, x_pos - 1});
            return queue_of_paths;

        case -1:

            if(get_colour()) queue_of_paths.push({y_pos - 1, x_pos + 1});
            else queue_of_paths.push({y_pos + 1, x_pos - 1});
            return queue_of_paths;

        case 0:

            if(get_colour()) {
                queue_of_paths.push({y_pos - 1, x_pos});
                if (how_many_moves() == 0) queue_of_paths.push({y_pos - 2, x_pos});
            }
            else{
                queue_of_paths.push({y_pos + 1, x_pos});
                if (how_many_moves() == 0) queue_of_paths.push({y_pos + 2, x_pos});
            }
            return queue_of_paths;

        default:

            print_error("Position out of range of the pawn!");
            return queue_of_paths;
    }
}

// KNIGHT DEFINITIONS
bool Knight::move(std::vector<size_t> pos){

    //Create a queue of available positions
    std::queue<std::vector<std::size_t>> available = create_path(get_pos(), HORSE);

    for(; !available.empty(); available.pop()){
        // Check if the position is the one that's wanted
        if(available.front() == pos){
            //Check if that wanted position is occupied
            if(get_board()->is_occupied(pos)){
                // If occupied check the colour of the occupying figure
                if(get_board()->find_figure(pos)->get_colour() != this->get_colour()){
                    // If the colours differ then strike the figure and move
                    get_board() -> remove_figure(pos);
                    change_pos(pos);
                    return true;
                }
                // If colours match then print error
                else{
                    print_error("The square is occupied with a figure of the same colour!");
                    return false;
                }
            }
            // If not occupied then move
            else{
                change_pos(pos);
                return true;
            }
        }
    }

    print_error("The desired position is out of the range of the knight!");
    return false;
}

// ROOK DEFINITIONS
std::queue<std::vector<std::size_t>> Rook::create_path_of_movement(const std::vector<size_t>& pos) const{
    std::queue<std::vector<std::size_t>> queue_of_paths = {};

    Directions direction = determine_direction_of_movement(this->get_pos(), pos);
    switch (direction) {
        case NW: case SW: case NE: case SE:
            return queue_of_paths;
        default:
            return create_path(this->get_pos(), direction);
    }
}

// BISHOP DEFINITIONS
std::queue<std::vector<std::size_t>> Bishop::create_path_of_movement(const std::vector<size_t>& pos) const{
    std::queue<std::vector<std::size_t>> queue_of_paths = {};

    Directions direction = determine_direction_of_movement(this->get_pos(), pos);
    switch (direction) {
        case N: case S: case E: case W:
            return queue_of_paths;
        default:
            return create_path(this->get_pos(), direction);
    }
}

// QUEEN DEFINITIONS
std::queue<std::vector<std::size_t>> Queen::create_path_of_movement(const std::vector<size_t>& pos) const{

    Directions direction = determine_direction_of_movement(this->get_pos(), pos);
    return create_path(this->get_pos(), direction);
}

// KING DEFINITIONS
bool King::move(std::vector<size_t> pos) {
    // Make a copy of king's actual position
    std::vector<std::size_t> original_pos = get_pos();
    Figure* temporary_bin;
    bool is_occupied;

    // If the square that kings want to move to is occupied temporary remove the figure from the board after checking
    // what colour it was
    if((is_occupied = get_board()->is_occupied(pos))){
        if(get_board()->find_figure(pos)->get_colour() != this->get_colour()) {
            temporary_bin = get_board()->find_figure(pos);
            get_board()->remove_figure(pos);
        }
        else{
            print_error("The way is blocked by figure of the same colour!");
            return false;
        }
    }

    // Move king to his desired position
    Figure::move(pos);

    // Find out if after moving king is still checked - if yes then return the previous state of the board
    if(is_checked()){
        print_error("The move would result in king getting checked!");
        Figure::move(original_pos);
        if(is_occupied) get_board()->add_to_board(*temporary_bin);
        return false;
    }
    return true;
}

std::queue<std::vector<std::size_t>> King::create_path_of_movement(const std::vector<size_t> &pos) const {
    std::queue<std::vector<std::size_t>> path;

    Directions direction = determine_direction_of_movement(this->get_pos(), pos);

    switch(direction){
        case N:
            path.push({get_y_pos() - 1, get_x_pos()});
            break;
        case S:
            path.push({get_y_pos() + 1, get_x_pos()});
            break;
        case NW:
            path.push({get_y_pos() - 1, get_x_pos() - 1});
            break;
        case NE:
            path.push({get_y_pos() - 1, get_x_pos() + 1});
            break;
        case SE:
            path.push({get_y_pos() + 1, get_x_pos() + 1});
            break;
        case SW:
            path.push({get_y_pos() + 1, get_x_pos() - 1});
            break;
        case E:
            path.push({get_y_pos(), get_x_pos() + 1});
            break;
        case W:
            path.push({get_y_pos(), get_x_pos() - 1});
            break;
        default:
            break;
    }

    return path;
}

bool King::is_checked(std::vector<std::size_t> pos) const {
    if(pos.empty()) pos = get_pos();

    std::vector<std::queue<std::vector<std::size_t>>> vector_of_paths{
        create_path(pos, N),
        create_path(pos, NE),
        create_path(pos, E),
        create_path(pos, SE),
        create_path(pos, S),
        create_path(pos, SW),
        create_path(pos, W),
        create_path(pos, NW),
        create_path(pos, HORSE),
    };

    for(auto& i : vector_of_paths){
        for(; !i.empty(); i.pop()){
            // If the square that has access to the king contains a hostile figure check if it can strike the king
            //Check if the square is occupied
            if(get_board()->is_occupied(i.front())){
                // If it accidentally chose the king then continue
                if(get_board()->find_figure(i.front()) == this) continue;
                // If the figure is of opposite colour and it can strike the king then return true
                if(get_board()->find_figure(i.front())->get_colour() != get_colour() &&
                        get_board()->find_figure((i.front()))->try_to_strike(pos)) return true;
                else break;
            }
        }
    }
    return false;
}

// BOARD DEFINITIONS
bool Board::is_occupied(std::vector<std::size_t> &pos) const {
    return !none_of(set_of_figures_.begin(), set_of_figures_.end(),
                   [pos](const Figure* i){return i->get_pos() == pos;});
}

void Board::print_board() const {

    std::vector<std::vector<std::size_t>> matrix(8, std::vector<std::size_t>(8, 0));

    for(const auto& i : set_of_figures_){
        std::size_t id = i->get_id();
        if(!i->get_colour()) id += 6;
        matrix[i->get_y_pos()][i->get_x_pos()] = id;
    }

    std::string set_of_literals_for_figures = "-phbrqkPHBRQK";

    std::cout << std::endl;
    std::cout << "    ";
    for(char i = 65; i < 73; ++i){
        std::cout << i << "   ";
        if( i == 'H') std::cout << std::endl;
    }

    for(std::size_t i = 0, nr = 0; i < 8; ++i){
        std::cout << ++nr << "   ";
        for(std::size_t j = 0; j < 8; ++j){
            std::cout << set_of_literals_for_figures[matrix[i][j]] << "   ";
            if(j == 7) std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

Figure* Board::find_figure(std::vector<std::size_t> &pos) const {
    for(const auto& i : get_set()){
        if(i->get_pos() == pos) return i;
    }
    return nullptr;
}

void Board::remove_figure(std::vector<std::size_t>& pos) {
    for(auto it = set_of_figures_.begin();
        it != set_of_figures_.end();
        ++it){
        if((*it)->get_pos() == pos){
            set_of_figures_.erase(it);
            return;
        }
    }
}

//EXTERNAL DEFINITIONS
void print_error(const std::string &error){
    std::cout << error << std::endl;
}

bool check_if_position_is_on_the_board(const std::vector<std::size_t> &pos) {
    return !(pos[0] > 7 || pos[1] > 7);
}

std::queue<std::vector<std::size_t>> create_path(const std::vector<std::size_t>& pos, Directions direction){

    std::queue<std::vector<std::size_t>> path;

    switch (direction) {
        case N:
            for(std::size_t i = pos[0]; i >= 1; --i){
                path.push({i - 1, pos[1]});
            } break;
        case NE:
            for(std::size_t i = pos[0], j = pos[1]; i >= 1 && j <= 6; --i, ++j){
                path.push({i - 1, j + 1});
            } break;
        case E:
            for(std::size_t i = pos[1]; i <= 6; ++i){
                path.push({pos[0], i + 1});
            } break;
        case SE:
            for(std::size_t i = pos[0], j = pos[1]; i <= 6 && j <= 6; ++i, ++j){
                path.push({i + 1, j + 1});
            } break;
        case S:
            for(std::size_t i = pos[0]; i <= 6; ++i){
                path.push({i + 1, pos[1]});
            } break;
        case SW:
            for(std::size_t i = pos[0], j = pos[1]; i <= 6 && j >= 1; ++i, --j){
                path.push({i + 1, j - 1});
            } break;
        case W:
            for(std::size_t i = pos[1]; i >= 1; --i){
                path.push({pos[0], i - 1});
            } break;
        case NW:
            for(std::size_t i = pos[0], j = pos[1]; i >= 1 && j >= 1; --i, --j){
                path.push({i - 1, j - 1});
            } break;
        case HORSE:
            std::vector<std::vector<std::size_t>> temp = {
                    {pos[0] + 1, pos[1] - 2},
                    {pos[0] + 2, pos[1] - 1},
                    {pos[0] + 2, pos[1] + 1},
                    {pos[0] + 1, pos[1] + 2},
                    {pos[0] - 1, pos[1] - 2},
                    {pos[0] - 2, pos[1] - 1},
                    {pos[0] - 2, pos[1] + 1},
                    {pos[0] - 1, pos[1] + 2},
            };
            for(auto& i : temp) {
                if(check_if_position_is_on_the_board(i)) path.push(i);
            } break;
    }
    return path;
}

void print_path(std::queue<std::vector<std::size_t>> path) {
    while(!path.empty()){
        std::cout << path.front()[0] << path.front()[1] << std::endl;
        path.pop();
    }
}

Directions determine_direction_of_movement(const std::vector<std::size_t> &start, const std::vector<std::size_t> &end){
    int relative_y_pos = static_cast<int>(start[0]) - static_cast<int>(end[0]);
    int relative_x_pos = static_cast<int>(start[1]) - static_cast<int>(end[1]);

    if(relative_y_pos > 0){
        if(relative_x_pos > 0) return NW;
        else if(relative_x_pos == 0) return N;
        else return NE;
    }
    else if(relative_y_pos == 0){
        if(relative_x_pos > 0) return W;
        else return E;
    }
    else {
        if(relative_x_pos > 0) return SW;
        else if(relative_x_pos == 0) return S;
        else return SE;
    }
}

// MAIN GAME FUNCTION
void game(Board& board){
    bool now_moving = white;
    char temporary;
    Figure* chosen_now;
    std::vector<std::size_t> destination(2, 0);

    std:: map<char, std::size_t> board_position{
            {'A', 0},
            {'B', 1},
            {'C', 2},
            {'D', 3},
            {'E', 4},
            {'F', 5},
            {'G', 6},
            {'H', 7},
    };

    // The proper game loop
    while(true){
        // Print the current state of the board
        board.print_board();

        // Prompt user to give the position of figure he wants to move with
        std::cout << "Enter a position of figure that u want to make a move with: " << std::endl;
        std::cin >> temporary >> destination[0];
        --destination[0];
        destination[1] = board_position[temporary];

        //Ending condition for now
        if(destination[0] == 8) break;

        // Check if the selection was correct and "pick" the appointed figure
        if(board.is_occupied(destination)){
            if(board.find_figure(destination)->get_colour() == now_moving){
                chosen_now = board.find_figure(destination);
            }
            else{
                std::string colour = "white";
                if(now_moving == white);
                else colour = "black";
                std::cout << "Now it is " << colour << "'s turn!" << std::endl;
                continue;
            }
        }
        else{
            print_error("Theres no figure on the chosen position!");
            continue;
        }

        // Prompt user to enter the position he wants to move to
        std::cout << "Enter a position that you want to move the figure to: " << std::endl;
        std::cin >> temporary >> destination[0];
        --destination[0];
        destination[1] = board_position[temporary];

        // Try to move to appointed position and if the move succeeded change the player that's moving
        if(chosen_now->move(destination)) now_moving = !now_moving;

        // Check if the king is checked and inform the player about it
        for(const auto& i : board.get_set()){
            if(i->get_id() == king){
                auto king_ptr = dynamic_cast<King*>(i);
                if(king_ptr->is_checked()){
                    std::cout << "The king is checked!!" << std::endl;
                }
            }
        }
    }
}

