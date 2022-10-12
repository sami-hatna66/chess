#include "Board.hpp"

Board::Board() { this->resetBoard(); }

// Create new object from array of squares
// Used for deep copy of board in isCheck and isCheckmate
Board::Board(std::array<std::array<std::shared_ptr<Square>, 8>, 8> pSquares) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto piece = pSquares[i][j]->getPiece();

            if (piece == nullptr) {
                squares[i][j] = std::make_shared<Square>(Square(i, j, nullptr));
            } else if (piece->getPieceName() == pieceType::Pawn) {
                squares[i][j] = std::make_shared<Square>(
                    Square(i, j,
                           std::make_shared<Pawn>(
                               Pawn(piece->getColor(), piece->getIsFirstMove(),
                                    piece->isVulnerableToEnPassant()))));
            } else if (piece->getPieceName() == pieceType::Rook) {
                squares[i][j] = std::make_shared<Square>(
                    Square(i, j,
                           std::make_shared<Rook>(Rook(
                               piece->getColor(), piece->getCanCastle()))));
            } else if (piece->getPieceName() == pieceType::Knight) {
                squares[i][j] = std::make_shared<Square>(Square(
                    i, j, std::make_shared<Knight>(Knight(piece->getColor()))));
            } else if (piece->getPieceName() == pieceType::Bishop) {
                squares[i][j] = std::make_shared<Square>(Square(
                    i, j, std::make_shared<Bishop>(Bishop(piece->getColor()))));
            } else if (piece->getPieceName() == pieceType::Queen) {
                squares[i][j] = std::make_shared<Square>(Square(
                    i, j, std::make_shared<Queen>(Queen(piece->getColor()))));
            } else if (piece->getPieceName() == pieceType::King) {
                squares[i][j] = std::make_shared<Square>(
                    Square(i, j,
                           std::make_shared<King>(King(
                               piece->getColor(), piece->getCanCastle()))));
            }
        }
    }
}

void Board::resetBoard() {
    // Clear old board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            squares[i][j] = nullptr;
        }
    }

    // Standard starting configuration
    squares[0][0] = std::make_shared<Square>(
        Square(0, 0, std::make_shared<Rook>(Rook(pieceColor::white))));
    squares[0][1] = std::make_shared<Square>(
        Square(0, 1, std::make_shared<Knight>(Knight(pieceColor::white))));
    squares[0][2] = std::make_shared<Square>(
        Square(0, 2, std::make_shared<Bishop>(Bishop(pieceColor::white))));
    squares[0][3] = std::make_shared<Square>(
        Square(0, 3, std::make_shared<Queen>(Queen(pieceColor::white))));
    squares[0][4] = std::make_shared<Square>(
        Square(0, 4, std::make_shared<King>(King(pieceColor::white))));
    squares[0][5] = std::make_shared<Square>(
        Square(0, 5, std::make_shared<Bishop>(Bishop(pieceColor::white))));
    squares[0][6] = std::make_shared<Square>(
        Square(0, 6, std::make_shared<Knight>(Knight(pieceColor::white))));
    squares[0][7] = std::make_shared<Square>(
        Square(0, 7, std::make_shared<Rook>(Rook(pieceColor::white))));

    for (int i = 0; i < 8; i++) {
        squares[1][i] = std::make_shared<Square>(
            Square(1, i, std::make_shared<Pawn>(Pawn(pieceColor::white))));
    }

    squares[7][0] = std::make_shared<Square>(
        Square(7, 0, std::make_shared<Rook>(Rook(pieceColor::black))));
    squares[7][1] = std::make_shared<Square>(
        Square(7, 1, std::make_shared<Knight>(Knight(pieceColor::black))));
    squares[7][2] = std::make_shared<Square>(
        Square(7, 2, std::make_shared<Bishop>(Bishop(pieceColor::black))));
    squares[7][3] = std::make_shared<Square>(
        Square(7, 3, std::make_shared<Queen>(Queen(pieceColor::black))));
    squares[7][4] = std::make_shared<Square>(
        Square(7, 4, std::make_shared<King>(King(pieceColor::black))));
    squares[7][5] = std::make_shared<Square>(
        Square(7, 5, std::make_shared<Bishop>(Bishop(pieceColor::black))));
    squares[7][6] = std::make_shared<Square>(
        Square(7, 6, std::make_shared<Knight>(Knight(pieceColor::black))));
    squares[7][7] = std::make_shared<Square>(
        Square(7, 7, std::make_shared<Rook>(Rook(pieceColor::black))));

    for (int i = 0; i < 8; i++) {
        squares[6][i] = std::make_shared<Square>(
            Square(6, i, std::make_shared<Pawn>(Pawn(pieceColor::black))));
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (squares[i][j] == nullptr) {
                squares[i][j] = std::make_shared<Square>(Square(i, j, nullptr));
            }
        }
    }
}

std::shared_ptr<Square> Board::getSquare(int row, int col) {
    return squares[row][col];
}

std::array<std::array<std::shared_ptr<Square>, 8>, 8> Board::getSquares() {
    return squares;
}

std::array<std::array<std::shared_ptr<Square>, 8>, 8> *Board::getBoard() {
    return &squares;
}

void Board::movePiece(std::shared_ptr<Square> start,
                      std::shared_ptr<Square> end,
                      std::shared_ptr<Piece> piece) {
    squares[end->getRow()][end->getCol()]->setPiece(
        std::move(squares[start->getRow()][start->getCol()]->getPiece()));
    squares[start->getRow()][start->getCol()]->setPiece(nullptr);
}

// Check if side is in check
gameStatus Board::isCheck(pieceColor side) {
    // Check if any piece from opposing side threatens king
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto piece = squares[i][j]->getPiece();
            if (piece != nullptr && piece->getColor() != side) {
                auto possibleMoves =
                    piece->legalMoves(shared_from_this(), squares[i][j]);
                for (auto move : possibleMoves) {
                    auto endPiece =
                        squares[move.first][move.second]->getPiece();
                    if (endPiece != nullptr &&
                        endPiece->getPieceName() == pieceType::King &&
                        endPiece->getColor() == side) {
                        return side == pieceColor::black
                                   ? gameStatus::blackCheck
                                   : gameStatus::whiteCheck;
                    }
                }
            }
        }
    }
    return gameStatus::inProgress;
}
// Check if colorInCheck is in checkmate
gameStatus Board::isCheckMate(pieceColor colorInCheck) {
    std::shared_ptr<Board> dummyBoard;

    // Check if any piece from colorInCheck can make a move which results in
    // colorInCheck no longer being in check
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto piece = squares[i][j]->getPiece();
            if (piece != nullptr && piece->getColor() == colorInCheck) {
                std::vector<std::pair<int, int>> possibleMoves =
                    piece->legalMoves(shared_from_this(), squares[i][j]);

                for (auto move : possibleMoves) {
                    dummyBoard = std::make_shared<Board>(Board(squares));
                    dummyBoard->movePiece(
                        dummyBoard->getSquare(i, j),
                        dummyBoard->getSquare(move.first, move.second),
                        dummyBoard->getSquare(i, j)->getPiece());
                    gameStatus testCheck = dummyBoard->isCheck(colorInCheck);
                    if ((colorInCheck == pieceColor::black &&
                         testCheck != gameStatus::blackCheck) ||
                        (colorInCheck == pieceColor::white &&
                         testCheck != gameStatus::whiteCheck)) {

                        return colorInCheck == pieceColor::black
                                   ? gameStatus::blackCheck
                                   : gameStatus::whiteCheck;
                    }
                }
            }
        }
    }

    return colorInCheck == pieceColor::black ? gameStatus::blackCheckmate
                                             : gameStatus::whiteCheckmate;
}
