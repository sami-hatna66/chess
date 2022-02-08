//
//  pieces.hpp
//  chess
//
//  Created by Sami Hatna on 01/02/2022.
//

#ifndef pieces_hpp
#define pieces_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <SDL.h>
using namespace std;

enum colors {black, white};
enum types {pawn, rook, knight, bishop, king, queen};

class ChessPiece {
private:
    int pos[2]; // [Row, Column]
    colors color;
    types type;
    bool isFirstMove;
    bool isAlive;
public:
    ChessPiece(int pRow, int pCol, colors pColor, types pType, bool pIsAlive);
    int getRow();
    int getCol();
    colors getColor();
    types getType();
    bool getIsFirstMove();
    bool getIsAlive();
    const char* getImgName();
    void movePiece(int newCol, int newRow);
    void toggleFirstMove();
    void toggleIsAlive();
};

//class Pawn : public ChessPiece {
//public:
//    Pawn(int pRow, int pCol, bool pColor);
//};
//
//class Bishop : public ChessPiece {
//public:
//    Bishop(int pRow, int pCol, bool pColor);
//};
//
//class Rook : public ChessPiece {
//public:
//    Rook(int pRow, int pCol, bool pColor);
//};
//
//class Knight : public ChessPiece {
//public:
//    Knight(int pRow, int pCol, bool pColor);
//};
//
//class King : public ChessPiece {
//public:
//    King(int pRow, int pCol, bool pColor);
//};
//
//class Queen : public ChessPiece {
//public:
//    Queen(int pRow, int pCol, bool pColor);
//};

#endif /* pieces_hpp */
