#ifndef Game_hpp
#define Game_hpp

#include "Board.hpp"
#include "Piece.hpp"
#include "Square.hpp"

enum class opponents {computer, player};

class Game : public std::enable_shared_from_this<Game> {
  private:
    pieceColor currentTurn;
    gameStatus status;
    opponents opponent;
    std::shared_ptr<Board> board;
    // Previous moves stack
    // start, end, pieceMoved, pieceTaken, castlingMove, enPassantMove, status
    std::vector<
        std::tuple<std::shared_ptr<Square>, std::shared_ptr<Square>,
                   std::shared_ptr<Piece>, std::shared_ptr<Piece>, bool, bool, gameStatus>>
        moves;

  public:
    Game();
    Game(pieceColor pCurrentTurn, gameStatus pStatus, opponents pOpponent, std::shared_ptr<Board> pBoard);
    void resetGame();
    pieceColor getCurrentTurn();
    void setCurrentTurn(pieceColor newCurrentTurn);
    gameStatus getStatus();
    void setStatus(gameStatus newStatus);
    opponents getOpponent();
    void setOpponent(opponents newOpponent);
    bool turn(std::shared_ptr<Square> start, std::shared_ptr<Square> end, bool dummyFlag = false);
    std::shared_ptr<Board> getBoard();
    void promote(pieceType chosenType);
    void undoMove();
};

#endif
