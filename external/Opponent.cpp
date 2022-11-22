#include "Opponent.hpp"

std::vector<std::array<int, 4>> getAiMoves(std::shared_ptr<Game> game, pieceColor color) {
    std::shared_ptr<Board> board = game->getBoard();

    // Get all squares with pieces belonging to the computer
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
    // Get all possible moves the computer can make
    for (auto startSquare : aiSquares) {
        if (startSquare->getPiece() != nullptr) {
            for (auto endSquare : startSquare->getPiece()->legalMoves(board, startSquare)) {
                aiMoves.push_back({startSquare->getRow(), startSquare->getCol(), endSquare.first, endSquare.second});
            }
        }
    }

    return aiMoves;
}

// Root of minimax process
void opponentTurn(std::shared_ptr<Game> game) {
    std::shared_ptr<Board> board = game->getBoard();

    auto aiMoves = getAiMoves(game, pieceColor::black);

    std::array<int, 4> bestMove;
    float bestScore = -99999999;
    int depth = 3;
    bool isMaximisingPlayer = true;
    // Iterate through each possible move and start a recursive minimax
    for (auto move : aiMoves) {
        // Clone board and game to prevent any side effects bleeding over into actual game/board
        auto dummyBoard = std::make_shared<Board>(Board(board->getSquares()));
        auto dummyGame = std::make_shared<Game>(Game(game->getCurrentTurn(), game->getStatus(), game->getOpponent(), dummyBoard));
        if (dummyGame->turn(dummyBoard->getSquare(move[0], move[1]), dummyBoard->getSquare(move[2], move[3]))) {
            float moveScore = miniMax(depth - 1, dummyGame, !isMaximisingPlayer, -10000, 10000);
            // Select move with highest evaluation at end of minimax
            if (moveScore >= bestScore) {
                bestMove = move;
                bestScore = moveScore;
            }
        }
    }

    // Make chosen move
    auto startSquare = board->getSquare(bestMove[0], bestMove[1]);
    auto endSquare = board->getSquare(bestMove[2], bestMove[3]);
    game->turn(startSquare, endSquare);
}

// Recursive minimax
float miniMax(int depth, std::shared_ptr<Game> game, bool isMaximising, float alpha, float beta) {
    // Base case, return final board evaluation
    if (depth == 0) {
        return -evaluateBoard(game, game->getBoard());
    }
    
    auto board = game->getBoard();

    auto aiMoves = getAiMoves(game, (isMaximising ? pieceColor::black : pieceColor::white));

    // Black is the maximising player
    if (isMaximising) {
        float bestScore = -99999999;
        for (auto move : aiMoves) {
            if (game->turn(board->getSquare(move[0], move[1]), board->getSquare(move[2], move[3]))) {
                bestScore = std::max(bestScore, miniMax(depth - 1, game, !isMaximising, alpha, beta));
                alpha = std::max(alpha, bestScore);
                game->undoMove();
                // Alpha-beta pruning
                if (beta <= alpha) {
                    return bestScore;
                }
            }
        }
        return bestScore;
    } else {
        float bestScore = 99999999;
        for (auto move : aiMoves) {
            if (game->turn(board->getSquare(move[0], move[1]), board->getSquare(move[2], move[3]))) {
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

// Each outcome is rated according to the value of the pieces left on the board
float evaluateBoard(std::shared_ptr<Game> game, std::shared_ptr<Board> board) {
    float score = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto piece = board->getSquare(i, j)->getPiece();
            if (piece != nullptr) {
                score += ratePiece(piece->getColor(), piece->getPieceName(), i, j);
            }
        }
    }

    return score;
}

// Define piece square tables used in piece evaluation
std::array<std::array<float, 8>, 8> blackPawnTable = {{
    {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
    {5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0},
    {1.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  1.0},
    {0.5,  0.5,  1.0,  2.5,  2.5,  1.0,  0.5,  0.5},
    {0.0,  0.0,  0.0,  2.0,  2.0,  0.0,  0.0,  0.0},
    {0.5, -0.5, -1.0,  0.0,  0.0, -1.0, -0.5,  0.5},
    {0.5,  1.0,  1.0, -2.0, -2.0,  1.0,  1.0,  0.5},
    {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
}};

std::array<std::array<float, 8>, 8> whitePawnTable = {{
    {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
    {0.5,  1.0,  1.0, -2.0, -2.0,  1.0,  1.0,  0.5},
    {0.5, -0.5, -1.0,  0.0,  0.0, -1.0, -0.5,  0.5},
    {0.0,  0.0,  0.0,  2.0,  2.0,  0.0,  0.0,  0.0},
    {0.5,  0.5,  1.0,  2.5,  2.5,  1.0,  0.5,  0.5},
    {1.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  1.0},
    {5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0},
    {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
}};

std::array<std::array<float, 8>, 8> blackKnightTable = {{
    {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
    {-4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0},
    {-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
    {-3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
    {-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.0, -3.0},
    {-3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0},
    {-4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0},
    {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
}};

std::array<std::array<float, 8>, 8> whiteKnightTable = {{
    {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
    {-4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0},
    {-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
    {-3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
    {-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.0, -3.0},
    {-3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0},
    {-4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0},
    {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
}};

std::array<std::array<float, 8>, 8> blackBishopTable = {{
    {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
    {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
    {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},
    {-1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0},
    {-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0},
    {-1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0},
    {-1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0},
    {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
}};

std::array<std::array<float, 8>, 8> whiteBishopTable = {{
    {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
    {-1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0},
    {-1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0},
    {-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0},
    {-1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0},
    {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},
    {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
    {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
}};

std::array<std::array<float, 8>, 8> blackRookTable = {{
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  0.0},
    { 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,  0.5},
    {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
    {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
    {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
    {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
    {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
    { 0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0,  0.0},
}};

std::array<std::array<float, 8>, 8> whiteRookTable = {{
    { 0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0,  0.0},
    {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
    {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
    {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
    {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
    {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
    { 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,  0.5},
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  0.0},
}};

std::array<std::array<float, 8>, 8> blackQueenTable = {{
    {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
    {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
    {-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
    {-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
    { 0.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
    {-1.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
    {-1.0,  0.0,  0.5,  0.0,  0.0,  0.0,  0.0, -1.0},
    {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
}};

std::array<std::array<float, 8>, 8> whiteQueenTable = {{
    {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
    {-1.0,  0.0,  0.5,  0.0,  0.0,  0.0,  0.0, -1.0},
    {-1.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
    { 0.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
    {-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
    {-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
    {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
    {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
}};

std::array<std::array<float, 8>, 8> blackKingTable = {{
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
    {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
    { 2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0},
    { 2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0},
}};

std::array<std::array<float, 8>, 8> whiteKingTable = {{
    { 2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0},
    { 2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0},
    {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
    {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
}};

float ratePiece(pieceColor color, pieceType type, int row, int col) {
    float score = 0;

    switch (type) {
        case pieceType::Pawn:
            score = 10;
            if (color == pieceColor::black) score += blackPawnTable[row][col];
            else score += whitePawnTable[row][col];
            break;
        case pieceType::Knight:
            score = 30;
            if (color == pieceColor::black) score += blackKnightTable[row][col];
            else score += whiteKnightTable[row][col];
            break;
        case pieceType::Bishop:
            score = 30;
            if (color == pieceColor::black) score += blackBishopTable[row][col];
            else score += whiteBishopTable[row][col];
            break;
        case pieceType::Rook:
            score = 50;
            if (color == pieceColor::black) score += blackRookTable[row][col];
            else score += whiteRookTable[row][col];
            break;
        case pieceType::Queen:
            score = 90;
            if (color == pieceColor::black) score += blackQueenTable[row][col];
            else score += whiteQueenTable[row][col];
            break;
        case pieceType::King:
            if (color == pieceColor::black) score += blackKingTable[row][col];
            else score += whiteKingTable[row][col];
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
