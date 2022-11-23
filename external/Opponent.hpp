#ifndef Opponent_hpp
#define Opponent_hpp

#include "Piece.hpp"
#include "Square.hpp"
#include "Board.hpp"
#include "Game.hpp"

std::vector<std::array<int, 4>> getAiMoves(std::shared_ptr<Game> game, pieceColor color);

void opponentTurn(std::shared_ptr<Game> game);

float miniMax(int depth, std::shared_ptr<Game> game, bool isMaximising, float alpha, float beta);

float evaluateBoard(std::shared_ptr<Game> game, std::shared_ptr<Board> board);

float ratePiece(pieceColor color, pieceType type, int row, int col);

#endif
