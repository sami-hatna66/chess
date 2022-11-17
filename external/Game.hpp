#ifndef Game_hpp
#define Game_hpp

#include "Board.hpp"
#include "Piece.hpp"
#include "Square.hpp"

enum class opponents {player, computer};

class Game {
  private:
    pieceColor currentTurn;
    gameStatus status;
    opponents opponent;
    std::shared_ptr<Board> board;
    // Previous moves stack
    // start, end, pieceMoved, pieceTaken, castlingMove, enPassantMove
    std::vector<
        std::tuple<std::shared_ptr<Square>, std::shared_ptr<Square>,
                   std::shared_ptr<Piece>, std::shared_ptr<Piece>, bool, bool>>
        moves;

  public:
    Game();
    void resetGame();
    pieceColor getCurrentTurn();
    void setCurrentTurn(pieceColor newCurrentTurn);
    gameStatus getStatus();
    void setStatus(gameStatus newStatus);
    opponents getOpponent();
    void setOpponent(opponents newOpponent);
    bool turn(std::shared_ptr<Square> start, std::shared_ptr<Square> end);
    std::shared_ptr<Board> getBoard();
    void promote(pieceType chosenType);
    void undoMove();
};

#endif
