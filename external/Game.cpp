#include "Game.hpp"

Game::Game() {
    currentTurn = pieceColor::white;
    status = gameStatus::startScreen;
    opponent = opponents::player;
    board = std::make_shared<Board>(Board());
}

Game::Game(pieceColor pCurrentTurn, gameStatus pStatus, opponents pOpponent, std::shared_ptr<Board> pBoard) {
    currentTurn = pCurrentTurn;
    status = pStatus;
    opponent = pOpponent;
    board = pBoard;
}

void Game::resetGame() {
    currentTurn = pieceColor::white;
    status = gameStatus::inProgress;
    board->resetBoard();
}

pieceColor Game::getCurrentTurn() { return currentTurn; }

void Game::setCurrentTurn(pieceColor newCurrentTurn) {
    currentTurn = newCurrentTurn;
}

gameStatus Game::getStatus() { return status; }

void Game::setStatus(gameStatus newStatus) { status = newStatus; }

opponents Game::getOpponent() { return opponent; }

void Game::setOpponent(opponents newOpponent) { opponent = newOpponent; }

// Called when user selects promotion choice from menu in main
void Game::promote(pieceType chosenType) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto square = board->getSquare(i, j);
            auto piece = square->getPiece();
            // Convert pawn into chosen piece
            if (piece != nullptr && piece->getPieceName() == pieceType::Pawn &&
                piece->getCanPromote()) {
                piece->setCanPromote(false);
                square->setPiece(nullptr);
                if (chosenType == pieceType::Queen) {
                    square->setPiece(
                        std::make_shared<Queen>(Queen(piece->getColor())));
                } else if (chosenType == pieceType::Rook) {
                    square->setPiece(
                        std::make_shared<Rook>(Rook(piece->getColor(), false)));
                } else if (chosenType == pieceType::Bishop) {
                    square->setPiece(
                        std::make_shared<Bishop>(Bishop(piece->getColor())));
                } else if (chosenType == pieceType::Knight) {
                    square->setPiece(
                        std::make_shared<Knight>(Knight(piece->getColor())));
                }
            }
        }
    }
    // Perform same check and mate reviews as in a normal turn
    status =
        board->isCheck(currentTurn == pieceColor::black ? pieceColor::white
                                                        : pieceColor::black);
    if (status == gameStatus::blackCheck) {
        status = board->isCheckMate(pieceColor::black, status, shared_from_this());
    } else if (status == gameStatus::whiteCheck) {
        status = board->isCheckMate(pieceColor::white, status, shared_from_this());
    }
    currentTurn = (currentTurn == pieceColor::white) ? pieceColor::black
                                                     : pieceColor::white;
}

// Pop latest move from stack and reverse that move
void Game::undoMove() {
    if (moves.size() > 0) {
        // Moves are stored as tuples of {start, end, pieceMoved, pieceTaken,
        // castlingMove, enPassantMove}
        auto latestTurn = moves.back();
        auto start = std::get<0>(latestTurn);
        auto end = std::get<1>(latestTurn);
        auto pieceMoved = std::get<2>(latestTurn);
        auto pieceTaken = std::get<3>(latestTurn);
        auto castlingMove = std::get<4>(latestTurn);
        auto enPassantMove = std::get<5>(latestTurn);
        auto moveStatus = std::get<6>(latestTurn);
        moves.pop_back();

        // Return moved piece to previous square
        board->movePiece(end, start, pieceMoved);
        // If a piece was taken, bring it back into play
        board->getSquare(end->getRow(), end->getCol())->setPiece(pieceTaken);

        // Rook must also be moved if previous move was a castling move
        if (castlingMove) {
            auto leftNeighbour =
                board->getSquare(end->getRow(), end->getCol() - 1)->getPiece();
            if (leftNeighbour != nullptr &&
                leftNeighbour->getPieceName() == pieceType::Rook) {
                    board->getSquare(start->getRow(), 7)->setPiece(leftNeighbour);
                    board->getSquare(end->getRow(), end->getCol() - 1)
                        ->setPiece(nullptr);
                    leftNeighbour->setCanCastle(true);
                    pieceMoved->setCanCastle(true);
            } else {
                auto rightNeighbour =
                    board->getSquare(end->getRow(), end->getCol() + 1)
                        ->getPiece();
                if (rightNeighbour != nullptr &&
                    rightNeighbour->getPieceName() == pieceType::Rook) {
                    board->getSquare(start->getRow(), 0)
                        ->setPiece(rightNeighbour);
                    board->getSquare(end->getRow(), end->getCol() + 1)
                        ->setPiece(nullptr);
                    rightNeighbour->setCanCastle(true);
                    pieceMoved->setCanCastle(true);
                }
            }
        }

        // Reset flag canCastle for king and rook
        // Also reset flag isFirstMove for pawn
        if (pieceMoved->getPieceName() == pieceType::King ||
            pieceMoved->getPieceName() == pieceType::Rook ||
            pieceMoved->getPieceName() == pieceType::Pawn) {
            bool isFirstMove = true;
            for (auto move : moves) {
                if (std::get<2>(move) == pieceMoved) {
                    isFirstMove = false;
                }
            }
            pieceMoved->setCanCastle(isFirstMove);
            pieceMoved->setIsFirstMove(isFirstMove);
        }

        // If move was en passant, taken pawn isn't in end square, so needs to
        // be brought back into play
        if (enPassantMove) {
            board->getSquare(start->getRow(), end->getCol())
                ->setPiece(std::make_shared<Pawn>(
                    Pawn(pieceMoved->getColor() == pieceColor::black
                             ? pieceColor::white
                             : pieceColor::black,
                         true, true)));
        }

        // Perform same check and mate reviews for reverted board
        currentTurn = std::get<2>(latestTurn)->getColor();
        status = moveStatus;
    }
}

bool Game::turn(std::shared_ptr<Square> start, std::shared_ptr<Square> end, bool dummyFlag) {
    auto piece = start->getPiece();

    int startRow = start->getRow();
    int startCol = start->getCol();
    int endRow = end->getRow();
    int endCol = end->getCol();

    if (piece != nullptr && piece->getColor() == currentTurn) {
        auto evaluateMove = piece->canMove(board, start, end);

        if (evaluateMove) {
            // A piece can only move if it doesn't put its own side into check
            auto cachePiece = board->getSquare(endRow, endCol)->getPiece();
            board->movePiece(board->getSquare(startRow, startCol), board->getSquare(endRow, endCol), board->getSquare(startRow, startCol)->getPiece());
            auto testCheck = board->isCheck(piece->getColor());
            board->movePiece(board->getSquare(endRow, endCol), board->getSquare(startRow, startCol), board->getSquare(endRow, endCol)->getPiece());
            board->getSquare(endRow, endCol)->setPiece(cachePiece);

            if ((currentTurn == pieceColor::black &&
                 testCheck != gameStatus::blackCheck) ||
                (currentTurn == pieceColor::white &&
                 testCheck != gameStatus::whiteCheck)) {
                
                // En passant take
                if (start->getPiece() != nullptr && start->getPiece()->getPieceName() == pieceType::Pawn) {
                    if (std::abs(endRow - startRow) == 1 &&
                        (startCol == endCol - 1 || startCol == endCol + 1) &&
                        end->getPiece() == nullptr) {
                            auto pieceBehind = board->getSquare(startRow, endCol)->getPiece();
                            if (pieceBehind != nullptr &&
                                pieceBehind->getColor() != piece->getColor() &&
                                pieceBehind->getPieceName() == pieceType::Pawn &&
                                pieceBehind->isVulnerableToEnPassant()) {
                                    board->getSquare(startRow, endCol)->setPiece(nullptr);
                                }
                        }
                }
                    
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        auto piece = board->getSquare(i, j)->getPiece();
                        // Any pawn whose first move was previous turn are no
                        // longer vulnerable to en passant
                        if (piece != nullptr &&
                            piece->getColor() !=
                                start->getPiece()->getColor() &&
                            piece->getPieceName() == pieceType::Pawn) {
                            piece->setVulnerableToEnPassant(false);
                        }
                    }
                }

                if (piece->getPieceName() == pieceType::Pawn) {
                    // Open pawn up to en passant
                    if (piece->getIsFirstMove() && std::abs(endRow - startRow) == 2) {
                        piece->setVulnerableToEnPassant(true);
                    }

                    piece->setIsFirstMove(false);

                    // Check for promotion
                    if ((piece->getColor() == pieceColor::white && endRow == 7) || 
                        (piece->getColor() == pieceColor::black && endRow == 0)) {
                            piece->setCanPromote(true);
                        }
                }

                // Once any king/rook has moved, it can no longer castle
                if (piece->getPieceName() == pieceType::Rook || piece->getPieceName() == pieceType::King) {
                    piece->setCanCastle(false);
                }

                // Record move
                // If move was castling
                if (start->getPiece()->getPieceName() == pieceType::King &&
                    std::abs(startCol - endCol) == 2) {
                    moves.push_back({start, end, start->getPiece(),
                                     end->getPiece(), true, false, gameStatus::inProgress});
                }
                // If move was en passant
                else if (start->getPiece() != nullptr &&
                         start->getPiece()->getPieceName() == pieceType::Pawn &&
                         (endCol == startCol - 1 ||
                          endCol == startCol + 1) &&
                         end->getPiece() == nullptr) {
                    moves.push_back({start, end, start->getPiece(),
                                     end->getPiece(), false, true, gameStatus::inProgress});
                }
                // All other moves
                else {
                    moves.push_back({start, end, start->getPiece(),
                                     end->getPiece(), false, false, gameStatus::inProgress});
                }

                board->movePiece(start, end, piece);

                // If move results in promotion, pause game whilst user chooses
                // piece to promote to Gameplay will resume when promote is
                // called from main
                if (piece->getPieceName() == pieceType::Pawn &&
                    piece->getCanPromote()) {
                        if (opponent == opponents::player) {
                            status = gameStatus::choosingPromotion;
                        } else {
                            promote(pieceType::Queen);
                        }
                } else {
                    // Check if in check
                    // If so, then check for mate
                    status = board->isCheck(currentTurn == pieceColor::black
                                                ? pieceColor::white
                                                : pieceColor::black);
                    
                    // Only need to check for mate if side is already in check
                    if (!dummyFlag && 
                        (status == gameStatus::blackCheck ||
                         status == gameStatus::whiteCheck)) {
                        if (currentTurn == pieceColor::white) {
                            status = board->isCheckMate(pieceColor::black, status, shared_from_this());
                        } else {
                            status = board->isCheckMate(pieceColor::white, status, shared_from_this());
                        }
                    }
                    
                    // Record status for last entry in move stack
                    auto latestTurn = moves.back();
                    std::get<6>(latestTurn) = status;

                    currentTurn = (currentTurn == pieceColor::white)
                                      ? pieceColor::black
                                      : pieceColor::white;
                }
                return true;
            } else return false;
        } else return false;
    } else return false;
}

std::shared_ptr<Board> Game::getBoard() { return board; }
