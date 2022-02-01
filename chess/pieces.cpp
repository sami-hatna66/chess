//
//  pieces.cpp
//  chess
//
//  Created by Sami Hatna on 01/02/2022.
//

#include <iostream>
#include <string>
using namespace std;

class ChessPiece {
private:
    int pos[2]; // [Row, Column]
    bool color; // true = black, false = white
    string type;
public:
    ChessPiece(int pRow, int pCol, bool pColor, string pType) {
        pos[0] = pRow;
        pos[1] = pCol;
        color = pColor;
        type = pType;
    }
    
    int getRow() {
        return pos[0];
    }
    int getCol() {
        return pos[1];
    }
    
    bool getColor() {
        return color;
    }
};

class Pawn : public ChessPiece {
public:
    Pawn(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "pawn") {
        
    }
};

class Bishop : public ChessPiece {
public:
    Bishop(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "bishop") {
        
    }
};

class Rook : public ChessPiece {
public:
    Rook(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "Rook") {
        
    }
};

class Knight : public ChessPiece {
public:
    Knight(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "knight") {
        
    }
};

class King : public ChessPiece {
public:
    King(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "king") {
        
    }
};

class Queen : public ChessPiece {
public:
    Queen(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "queen") {
        
    }
};
