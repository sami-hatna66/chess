#ifndef Piece_hpp
#define Piece_hpp

#include "Board.hpp"
#include "Square.hpp"
#include <iostream>
#include <memory>
#include <vector>

enum class pieceColor { white, black };
enum class pieceType { Base, Pawn, Rook, Knight, Bishop, King, Queen };

// Forward declarations
class Board;
class Square;

class Piece : public std::enable_shared_from_this<Piece> {
  private:
    bool alive;
    pieceColor color;

  protected:
    const char *imageName;
    pieceType pieceName;

  public:
    Piece(pieceColor pColor);
    bool isAlive();
    void setAlive(bool pAlive);
    pieceColor getColor();
    const char *getImageName();
    virtual bool canMove(std::shared_ptr<Board> board,
                         std::shared_ptr<Square> start,
                         std::shared_ptr<Square> end);
    virtual std::vector<std::pair<int, int>>
    legalMoves(std::shared_ptr<Board> board,
               std::shared_ptr<Square> currentSquare);
    virtual bool getIsFirstMove();
    virtual void setIsFirstMove(bool pIsFirstMove);
    virtual bool isVulnerableToEnPassant();
    virtual void setVulnerableToEnPassant(bool pVulnerability);
    virtual bool getCanCastle();
    virtual void setCanCastle(bool flag);
    virtual bool getCanPromote();
    virtual void setCanPromote(bool newCanPromote);
    pieceType getPieceName();
};

class Pawn : public Piece {
  private:
    bool isFirstMove;
    bool vulnerableToEnPassant;
    bool canPromote;

  public:
    Pawn(pieceColor pColor, bool pIsFirstMove = true,
         bool pVulnerableToEnPassant = false);
    bool canMove(std::shared_ptr<Board> board, std::shared_ptr<Square> start,
                 std::shared_ptr<Square> end) override;
    bool getIsFirstMove() override;
    void setIsFirstMove(bool pIsFirstMove) override;
    bool isVulnerableToEnPassant() override;
    void setVulnerableToEnPassant(bool pVulnerability) override;
    bool getCanPromote() override;
    void setCanPromote(bool newCanPromote) override;
};

class Rook : public Piece {
  private:
    bool canCastle;

  public:
    Rook(pieceColor pColor, bool pCanCastle = true);
    bool canMove(std::shared_ptr<Board> board, std::shared_ptr<Square> start,
                 std::shared_ptr<Square> end) override;
    bool getCanCastle() override;
    void setCanCastle(bool flag) override;
};

class Knight : public Piece {
  public:
    Knight(pieceColor pColor);
    bool canMove(std::shared_ptr<Board> board, std::shared_ptr<Square> start,
                 std::shared_ptr<Square> end) override;
};

class Bishop : public Piece {
  public:
    Bishop(pieceColor pColor);
    bool canMove(std::shared_ptr<Board> board, std::shared_ptr<Square> start,
                 std::shared_ptr<Square> end) override;
};

class King : public Piece {
  private:
    bool canCastle;

  public:
    King(pieceColor pColor, bool pCanCastle = true);
    bool canMove(std::shared_ptr<Board> board, std::shared_ptr<Square> start,
                 std::shared_ptr<Square> end) override;
    void setCanCastle(bool flag) override;
};

class Queen : public Piece {
  private:
    std::shared_ptr<Rook> dummyRook;
    std::shared_ptr<Bishop> dummyBishop;

  public:
    Queen(pieceColor pColor);
    bool canMove(std::shared_ptr<Board> board, std::shared_ptr<Square> start,
                 std::shared_ptr<Square> end) override;
};

#endif
