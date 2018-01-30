#include<string>
#include<vector>

using namespace std;

class ChessPiece;
class Point;

/* Handles all board and game management. */
class ChessBoard {
private:
  ChessPiece* chess_map[8][8];
  int moves_made;
  char current_turn;

public:
  /* -------------------- Constructors -------------------- */
  ChessBoard();

  void initialise_board();

  /* -------------------- Game management -------------------- */
  /* Perform move on chess board. Print move/error message. */
  void submitMove(const char from[], const char to[]);
  /* Remove all pieces from chess board */
  void resetBoard();
  /* Print the current chess maps */
  void printBoard();


private:
  /* -------------------- Helpers -------------------- */
  /* If pawn was moved, tell it to update move counter */
  void remember_pawn_move(Point from_pos);

  /* Return true if a move from_pos to_pos is valid */
  bool valid_move(Point from_pos, Point to_pos, bool print_errors);

  /* Return true if it is the current player's turn */
  bool check_turn(Point from_pos);

  /* Move a piece on the chessboard and update piece's internal location */
  ChessPiece* move_piece(Point from_pos, Point to_pos, bool print_message = true);

  /* Return true if the path between to positions on the board is blocked */
  bool blocked_path(Point from_pos, Point to_pos);

  /* Save a direct route between two positions to path vector */
  void find_route(Point from_pos, Point to_pos, vector<Point> &path);

  /* Print the contents of the path vector */
  void print_path(vector<Point> path);

  /* Calculate the directon of travel between two positions on the board */
  int step(int diff);

  /* Save locations of the two kings to the positions provided */
  void find_kings(Point &white_king, Point &black_king);

  /* Return true if a king is in check and print message */
  bool check();

  /* Find all positions to which the piece at from_pos can move */
  void find_all_moves(Point from_pos, vector<Point> &possible_moves);

  /* Simulate move and return true if would put king in check */
  bool simulate_move_check(Point from_pos, Point to_pos, Point king_location);

  /* Return true if would would put current player's king in check */
  bool move_to_check(Point from_pos, Point to_pos);

  /* Return true if a king is in chekmate and print notice */
  bool check_mate(Point king_in_check);

  /* Return true the the king at king_location is in check */
  bool in_check(Point king_location);

  /* Return true if king incheck from opponent with attacker colour */
  bool in_check(Point king_location, char attacker_colour);

  /* Return true if game is in stalemate */
  bool stalemate();

};

/* -------------------- ChessPiece -------------------- */
/* Handles chess piece represenation and properties. Does not alter the board. */
class ChessPiece {
protected:
  char type;
  int rank;
  int file;
  char colour;
  bool in_play;
  string colour_long;
  string name;

  /* Return true is position is not on the board */
  bool out_of_bounds(Point position);

  /* Print destination out of bounds error message */
  void print_bounds_error(Point position);

public:
  /* Print position movement error message */
  void print_move_error(Point position);

  /* -------------------- Constructors -------------------- */
  ChessPiece(char type, int rank, int file, char colour);
  virtual ~ChessPiece();

  /* -------------------- Helpers -------------------- */
  friend ostream& operator<<(ostream& os, const ChessPiece& cp);

  /* Return true if piece can take position (different colour, not check etc.) */
  bool can_take(ChessPiece* other_piece, Point to_position);

  /* Change piece's internal location to reflect empty to_position and print message */
  void take_position(Point from_position, Point to_position, bool print_message);

  /* Change piece's internal location to reflect to_position and print message */
  void take_position(Point from_position, Point to_position, ChessPiece *other_piece, bool print_message);

  /* Return the piece's colour */
  char get_colour();

  /* Return the piece's type */
  char get_type();

  /* -------------------- Virtual Functions -------------------- */
  /* Return true if piece is able to move to location */
  virtual bool valid_destination(Point to_position) = 0;
};


/* -------------------- Pawn -------------------- */
class Pawn : public ChessPiece {
private:
  bool first_move;

public:
  /* -------------------- Constructors -------------------- */
  Pawn(char type, int rank, int file, char colour);
  virtual ~Pawn();

  /* -------------------- Public Helper -------------------- */
  void check_first_move();

  /* -------------------- Virtual Functions -------------------- */
  /* Return true if Pawn is able to move to location */
  virtual bool valid_destination(Point to_position);
};


/* -------------------- Knight -------------------- */
class Knight : public ChessPiece {
public:
  /* -------------------- Constructors -------------------- */
  Knight(char type, int rank, int file, char colour);
  virtual ~Knight();

  /* -------------------- Virtual Functions -------------------- */
  /* Return true if Knight is able to move to location */
  virtual bool valid_destination(Point to_position);
};


/* -------------------- Queen -------------------- */
class Queen : public ChessPiece {
public:
  /* -------------------- Constructors -------------------- */
  Queen(char type, int rank, int file, char colour);
  virtual ~Queen();

  /* -------------------- Virtual Functions -------------------- */
  /* Return true if Queen is able to move to location */
  virtual bool valid_destination(Point to_position);
};


/* -------------------- Bishop -------------------- */
class Bishop : public ChessPiece {
public:
  /* -------------------- Constructors -------------------- */
  Bishop(char type, int rank, int file, char colour);
  virtual ~Bishop();

  /* -------------------- Virtual Functions -------------------- */
  /* Return true if Bishop is able to move to location */
  virtual bool valid_destination(Point to_position);
};


/* -------------------- Castle -------------------- */
class Castle : public ChessPiece {
public:
  /* -------------------- Constructors -------------------- */
  Castle(char type, int rank, int file, char colour);
  virtual ~Castle();

  /* -------------------- Virtual Functions -------------------- */
  /* Return true if Castle is able to move to location */
  virtual bool valid_destination(Point to_position);
};


/* -------------------- King -------------------- */
class King : public ChessPiece {
public:
  /* -------------------- Constructors -------------------- */
  King(char type, int rank, int file, char colour);
  virtual ~King();

  /* -------------------- Virtual Functions -------------------- */
  /* Return true if King is able to move to location */
  virtual bool valid_destination(Point to_position);
};


/* Represents points on a chess board */
class Point {
private:
  int rank;
  int file;
  string position;

public:
  /* -------------------- Constructors -------------------- */
  Point(const char input_position[]);
  Point(int rank, int file);
  Point();

  /* -------------------- Helpers -------------------- */
  friend ostream& operator<<(ostream& os, const Point& p);
  friend bool operator==(const Point &p1, const Point &p2);
  /* Return the point's rank */
  int get_rank();

  /* Return the point's file */
  int get_file();
};
