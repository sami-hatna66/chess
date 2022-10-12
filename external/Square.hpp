#ifndef Square_hpp
#define Square_hpp

#include "Piece.hpp"
#include <memory>

// Forward declaration
class Piece;

class Square {
  private:
    std::shared_ptr<Piece> piece;
    int row;
    int col;

  public:
    Square(int pRow = 0, int pCol = 0, std::shared_ptr<Piece> pPiece = nullptr);
    std::shared_ptr<Piece> getPiece();
    void setPiece(std::shared_ptr<Piece> newPiece);
    int getRow();
    int getCol();
};

#endif
