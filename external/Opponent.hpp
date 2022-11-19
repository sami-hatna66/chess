#ifndef Opponent_hpp
#define Opponent_hpp

#include "Piece.hpp"
#include "Square.hpp"
#include "Board.hpp"
#include "Game.hpp"

#include <random>
#include <cstdlib>
#include <iostream>

std::vector<std::array<int, 4>> getAiMoves(std::shared_ptr<Game> game, pieceColor color);

void opponentTurn(std::shared_ptr<Game> game);

int miniMax(int depth, std::shared_ptr<Game> game, bool isMaximising, int alpha, int beta);

int evaluateBoard(std::shared_ptr<Game> game, std::shared_ptr<Board> board);

int ratePiece(pieceColor color, pieceType type);

#endif
