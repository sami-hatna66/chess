#include "Opponent.hpp"

std::vector<std::array<int, 4>> getAiMoves(std::shared_ptr<Game> game, pieceColor color) {
    std::shared_ptr<Board> board = game->getBoard();

    std::vector<std::shared_ptr<Square>> aiSquares;
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            auto square = board->getSquare(i, j);
            if (square->getPiece() != nullptr && square->getPiece()->getColor() == color) {
                aiSquares.push_back(square);
            }
        }
    }

    // startRow, startCol, endRow, endCol
    std::vector<std::array<int, 4>> aiMoves;
    for (auto startSquare : aiSquares) {
        if (startSquare->getPiece() != nullptr) {
            for (auto endSquare : startSquare->getPiece()->legalMoves(board, startSquare)) {
                aiMoves.push_back({startSquare->getRow(), startSquare->getCol(), endSquare.first, endSquare.second});
            }
        }
    }

    return aiMoves;
}

void opponentTurn(std::shared_ptr<Game> game) {
    std::shared_ptr<Board> board = game->getBoard();

    auto aiMoves = getAiMoves(game, pieceColor::black);

    std::array<int, 4> bestMove;
    int bestScore = -99999999;
    int depth = 3;
    bool isMaximisingPlayer = true;
    for (auto move : aiMoves) {
        auto dummyBoard = std::make_shared<Board>(Board(board->getSquares()));
        auto dummyGame = std::make_shared<Game>(Game(game->getCurrentTurn(), game->getStatus(), game->getOpponent(), dummyBoard));
        if (dummyGame->turn(dummyBoard->getSquare(move[0], move[1]), dummyBoard->getSquare(move[2], move[3]))) {
            int moveScore = miniMax(depth - 1, dummyGame, !isMaximisingPlayer, -10000, 10000);
            std::cout << moveScore << std::endl;
            if (moveScore >= bestScore) {
                bestMove = move;
                bestScore = moveScore;
            }
        }
    }

    auto startSquare = board->getSquare(bestMove[0], bestMove[1]);
    auto endSquare = board->getSquare(bestMove[2], bestMove[3]);
    game->turn(startSquare, endSquare);
}

int miniMax(int depth, std::shared_ptr<Game> game, bool isMaximising, int alpha, int beta) {
    if (depth == 0) {
        return -evaluateBoard(game, game->getBoard());
    }
    
    auto board = game->getBoard();

    auto aiMoves = getAiMoves(game, (isMaximising ? pieceColor::black : pieceColor::white));

    if (isMaximising) {
        int bestScore = -99999999;
        for (auto move : aiMoves) {
            auto turn = game->turn(board->getSquare(move[0], move[1]), board->getSquare(move[2], move[3]));
            if (turn) {
                bestScore = std::max(bestScore, miniMax(depth - 1, game, !isMaximising, alpha, beta));
                alpha = std::max(alpha, bestScore);
                game->undoMove();
                if (beta <= alpha) {
                    return bestScore;
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 99999999;
        for (auto move : aiMoves) {
            auto turn = game->turn(board->getSquare(move[0], move[1]), board->getSquare(move[2], move[3]));
            if (turn) {
                bestScore = std::min(bestScore, miniMax(depth - 1, game, !isMaximising, alpha, beta));
                beta = std::min(beta, bestScore);
                game->undoMove();
                if (beta <= alpha) {
                    return bestScore;
                }
            }
        }
        return bestScore;
    }
}

int evaluateBoard(std::shared_ptr<Game> game, std::shared_ptr<Board> board) {
    int score = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto piece = board->getSquare(i, j)->getPiece();
            if (piece != nullptr) {
                score += ratePiece(piece->getColor(), piece->getPieceName());
            }
        }
    }

    return score;
}

int ratePiece(pieceColor color, pieceType type) {
    int score = 0;

    switch (type) {
        case pieceType::Pawn:
            score = 10;
            break;
        case pieceType::Knight:
            score = 30;
            break;
        case pieceType::Bishop:
            score = 30;
            break;
        case pieceType::Rook:
            score = 50;
            break;
        case pieceType::Queen:
            score = 90;
            break;
        case pieceType::King:
            score = 900;
            break;
        default:
            score = 0;
    }

    if (color == pieceColor::black) {
        return -score;
    } else {
        return score;
    }
}
