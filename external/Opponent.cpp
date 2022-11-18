#include "Opponent.hpp"

void opponentTurn(std::shared_ptr<Game> game) {
    std::shared_ptr<Board> board = game->getBoard();

    std::vector<std::shared_ptr<Square>> aiSquares;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto square = board->getSquare(i, j);
            if (square->getPiece() != nullptr && square->getPiece()->getColor() == pieceColor::black) {
                aiSquares.push_back(square);
            }
        }
    }

    // startRow, startCol, endRow, endCol
    std::vector<std::array<int, 4>> aiMoves;
    for (auto startSquare : aiSquares) {
        for (auto endSquare : startSquare->getPiece()->legalMoves(board, startSquare)) {
            aiMoves.push_back({startSquare->getRow(), startSquare->getCol(), endSquare.first, endSquare.second});
        }
    }

    std::array<int, 4> bestMove;
    int bestScore = -99999999;
    for (auto move : aiMoves) {
        int moveScore = -evaluateMove(game, board, move);
        if (moveScore > bestScore) {
            bestScore = moveScore;
            bestMove = move;
        }
    }

    auto startSquare = board->getSquare(bestMove[0], bestMove[1]);
    auto endSquare = board->getSquare(bestMove[2], bestMove[3]);
    game->turn(startSquare, endSquare);
}

int evaluateMove(std::shared_ptr<Game> game, std::shared_ptr<Board> board, std::array<int, 4> move) {
    auto dummyBoard = std::make_shared<Board>(Board(board->getSquares()));
    auto dummyGame = std::make_shared<Game>(Game(game->getCurrentTurn(), game->getStatus(), game->getOpponent(), dummyBoard));

    if (dummyGame->turn(dummyBoard->getSquare(move[0], move[1]), dummyBoard->getSquare(move[2], move[3]))) {
        int score = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                auto piece = dummyBoard->getSquare(i, j)->getPiece();
                if (piece != nullptr) {
                    score += ratePiece(piece->getColor(), piece->getPieceName());
                }
            }
        }
        return score;
    } 
    return 999999999;
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
        default:
            score = 900;
    }

    if (color == pieceColor::black) {
        return -score;
    } else {
        return score;
    }
}
