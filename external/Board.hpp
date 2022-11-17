#ifndef Board_hpp
#define Board_hpp

#include "Piece.hpp"
#include "Square.hpp"
#include <array>

// Forward Declarations
class Square;
class Piece;
enum class pieceColor;

enum class gameStatus {
    startScreen,
    inProgress,
    blackCheck,
    whiteCheck,
    blackCheckmate,
    whiteCheckmate,
    choosingPromotion
};

class Board : public std::enable_shared_from_this<Board> {
  private:
    std::array<std::array<std::shared_ptr<Square>, 8>, 8> squares;

  public:
    Board();
    Board(std::array<std::array<std::shared_ptr<Square>, 8>, 8> pSquares);
    void printBoard();
    void resetBoard();
    std::shared_ptr<Square> getSquare(int row, int col);
    std::array<std::array<std::shared_ptr<Square>, 8>, 8> getSquares();
    std::array<std::array<std::shared_ptr<Square>, 8>, 8> *getBoard();
    void movePiece(std::shared_ptr<Square> start, std::shared_ptr<Square> end,
                   std::shared_ptr<Piece> piece);
    gameStatus isCheck(pieceColor side);
    gameStatus isCheckMate(pieceColor colorInCheck);
};

#endif
