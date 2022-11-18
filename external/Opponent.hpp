#ifndef Opponent_hpp
#define Opponent_hpp

#include "Piece.hpp"
#include "Square.hpp"
#include "Board.hpp"
#include "Game.hpp"

#include <random>
#include <cstdlib>
#include <iostream>

void opponentTurn(std::shared_ptr<Game> game);

int evaluateMove(std::shared_ptr<Game> game, std::shared_ptr<Board> board, std::array<int, 4> move);

int ratePiece(pieceColor color, pieceType type);

#endif
