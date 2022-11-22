#include "Piece.hpp"

//------------------------------------------------------------------------------------------------------------------------------------

Piece::Piece(pieceColor pColor) {
    color = pColor;
    alive = true;
    imageName = "null";
    pieceName = pieceType::Base;
}

bool Piece::isAlive() { return alive; }

void Piece::setAlive(bool pAlive) { alive = pAlive; }

pieceColor Piece::getColor() { return color; }

const char *Piece::getImageName() { return imageName; }

bool Piece::canMove(std::shared_ptr<Board> board, std::shared_ptr<Square> start,
                    std::shared_ptr<Square> end) {
    return false;
}

std::vector<std::pair<int, int>>
Piece::legalMoves(std::shared_ptr<Board> board,
                  std::shared_ptr<Square> currentSquare) {
    std::vector<std::pair<int, int>> result;

    auto piece = currentSquare->getPiece();
    int currentRow = currentSquare->getRow();
    int currentCol = currentSquare->getCol();
    
    // Iterate through squares and collect all possible moves
    // (doesn't consider whether moves result in check --> this is handled in game.turn)
    for (int i = 7; i>= 0; i--) {
        for (int j = 0; j < 8; j++) {
            if ((i != currentRow || j != currentCol) &&
                this->canMove(board, currentSquare, board->getSquare(i, j))) {
                    result.push_back(std::make_pair(i, j));
                }
        }
    }

    return result;
}

bool Piece::getIsFirstMove() { return false; }

void Piece::setIsFirstMove(bool pIsFirstMove) {}

bool Piece::isVulnerableToEnPassant() { return false; }

void Piece::setVulnerableToEnPassant(bool pVulnerability) {}

pieceType Piece::getPieceName() { return pieceName; }

bool Piece::getCanCastle() { return false; }

void Piece::setCanCastle(bool flag) {}

bool Piece::getCanPromote() { return false; }

void Piece::setCanPromote(bool newCanPromote) {}

//------------------------------------------------------------------------------------------------------------------------------------

Pawn::Pawn(pieceColor pColor, bool pIsFirstMove, bool pVulnerableToEnPassant)
    : Piece(pColor), isFirstMove(pIsFirstMove),
      vulnerableToEnPassant(pVulnerableToEnPassant) {
    isFirstMove = pIsFirstMove;
    imageName = pColor == pieceColor::white ? "assets/whtPawn.png"
                                            : "assets/blkPawn.png";
    pieceName = pieceType::Pawn;
    canPromote = false;
}

bool Pawn::getIsFirstMove() { return isFirstMove; }

void Pawn::setIsFirstMove(bool pIsFirstMove) { isFirstMove = pIsFirstMove; }

bool Pawn::canMove(std::shared_ptr<Board> board, std::shared_ptr<Square> start,
                   std::shared_ptr<Square> end) {
    int startCol = start->getCol();
    int startRow = start->getRow();
    int endCol = end->getCol();
    int endRow = end->getRow();

    // Make sure pawn is moving in right direction
    if (this->getColor() == pieceColor::white &&
        end->getRow() < start->getRow()) {
        return false;
    } else if (this->getColor() == pieceColor::black &&
               end->getRow() > start->getRow()) {
        return false;
    }

    // If move is straight line
    if (startCol == endCol) {
        // Pawn can only move straight on if square is empty
        if (std::abs(endRow - startRow) == 1) {
            if (end->getPiece() != nullptr) {
                return false;
            } else {
                return true;
            }
            // Only move two squares if first move
        } else if (std::abs(endRow - startRow) == 2 && this->getIsFirstMove()) {
            if (end->getPiece() != nullptr ||
                board->getSquare((startRow + endRow) / 2, startCol)
                        ->getPiece() != nullptr) {
                return false;
            } else {
                // Pawn can be taken in en passant immediately after its first
                // move
                return true;
            }
        } else {
            return false;
        }
    } else {
        // Take diagonally
        if (std::abs(endRow - startRow) == 1 &&
            (startCol == endCol - 1 || startCol == endCol + 1) &&
            end->getPiece() != nullptr &&
            end->getPiece()->getColor() != this->getColor()) {
            return true;
        }
        // En passant take
        else if (std::abs(endRow - startRow) == 1 &&
                 (startCol == endCol - 1 || startCol == endCol + 1) &&
                 end->getPiece() == nullptr) {
            auto pieceBehind = board->getSquare(startRow, endCol)->getPiece();
            if (pieceBehind != nullptr &&
                pieceBehind->getColor() != this->getColor() &&
                pieceBehind->getPieceName() == pieceType::Pawn &&
                pieceBehind->isVulnerableToEnPassant()) {;
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}

bool Pawn::isVulnerableToEnPassant() { return vulnerableToEnPassant; }

void Pawn::setVulnerableToEnPassant(bool pVulnerability) {
    vulnerableToEnPassant = pVulnerability;
}

bool Pawn::getCanPromote() { return canPromote; }

void Pawn::setCanPromote(bool newCanPromote) { canPromote = newCanPromote; }

//------------------------------------------------------------------------------------------------------------------------------------

Rook::Rook(pieceColor pColor, bool pCanCastle)
    : Piece(pColor), canCastle(pCanCastle) {
    imageName = pColor == pieceColor::white ? "assets/whtRook.png"
                                            : "assets/blkRook.png";
    pieceName = pieceType::Rook;
}

bool Rook::canMove(std::shared_ptr<Board> board, std::shared_ptr<Square> start,
                   std::shared_ptr<Square> end) {
    int startCol = start->getCol();
    int startRow = start->getRow();
    int endCol = end->getCol();
    int endRow = end->getRow();

    // Only move in straight lines
    if ((startCol != endCol && startRow == endRow) ||
        (startRow != endRow && startCol == endCol)) {
        int step;
        int startVal;
        int endVal;

        // Squares to loop over
        if (startCol != endCol) {
            step = startCol < endCol ? 1 : -1;
            startVal = startCol;
            endVal = endCol;
        } else {
            step = startRow < endRow ? 1 : -1;
            startVal = startRow;
            endVal = endRow;
        }

        // Check that squares between start and end are empty
        for (int i = startVal += step;
             startVal < endVal ? i < endVal : i > endVal; i += step) {
            if (startCol != endCol &&
                board->getSquare(startRow, i)->getPiece() != nullptr) {
                return false;
            } else if (startRow != endRow &&
                       board->getSquare(i, startCol)->getPiece() != nullptr) {
                return false;
            }
        }

        if (board->getSquare(endRow, endCol)->getPiece() != nullptr &&
            board->getSquare(endRow, endCol)->getPiece()->getColor() ==
                this->getColor()) {
            return false;
        } else {
            // Rook can't perform castling if it has already moved
            return true;
        }
    } else {
        return false;
    }
}

bool Rook::getCanCastle() { return canCastle; }

void Rook::setCanCastle(bool flag) { canCastle = flag; }

//------------------------------------------------------------------------------------------------------------------------------------

Knight::Knight(pieceColor pColor) : Piece(pColor) {
    imageName = pColor == pieceColor::white ? "assets/whtKnight.png"
                                            : "assets/blkKnight.png";
    pieceName = pieceType::Knight;
}

bool Knight::canMove(std::shared_ptr<Board> board,
                     std::shared_ptr<Square> start,
                     std::shared_ptr<Square> end) {
    int startCol = start->getCol();
    int startRow = start->getRow();
    int endCol = end->getCol();
    int endRow = end->getRow();

    // List of squares rook can move to relative from start position
    std::array<std::array<int, 2>, 8> validMoves = {{
        {startRow + 2, startCol + 1},
        {startRow + 1, startCol + 2},
        {startRow - 1, startCol + 2},
        {startRow - 2, startCol + 1},
        {startRow - 2, startCol - 1},
        {startRow - 1, startCol - 2},
        {startRow + 1, startCol - 2},
        {startRow + 2, startCol - 1}}};

    if (std::any_of(validMoves.begin(), validMoves.end(),
                    [&](std::array<int, 2> move) {
                        return move[0] == endRow && move[1] == endCol;
                    })) {
        if (board->getSquare(endRow, endCol)->getPiece() != nullptr &&
            board->getSquare(endRow, endCol)->getPiece()->getColor() ==
                this->getColor()) {
            return false;
        } else {
            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------------------------------------------------------------

Bishop::Bishop(pieceColor pColor) : Piece(pColor) {
    imageName = pColor == pieceColor::white ? "assets/whtBishop.png"
                                            : "assets/blkBishop.png";
    pieceName = pieceType::Bishop;
}

bool Bishop::canMove(std::shared_ptr<Board> board,
                     std::shared_ptr<Square> start,
                     std::shared_ptr<Square> end) {
    int startCol = start->getCol();
    int startRow = start->getRow();
    int endCol = end->getCol();
    int endRow = end->getRow();

    // Diagonal moves are only permitted if change in x == change in y
    if (std::abs(endCol - startCol) != std::abs(endRow - startRow)) {
        return false;
    }

    int rowStep;
    int colStep;
    if (startRow < endRow && startCol < endCol) {
        rowStep = 1;
        colStep = 1;
    } else if (startRow > endRow && startCol < endCol) {
        rowStep = -1;
        colStep = 1;
    } else if (startRow > endRow && startCol > endCol) {
        rowStep = -1;
        colStep = -1;
    } else {
        rowStep = 1;
        colStep = -1;
    }

    int j = startCol + colStep;
    for (int i = startRow + rowStep;
         startRow < endRow ? i < endRow : i > endRow; i += rowStep) {
        if (board->getSquare(i, j)->getPiece() != nullptr) {
            return false;
        }
        j += colStep;
    }

    if (board->getSquare(endRow, endCol)->getPiece() != nullptr &&
        board->getSquare(endRow, endCol)->getPiece()->getColor() ==
            this->getColor()) {
        return false;
    } else {
        return true;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------

King::King(pieceColor pColor, bool pCanCastle)
    : Piece(pColor), canCastle(pCanCastle) {
    imageName = pColor == pieceColor::white ? "assets/whtKing.png"
                                            : "assets/blkKing.png";
    pieceName = pieceType::King;
}

bool King::canMove(std::shared_ptr<Board> board, std::shared_ptr<Square> start,
                   std::shared_ptr<Square> end) {
    int startCol = start->getCol();
    int startRow = start->getRow();
    int endCol = end->getCol();
    int endRow = end->getRow();

    int rowDifference = std::abs(endRow - startRow);
    int colDifference = std::abs(endCol - startCol);
    // King can only move one place in any direction
    if ((rowDifference == 1 && colDifference == 0) ||
        (rowDifference == 0 && colDifference == 1) ||
        (rowDifference == 1 && colDifference == 1)) {
        if (board->getSquare(endRow, endCol)->getPiece() == nullptr) {
            return true;
        } else if (board->getSquare(endRow, endCol)->getPiece()->getColor() !=
                   this->getColor()) {
            return true;
        }
    }

    // Check if castling move
    if (rowDifference == 0 && colDifference == 2) {
        auto kingDestination = board->getSquare(startRow, endCol);
        auto rookDestination = board->getSquare(
            startRow, endCol > startCol ? endCol - 1 : endCol + 1);

        bool emptyFlag = true;
        bool rookFlag = false;
        std::pair<int, int> rookPos;
        for (int i = endCol > startCol ? startCol + 1 : startCol - 1;
             endCol > startCol ? i < 8 : i >= 0;
             endCol > startCol ? i++ : i--) {
            auto piece = board->getSquare(startRow, i)->getPiece();
            if (piece != nullptr && piece->getPieceName() != pieceType::Rook) {
                emptyFlag = false;
                break;
            } else if (piece != nullptr &&
                       piece->getPieceName() == pieceType::Rook &&
                       piece->getColor() == this->getColor()) {
                if (piece->getCanCastle()) {
                    rookFlag = true;
                    rookPos = {startRow, i};
                    piece->setCanCastle(false);
                    break;
                } else {
                    break;
                }
            }
        }
        if (emptyFlag && rookFlag && kingDestination->getPiece() == nullptr &&
            rookDestination->getPiece() == nullptr && canCastle) {
            rookDestination->setPiece(
                board->getSquare(rookPos.first, rookPos.second)->getPiece());
            board->getSquare(rookPos.first, rookPos.second)->setPiece(nullptr);
            return true;
        }

        return false;
    }

    return false;
}

void King::setCanCastle(bool flag) { canCastle = flag; }

//------------------------------------------------------------------------------------------------------------------------------------

Queen::Queen(pieceColor pColor) : Piece(pColor) {
    imageName = pColor == pieceColor::white ? "assets/whtQueen.png"
                                            : "assets/blkQueen.png";
    pieceName = pieceType::Queen;
    dummyRook = std::make_shared<Rook>(Rook(pColor, false));
    dummyBishop = std::make_shared<Bishop>(Bishop(pColor));
}

bool Queen::canMove(std::shared_ptr<Board> board, std::shared_ptr<Square> start,
                    std::shared_ptr<Square> end) {
    // Queen movement is combination of rook and bishop
    int startCol = start->getCol();
    int startRow = start->getRow();
    int endCol = end->getCol();
    int endRow = end->getRow();

    // dummyRook and dummyBishop are cached as attributes of this, saves expensive creation/deletion each time this method is called
    board->getSquare(startRow, startCol)->setPiece(dummyRook);
    if (dummyRook->canMove(board, board->getSquare(startRow, startCol), board->getSquare(endRow, endCol))) {
        board->getSquare(startRow, startCol)->setPiece(shared_from_this());
        return true;
    } else {
        board->getSquare(startRow, startCol)->setPiece(dummyBishop);
        if (dummyBishop->canMove(board, board->getSquare(startRow, startCol), board->getSquare(endRow, endCol))) {
            board->getSquare(startRow, startCol)->setPiece(shared_from_this());
            return true;
        } else {
            board->getSquare(startRow, startCol)->setPiece(shared_from_this());
            return false;
        }
    }
}
