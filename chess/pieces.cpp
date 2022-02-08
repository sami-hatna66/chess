//
//  pieces.cpp
//  chess
//
//  Created by Sami Hatna on 01/02/2022.
//

#include "pieces.hpp"
#include <iostream>
#include <string>
#include <SDL.h>
using namespace std;

ChessPiece::ChessPiece(int pRow, int pCol, colors pColor, types pType, bool pIsAlive) {
    pos[0] = pRow;
    pos[1] = pCol;
    color = pColor;
    type = pType;
    isFirstMove = true;
    isAlive = pIsAlive;
}
    
int ChessPiece::getRow() {
    return pos[0];
}

int ChessPiece::getCol() {
    return pos[1];
}

void ChessPiece::movePiece(int newCol, int newRow) {
    pos[0] = newRow;
    pos[1] = newCol;
}

colors ChessPiece::getColor() {
    return color;
}

types ChessPiece::getType() {
    return type;
}

bool ChessPiece::getIsFirstMove() {
    return isFirstMove;
}

void ChessPiece::toggleFirstMove() {
    isFirstMove = false;
}

void ChessPiece::toggleIsAlive() {
    isAlive = false;
}

bool ChessPiece::getIsAlive() {
    return isAlive;
}

const char* ChessPiece::getImgName() {
    string result = color == black ? "blk" : "wht";
    string tempType;
    switch (type) {
        case pawn:
            tempType = "pawn"; break;
        case rook:
            tempType = "rook"; break;
        case knight:
            tempType = "knight"; break;
        case bishop:
            tempType = "bishop"; break;
        case king:
            tempType = "king"; break;
        default:
            tempType = "queen"; break;
    }
    tempType[0] = toupper(tempType[0]);
    result = result + tempType + ".png";
    return result.c_str();
}

//Pawn::Pawn(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "pawn") {
//
//}
//
//Bishop::Bishop(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "bishop") {
//        
//}
//
//Rook::Rook(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "Rook") {
//        
//}
//
//Knight::Knight(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "knight") {
//    
//}
//
//King::King(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "king") {
//        
//    }
//
//Queen::Queen(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "queen") {
//        
//}

