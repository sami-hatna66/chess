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

    // row, col
    blackKingPos = std::make_pair<int, int>(7, 4);
    whiteKingPos = std::make_pair<int, int>(0, 4);
}

std::shared_ptr<Square> Board::getSquare(int row, int col) {
    return squares[row][col];
}

std::array<std::array<std::shared_ptr<Square>, 8>, 8> Board::getSquares() {
    return squares;
}

void Board::setSquares(std::array<std::array<std::shared_ptr<Square>, 8>, 8> newSquares) {
    squares = newSquares;
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

    if (piece->getPieceName() == pieceType::King) {
        if (piece->getColor() == pieceColor::black) {
            blackKingPos = std::make_pair<int, int>(end->getRow(), end->getCol());
        } else {
            whiteKingPos = std::make_pair<int, int>(end->getRow(), end->getCol());
        }
    }
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
gameStatus Board::isCheckMate(pieceColor colorInCheck, gameStatus prevStatus) {
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

                        return prevStatus;
                    }
                }
            }
        }
    }

    return colorInCheck == pieceColor::black ? gameStatus::blackCheckmate
                                             : gameStatus::whiteCheckmate;
}

std::pair<int, int> Board::getBlackKingPos() {
    return blackKingPos;
}

void Board::setBlackKingPos(std::pair<int, int> newPos) {
    blackKingPos = newPos;
}

std::pair<int, int> Board::getWhiteKingPos() {
    return whiteKingPos;
}

void Board::setWhiteKingPos(std::pair<int, int> newPos) {
    whiteKingPos = newPos;
}

void Board::printBoard() {
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            auto square = squares[i][j];
            if (square->getPiece() == nullptr) {
                std::cout << "o ";
            }
            else {
                auto piece = square->getPiece();
                if (piece->getColor() == pieceColor::black) {
                    switch(piece->getPieceName()) {
                        case pieceType::Pawn: 
                            std::cout << "P ";
                            break;
                        case pieceType::Rook: 
                            std::cout << "R ";
                            break;
                        case pieceType::Knight: 
                            std::cout << "K ";
                            break;
                        case pieceType::Bishop: 
                            std::cout << "B ";
                            break;
                        case pieceType::King: 
                            std::cout << "K ";
                            break;
                        case pieceType::Queen: 
                            std::cout << "Q ";
                            break;
                        default:
                            std::cout << "err";
                    }
                } else {
                    switch(piece->getPieceName()) {
                        case pieceType::Pawn: 
                            std::cout << "p ";
                            break;
                        case pieceType::Rook: 
                            std::cout << "r ";
                            break;
                        case pieceType::Knight: 
                            std::cout << "k ";
                            break;
                        case pieceType::Bishop: 
                            std::cout << "b ";
                            break;
                        case pieceType::King: 
                            std::cout << "k ";
                            break;
                        case pieceType::Queen: 
                            std::cout << "q ";
                            break;
                        default:
                            std::cout << "err";
                    }
                }
            }
        }
        std::cout << std::endl;
    }
}
