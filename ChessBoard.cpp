#include<iostream>
#include<iomanip>
#include<cmath>

using namespace std;

#include"ChessBoard.h"


/* -------------------- ChessBoard -------------------- */
/* -------------------- Constructor -------------------- */
ChessBoard::ChessBoard() : moves_made(0), current_turn('W') {
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      chess_map[rank][file] = nullptr;
    }
  }

  initialise_board();
}

void ChessBoard::initialise_board() {
  /* Create Pawns */
  for (int file = 0; file < 8; file++) {
    chess_map[1][file] = new Pawn('P', 1, file, 'B');
    chess_map[6][file] = new Pawn('P', 6, file, 'W');
  }

  /* Create Knights */
  chess_map[0][1] = new Knight('N', 0, 1, 'B');
  chess_map[0][6] = new Knight('N', 0, 6, 'B');
  chess_map[7][1] = new Knight('N', 7, 1, 'W');
  chess_map[7][6] = new Knight('N', 7, 6, 'W');

  /* Create Queens */
  chess_map[0][3] = new Queen('Q', 0, 3, 'B');
  chess_map[7][3] = new Queen('Q', 7, 3, 'W');

  /* Create Bishops */
  chess_map[0][2] = new Bishop('B', 0, 2, 'B');
  chess_map[0][5] = new Bishop('B', 0, 5, 'B');
  chess_map[7][2] = new Bishop('B', 7, 2, 'W');
  chess_map[7][5] = new Bishop('B', 7, 5, 'W');

  /* Create Castles */
  chess_map[0][0] = new Castle('C', 0, 0, 'B');
  chess_map[0][7] = new Castle('C', 0, 7, 'B');
  chess_map[7][0] = new Castle('C', 7, 0, 'W');
  chess_map[7][7] = new Castle('C', 7, 7, 'W');

  /* Create Kings */
  chess_map[0][4] = new King('K', 0, 4, 'B');
  chess_map[7][4] = new King('K', 7, 4, 'W');

  cout << "A new chess game is started!" << endl;
}

/* -------------------- Game management -------------------- */
void ChessBoard::submitMove(const char from[], const char to[]) {
  Point from_pos(from);
  Point to_pos(to);
  ChessPiece* removed_piece;

  // check from_pos not empty
  if (chess_map[from_pos.get_rank()][from_pos.get_file()] == NULL) {
    cout << "There is no piece at position " << from_pos << "!" << endl;
    return;
  }

  if (!check_turn(from_pos))
    return;

  if (!valid_move(from, to, true))
    return;

  // check the move won't put current player in check
  if (move_to_check(from_pos, to_pos))
    return;

  // if a pawn is to be moved, tell it to remember
  remember_pawn_move(from_pos);

  removed_piece = move_piece(from_pos, to_pos);
  delete removed_piece;

  // check for check, checkmate and stalemate
  if (!check())
    stalemate();
}

void ChessBoard::remember_pawn_move(Point from_pos) {
  ChessPiece* piece_moved = chess_map[from_pos.get_rank()][from_pos.get_file()];

  if (piece_moved->get_type() == 'P')
    static_cast<Pawn*>(piece_moved)->check_first_move();
}

ChessPiece* ChessBoard::move_piece(Point from_pos, Point to_pos, bool print_message) {
  ChessPiece* tmp = nullptr;

  // if destination empty, update moving piece and print message
  if (chess_map[to_pos.get_rank()][to_pos.get_file()] == NULL)
    chess_map[from_pos.get_rank()][from_pos.get_file()]->take_position(from_pos, to_pos, print_message);

  // else update taking piece, print message and delete taken piece
  else  {
    chess_map[from_pos.get_rank()][from_pos.get_file()]->take_position(from_pos, to_pos, chess_map[to_pos.get_rank()][to_pos.get_file()], print_message);
    tmp = chess_map[to_pos.get_rank()][to_pos.get_file()];
  }

  // update board
  chess_map[to_pos.get_rank()][to_pos.get_file()] = chess_map[from_pos.get_rank()][from_pos.get_file()];
  chess_map[from_pos.get_rank()][from_pos.get_file()] = NULL;

  return tmp;
}

void ChessBoard::resetBoard() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      delete chess_map[i][j];
      chess_map[i][j] = nullptr;
    }
  }

  moves_made = 0;
  current_turn = 'W';
  initialise_board();
}

/* -------------------- Helpers -------------------- */
bool ChessBoard::valid_move(Point from_pos, Point to_pos, bool print_errors) {
  bool good_move = true;

  // check valid destination
  if (!(chess_map[from_pos.get_rank()][from_pos.get_file()]->valid_destination(to_pos)))
    good_move = false;

  // check blocked path
  else if (blocked_path(from_pos, to_pos))
    good_move = false;

  // if piece can take other piece, move
  else if (!chess_map[from_pos.get_rank()][from_pos.get_file()]->can_take(chess_map[to_pos.get_rank()][to_pos.get_file()], to_pos))
    good_move = false;

  // print error if needed
  if ((good_move == false) && (print_errors == true))
    chess_map[from_pos.get_rank()][from_pos.get_file()]->print_move_error(to_pos);
  return good_move;
}

bool ChessBoard::check_turn(Point from_pos) {
  if ((moves_made % 2) == 0) {
    if (chess_map[from_pos.get_rank()][from_pos.get_file()]->get_colour() != 'W') {
      cout << "It's not Black's turn to move!" << endl;
      current_turn = 'W';
      return false;
    }
  }

  else if (chess_map[from_pos.get_rank()][from_pos.get_file()]->get_colour() != 'B') {
    cout << "It's not White's turn to move!" << endl;
    current_turn = 'W';
    return false;
  }

  moves_made ++;
  return true;
}

void ChessBoard::printBoard() {
  cout << "Current board: " << endl;
  cout << setiosflags (ios::left);
  cout.setf(ios::fixed);

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (chess_map[i][j] == NULL)
        cout << setw(9) << "";
      else
        cout << setw(9) << *chess_map[i][j];
    }
    cout << endl;
  }
  cout << endl;
}

bool ChessBoard::blocked_path(Point from_pos, Point to_pos) {
  if (chess_map[from_pos.get_rank()][from_pos.get_file()]->get_type() == 'N')
    return false;

  vector<Point> path;
  find_route(from_pos, to_pos, path);

  for (auto point : path) {
    if (chess_map[point.get_rank()][point.get_file()] != NULL) {
      return true;
    }
  }
  return false;
}

void ChessBoard::find_route(Point from_pos, Point to_pos, vector<Point> &path) {
  int rank_temp;
  int file_temp;

  int rank_diff = to_pos.get_rank() - from_pos.get_rank();
  int file_diff = to_pos.get_file() - from_pos.get_file();

  if (max(abs(rank_diff), abs(file_diff)) == 1)
    return;

  int rank_step = step(rank_diff);
  int file_step = step(file_diff);

  rank_temp = from_pos.get_rank() + rank_step;
  file_temp = from_pos.get_file() + file_step;

  path.push_back(Point(rank_temp, file_temp));

  for (int i = 1; i < max(abs(rank_diff), abs(file_diff)) - 1; i++) {
    rank_temp = path[i - 1].get_rank() + rank_step;
    file_temp = path[i - 1].get_file() + file_step;

    path.push_back(Point(rank_temp, file_temp));
  }
}

int ChessBoard::step(int diff) {
  if (diff > 0) return 1;
  if (diff < 0) return -1;
  return 0;
}

void ChessBoard::print_path(vector<Point> path) {
  cout << "Path: ";
  for (auto point : path) {
    cout << point;
  }
  cout << endl;
}

void ChessBoard::find_kings(Point &white_king, Point &black_king) {
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      if (chess_map[rank][file] == NULL)
        continue;
      else if ((chess_map[rank][file]->get_type() == 'K') && (chess_map[rank][file]->get_colour() == 'B'))
        black_king = Point(rank, file);
      else if ((chess_map[rank][file]->get_type() == 'K') && (chess_map[rank][file]->get_colour() == 'W'))
        white_king = Point(rank, file);
      }
    }
  }

bool ChessBoard::in_check(Point king_location, char king_colour) {
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      if (chess_map[rank][file] == NULL)
        continue;
      else if (chess_map[rank][file]->get_colour() == king_colour)
        continue;

      else {
        Point attacker(rank, file);
        if (valid_move(attacker, king_location, false))
          return true;
        }
      }
    }
  return false;
}

bool ChessBoard::in_check(Point king_location) {
  char king_colour = chess_map[king_location.get_rank()][king_location.get_file()]->get_colour();

  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      if (chess_map[rank][file] == NULL)
        continue;
      else if (chess_map[rank][file]->get_colour() == king_colour)
        continue;

      else {
        Point attacker(rank, file);
        if (valid_move(attacker, king_location, false))
          return true;
        }
      }
    }

  return false;
}

bool ChessBoard::check() {
  Point white_king;
  Point black_king;

  find_kings(white_king, black_king);

  if (in_check(black_king)) {
    if (check_mate(black_king))
      cout << "Black is in checkmate" << endl;
    else cout << "Black is in check" << endl;
    return true;
  }
  else if (in_check(white_king)) {
    if (check_mate(white_king))
      cout << "White is in checkmate" << endl;
    else cout << "White is in check" << endl;
    return true;
  }

  return false;
}

void ChessBoard::find_all_moves(Point from_pos, vector<Point> &possible_moves) {
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      Point to_pos(rank, file);
      if (valid_move(from_pos, to_pos, false))
        possible_moves.push_back(to_pos);
    }
  }
}

bool ChessBoard::move_to_check(Point from_pos, Point to_pos) {
  Point black_king;
  Point white_king;
  bool simulation_result;

  find_kings(white_king, black_king);

  if (chess_map[from_pos.get_rank()][from_pos.get_file()]->get_colour() == 'W')
    simulation_result = simulate_move_check(from_pos, to_pos, white_king);
  else
    simulation_result = simulate_move_check(from_pos, to_pos, black_king);

  if (simulation_result)
    chess_map[from_pos.get_rank()][from_pos.get_file()]->print_move_error(to_pos);

  return simulation_result;
}

bool ChessBoard::simulate_move_check(Point from_pos, Point to_pos, Point king_location) {
  ChessPiece *current_piece;
  bool simulation_result = false;

  // simulate move
  current_piece = move_piece(from_pos, to_pos, false);

  // check still in check
  if (in_check(king_location))
    simulation_result = true;

  // reinstate the original pointer
  move_piece(to_pos, from_pos, false);
  chess_map[to_pos.get_rank()][to_pos.get_file()] = current_piece;

  return simulation_result;
}

bool ChessBoard::check_mate(Point king_in_check) {
  char attacker_colour = 'B';
  if (chess_map[king_in_check.get_rank()][king_in_check.get_file()]->get_colour() == 'B')
    attacker_colour = 'W';

  /* escape by moving king */
  // look at all points on board
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      Point escape_route(rank, file);

      // if king can move there
      if (valid_move(king_in_check, escape_route, false)) {

        // check if that positin is in check
        if (!in_check(escape_route, attacker_colour)) {
          return false;
        }
      }
    }
  }

  /* escape by moving other pieces */
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {

      if (chess_map[rank][file] == NULL)
        continue;
      else if (chess_map[rank][file]->get_colour() == attacker_colour)
        continue;
      else if (chess_map[rank][file]->get_type() == 'K')
        continue;

      Point from_pos(rank, file);
      vector<Point> possible_moves;
      find_all_moves(from_pos, possible_moves);

      for (auto i : possible_moves) {
        if (!simulate_move_check(from_pos, i, king_in_check))
          return false;
      }
    }
  }

  return true;
}

bool ChessBoard::stalemate() {
  vector<Point> possible_moves;

  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {

      if (chess_map[rank][file] == NULL)
        continue;
      if (chess_map[rank][file]->get_colour() != current_turn)
        continue;

      find_all_moves(Point(rank, file), possible_moves);

      if (!(possible_moves.empty()))
        return false;
      }
    }

  cout << "Game is in stalemate";
  return true;
}

/* -------------------- ChessPiece -------------------- */
/* -------------------- Constructor -------------------- */
ChessPiece::ChessPiece(char type, int rank, int file, char colour) : type(type), rank(rank), file(file), colour(colour) {
    switch(colour) {
      case 'B': colour_long = "Black";
                break;
      case 'W': colour_long = "White";
                break;
    }

    switch(type) {
      case 'P': name = "Pawn";
                break;
      case 'N': name = "Knight";
                break;
      case 'Q': name = "Queen";
                break;
      case 'B': name = "Bishop";
                break;
      case 'C': name = "Castle";
                break;
      case 'K': name = "King";
                break;
    }
};

ChessPiece::~ChessPiece() {};

/* -------------------- Helpers -------------------- */
ostream& operator<<(ostream& os, const ChessPiece& cp) {
  os << setw(0) << cp.type << cp.colour << "(" << cp.rank << "," << cp.file << ")" << "| ";
  return os;
}

bool ChessPiece::can_take(ChessPiece* other_piece, Point to_position) {
  if (other_piece == NULL)
    return true;

  if (type == 'P') {
    if (file == to_position.get_file())
      return false;
  }

  else if (other_piece->get_colour() == colour) {
    return false;
  }

  return true;
}

void ChessPiece::take_position(Point from_position, Point to_position, bool print_message) {
  rank = to_position.get_rank();
  file = to_position.get_file();
  if (print_message)
    cout << colour_long << "'s " << name << " moves from " << from_position << " to " << to_position << endl;
}

void ChessPiece::take_position(Point from_position, Point to_position, ChessPiece *other_piece, bool print_message) {
  rank = to_position.get_rank();
  file = to_position.get_file();
  if (print_message)
    cout << colour_long << "'s " << name << " moves from " << from_position << " to " << to_position << " taking " << other_piece->colour_long << "'s " << other_piece->name  << endl;
}

char ChessPiece::get_colour() {
  return colour;
}

char ChessPiece::get_type() {
  return type;
}

bool ChessPiece::out_of_bounds(Point position) {
  int rank = position.get_rank();
  int file = position.get_file();

  if ((rank < 0) || (file < 0) || (rank > 7) || (file > 7))
    return true;

  return false;
}

void ChessPiece::print_move_error(Point position) {
  cout << colour_long << "'s " << name << " cannot move to " << position << "!" << endl;
}

void ChessPiece::print_bounds_error(Point position) {
  cout << "Position " << position << " is out out of bounds!" << endl;
}


/* -------------------- Pawn -------------------- */
/* -------------------- Constructor -------------------- */
Pawn::Pawn(char type, int rank, int file, char colour) : ChessPiece(type, rank, file, colour), first_move(true) {};

Pawn::~Pawn() {};

/* -------------------- Helpers -------------------- */
void Pawn::check_first_move() {
  if (first_move == true)
    first_move = false;
}

bool Pawn::valid_destination(Point to_position) {
  bool valid = true;
  int rank_diff = to_position.get_rank() - rank;
  int file_diff = to_position.get_file() - file;

  if (out_of_bounds(to_position)) {
    print_bounds_error(to_position);
    return false;
  }

  // can't move sideways
   if (!((abs(file_diff) == 1) || (file_diff == 0))) {
     valid = false;
   }

   // can only move one or two squares forward
   else if (colour == 'B') {
     if (!((rank_diff == 1) || ((first_move) && rank_diff == 2)))
       valid = false;

     // can move diagonally
     else if ((abs(file_diff) == 1) && (rank_diff == 1))
       valid = true;
   }
   else if (colour == 'W') {
     if (!((rank_diff == -1) || ((first_move) && rank_diff == -2)))
       valid = false;

     // can move diagonally
     else if ((abs(file_diff) == 1) && (rank_diff == -1))
       valid = true;
   }

  return valid;
}


/* -------------------- Knight -------------------- */
/* -------------------- Constructor -------------------- */
Knight::Knight(char type, int rank, int file, char colour) : ChessPiece(type, rank, file, colour) {};

Knight::~Knight() {};

/* -------------------- Helpers -------------------- */
bool Knight::valid_destination(Point to_position) {
  bool valid = false;
  int rank_diff = to_position.get_rank() - rank;
  int file_diff = to_position.get_file() - file;

  if (out_of_bounds(to_position)) {
    print_bounds_error(to_position);
    return false;
  }

  else if (abs(rank_diff) == 2)
    valid = (abs(file_diff) == 1);
  else if (abs(rank_diff) == 1)
    valid = (abs(file_diff) == 2);

  return valid;
}


/* -------------------- Queen -------------------- */
/* -------------------- Constructor -------------------- */
Queen::Queen(char type, int rank, int file, char colour) : ChessPiece(type, rank, file, colour) {};

Queen::~Queen() {};

/* -------------------- Helpers -------------------- */
// TODO move error print ot helper function in Queen or even ChessPiece?
bool Queen::valid_destination(Point to_position) {
  bool valid = false;
  int rank_diff = to_position.get_rank() - rank;
  int file_diff = to_position.get_file() - file;

  if (out_of_bounds(to_position)) {
    print_bounds_error(to_position);
    return false;
  }

  // no movement
  if ((rank_diff == 0) && (file_diff == 0))
    valid = false;

  // castle behaviour - only on is zero
  else if ((rank_diff == 0) || (file_diff == 0))
    valid = true;

  // bishop behaviour - abs diff ==
  else if (abs(rank_diff) == abs(file_diff))
    valid = true;

  return valid;
}


/* -------------------- Bishop -------------------- */
/* -------------------- Constructor -------------------- */
Bishop::Bishop(char type, int rank, int file, char colour) : ChessPiece(type, rank, file, colour) {};

Bishop::~Bishop() {};

/* -------------------- Helpers -------------------- */
bool Bishop::valid_destination(Point to_position) {
  bool valid = false;
  int rank_diff = to_position.get_rank() - rank;
  int file_diff = to_position.get_file() - file;

  if (out_of_bounds(to_position)) {
    print_bounds_error(to_position);
    return false;
  }

  // no movement
  if ((rank_diff == 0) && (file_diff == 0))
    valid = false;

  // bishop behaviour - abs diff ==
  else if (abs(rank_diff) == abs(file_diff))
    valid = true;

  return valid;
}


/* -------------------- Castle -------------------- */
/* -------------------- Constructor -------------------- */
Castle::Castle(char type, int rank, int file, char colour) : ChessPiece(type, rank, file, colour) {};

Castle::~Castle() {};

/* -------------------- Helpers -------------------- */
bool Castle::valid_destination(Point to_position) {
  bool valid = false;
  int rank_diff = to_position.get_rank() - rank;
  int file_diff = to_position.get_file() - file;

  if (out_of_bounds(to_position)) {
    print_bounds_error(to_position);
    return false;
  }

  // no movement
  if ((rank_diff == 0) && (file_diff == 0))
    valid = false;

  // castle behaviour - only on is zero
  else if ((rank_diff == 0) || (file_diff == 0))
    valid = true;

  return valid;
}


/* -------------------- King -------------------- */
/* -------------------- Constructor -------------------- */
King::King(char type, int rank, int file, char colour) : ChessPiece(type, rank, file, colour) {};

King::~King() {};


/* -------------------- Helpers -------------------- */
bool King::valid_destination(Point to_position) {
  bool valid = false;
  int rank_diff = to_position.get_rank() - rank;
  int file_diff = to_position.get_file() - file;

  if (out_of_bounds(to_position)) {
    print_bounds_error(to_position);
    return false;
  }

  // no movement
  if ((rank_diff == 0) && (file_diff == 0))
    valid = false;

  else if ((abs(rank_diff) <= 1) && (abs(file_diff) <= 1)) {
    valid = true;
  }

  return valid;
}


/* -------------------- Point -------------------- */
/* -------------------- Constructor -------------------- */
Point::Point(const char input_position[]) : position(input_position) {
  rank = abs(static_cast<int>(position[1]) - 48 - 8);
  file = static_cast<int>(position[0]) - 65;
}

Point::Point(int rank, int file) : rank(rank), file(file), position("") {}

Point::Point() {}

/* -------------------- Helpers -------------------- */
ostream& operator<<(ostream& os, const Point& p) {
  if (p.position != "")
    os << p.position;
  else
    os << "(" << p.rank << "," << p.file << ")";
  return os;
}

bool operator==(const Point &p1, const Point &p2) {
  return ((p1.rank == p2.rank) && (p1.file == p2.file));
}

int Point::get_rank() {
  return rank;
}

int Point::get_file() {
  return file;
}