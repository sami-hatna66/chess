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
using namespace std;

class ChessPiece {
private:
    int pos[2]; // [Row, Column]
    bool color; // true = black, false = white
    string type;
public:
    ChessPiece(int pRow, int pCol, bool pColor, string pType);
    int getRow();
    int getCol();
    bool getColor();
    const char* getImgName();
};

class Pawn : public ChessPiece {
public:
    Pawn(int pRow, int pCol, bool pColor);
};

class Bishop : public ChessPiece {
public:
    Bishop(int pRow, int pCol, bool pColor);
};

class Rook : public ChessPiece {
public:
    Rook(int pRow, int pCol, bool pColor);
};

class Knight : public ChessPiece {
public:
    Knight(int pRow, int pCol, bool pColor);
};

class King : public ChessPiece {
public:
    King(int pRow, int pCol, bool pColor);
};

class Queen : public ChessPiece {
public:
    Queen(int pRow, int pCol, bool pColor);
};

#endif /* pieces_hpp */
