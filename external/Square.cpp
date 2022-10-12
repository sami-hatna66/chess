#include "Square.hpp"

Square::Square(int pRow, int pCol, std::shared_ptr<Piece> pPiece) {
    row = pRow;
    col = pCol;
    piece = std::move(pPiece);
}

std::shared_ptr<Piece> Square::getPiece() { return piece; }

void Square::setPiece(std::shared_ptr<Piece> newPiece) {
    piece = std::move(newPiece);
}

int Square::getRow() { return row; }

int Square::getCol() { return col; }
