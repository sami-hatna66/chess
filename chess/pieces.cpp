//
//  pieces.cpp
//  chess
//
//  Created by Sami Hatna on 01/02/2022.
//

#include "pieces.hpp"
#include <iostream>
#include <string>
using namespace std;

ChessPiece::ChessPiece(int pRow, int pCol, bool pColor, string pType) {
    pos[0] = pRow;
    pos[1] = pCol;
    color = pColor;
    type = pType;
}
    
int ChessPiece::getRow() {
    return pos[0];
}

int ChessPiece::getCol() {
    return pos[1];
}
    
bool ChessPiece::getColor() {
    return color;
}

const char* ChessPiece::getImgName() {
    string result = color ? "blk" : "wht";
    string tempType = type;
    tempType[0] = toupper(tempType[0]);
    result = result + tempType + ".png";
    return result.c_str();
}

Pawn::Pawn(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "pawn") {

}

Bishop::Bishop(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "bishop") {
        
}

Rook::Rook(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "Rook") {
        
}

Knight::Knight(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "knight") {
    
}

King::King(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "king") {
        
    }

Queen::Queen(int pRow, int pCol, bool pColor) : ChessPiece(pRow, pCol, pColor, "queen") {
        
}

