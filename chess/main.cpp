//
//  main.cpp
//  chess
//
//  Created by Sami Hatna on 30/01/2022.
//

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <map>
#include "pieces.hpp"
using namespace std;

void init();
void draw();
void close();
void drawPiece();
ChessPiece* checkIfPieceIn(int col, int row);
void clicked(int col, int row);
void chooseMove(int col, int row);
void pawnLogic(ChessPiece* piece, int col, int row);
void rookLogic(ChessPiece* piece, int col, int row);
void knightLogic(ChessPiece* piece, int col, int row);
void bishopLogic(ChessPiece* piece, int col, int row);
void queenLogic(ChessPiece* piece, int col, int row);
void kingLogic(ChessPiece* piece, int col, int row);
bool checkCheck(bool isDraw);
bool checkCheckMate();
void renderText(char *text, int x, int y, string color, int h);

static SDL_Window* win = NULL;
SDL_Surface* surface = NULL;
static SDL_Renderer* render = NULL;

struct gameStats {
    int turnCount = 0;
    enum {choosingPiece, choosingMove} phase = choosingPiece; // "choosingPiece" or "choosingMove"
    enum {black, white} activePlayer = black; // "black" or "white"
    int prevX = -1;
    int prevY = -1;
    enum {blackCheck, whiteCheck, both, neither} inCheck = neither;
    bool isCheckmate = false;
};

map<string, vector<int>> colorMap = {
    { "red", {255, 0, 0} },
    {"white", {255, 255, 255} }
};

gameStats gameInstance;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    win = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 450, SDL_WINDOW_SHOWN);
    SDL_Surface *iconSurface = IMG_Load("appIcon.png");
    SDL_SetWindowIcon(win, iconSurface);
}

enum boardStates {notActive, activeNotMove, activeMove};
boardStates highlightBoardMap[8][8] = {{notActive}};

//vector<ChessPiece> whitePawns = {
//    ChessPiece(1, 0, white, pawn, true), ChessPiece(1, 1, white, pawn, true),
//    ChessPiece(1, 2, white, pawn, true), ChessPiece(1, 3, white, pawn, true),
//    ChessPiece(1, 4, white, pawn, true), ChessPiece(1, 5, white, pawn, true),
//    ChessPiece(1, 6, white, pawn, true), ChessPiece(1, 7, white, pawn, true),
//};
//
//ChessPiece whiteRook1 = ChessPiece(0, 0, white, rook, true);
//ChessPiece whiteRook2 = ChessPiece(0, 7, white, rook, true);
//ChessPiece whiteKnight1 = ChessPiece(0, 1, white, knight, true);
//ChessPiece whiteKnight2 = ChessPiece(0, 6, white, knight, true);
//ChessPiece whiteBishop1 = ChessPiece(0, 2, white, bishop, true);
//ChessPiece whiteBishop2 = ChessPiece(0, 5, white, bishop, true);
//ChessPiece whiteQueen = ChessPiece(0, 3, white, queen, true);
//ChessPiece whiteKing = ChessPiece(0, 4, white, king, true);
//
//vector<ChessPiece> blackPawns = {
//    ChessPiece(6, 0, black, pawn, true), ChessPiece(6, 1, black, pawn, true),
//    ChessPiece(6, 2, black, pawn, true), ChessPiece(6, 3, black, pawn, true),
//    ChessPiece(6, 4, black, pawn, true), ChessPiece(6, 5, black, pawn, true),
//    ChessPiece(6, 6, black, pawn, true), ChessPiece(6, 7, black, pawn, true),
//};
//
//ChessPiece blackRook1 = ChessPiece(7, 0, black, rook, true);
//ChessPiece blackRook2 = ChessPiece(7, 7, black, rook, true);
//ChessPiece blackKnight1 = ChessPiece(7, 1, black, knight, true);
//ChessPiece blackKnight2 = ChessPiece(7, 6, black, knight, true);
//ChessPiece blackBishop1 = ChessPiece(7, 2, black, bishop, true);
//ChessPiece blackBishop2 = ChessPiece(7, 5, black, bishop, true);
//ChessPiece blackQueen = ChessPiece(7, 3, black, queen, true);
//ChessPiece blackKing = ChessPiece(7, 4, black, king, true);

vector<ChessPiece> whitePawns = {
    ChessPiece(1, 0, white, pawn, true), ChessPiece(3, 1, white, pawn, true),
    ChessPiece(1, 2, white, pawn, false), ChessPiece(1, 3, white, pawn, false),
    ChessPiece(3, 4, white, pawn, true), ChessPiece(1, 5, white, pawn, true),
    ChessPiece(1, 6, white, pawn, true), ChessPiece(1, 7, white, pawn, true),
};

ChessPiece whiteRook1 = ChessPiece(0, 0, white, rook, true);
ChessPiece whiteRook2 = ChessPiece(0, 7, white, rook, true);
ChessPiece whiteKnight1 = ChessPiece(0, 1, white, knight, true);
ChessPiece whiteKnight2 = ChessPiece(2, 5, white, knight, true);
ChessPiece whiteBishop1 = ChessPiece(0, 2, white, bishop, false);
ChessPiece whiteBishop2 = ChessPiece(0, 5, white, bishop, true);
ChessPiece whiteQueen = ChessPiece(1, 4, white, queen, true);
ChessPiece whiteKing = ChessPiece(0, 4, white, king, true);

vector<ChessPiece> blackPawns = {
    ChessPiece(6, 0, black, pawn, true), ChessPiece(6, 1, black, pawn, true),
    ChessPiece(6, 2, black, pawn, true), ChessPiece(6, 3, black, pawn, false),
    ChessPiece(4, 4, black, pawn, true), ChessPiece(6, 5, black, pawn, true),
    ChessPiece(6, 6, black, pawn, true), ChessPiece(6, 7, black, pawn, true),
};

ChessPiece blackRook1 = ChessPiece(7, 0, black, rook, true);
ChessPiece blackRook2 = ChessPiece(7, 7, black, rook, false);
ChessPiece blackKnight1 = ChessPiece(7, 1, black, knight, false);
ChessPiece blackKnight2 = ChessPiece(7, 6, black, knight, true);
ChessPiece blackBishop1 = ChessPiece(4, 2, black, bishop, true);
ChessPiece blackBishop2 = ChessPiece(3, 6, black, bishop, true);
ChessPiece blackQueen = ChessPiece(5, 1, black, queen, true);
ChessPiece blackKing = ChessPiece(7, 4, black, king, true);

ChessPiece* pieceArray[32] = {
    &whiteRook1, &whiteRook2, &whiteKnight1, &whiteKnight2, &whiteBishop1,
    &whiteBishop2, &whiteQueen, &whiteKing, &whitePawns[0], &whitePawns[1],
    &whitePawns[2], &whitePawns[3], &whitePawns[4], &whitePawns[5], &whitePawns[6],
    &whitePawns[7], &blackRook1, &blackRook2, &blackKnight1, &blackKnight2,
    &blackBishop1, &blackBishop2, &blackQueen, &blackKing, &blackPawns[0],
    &blackPawns[1], &blackPawns[2], &blackPawns[3], &blackPawns[4], &blackPawns[5],
    &blackPawns[6], &blackPawns[7]
};

bool checkCheckMate() {
    int prevPos[] = {whiteKing.getRow(), whiteKing.getCol()};
    kingLogic(&whiteKing, whiteKing.getCol(), whiteKing.getRow());
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (highlightBoardMap[i][j] == activeMove) {
                whiteKing.movePiece(j, i);
                checkCheck(false);
                if (gameInstance.inCheck != gameStats::whiteCheck && gameInstance.inCheck != gameStats::both) {
                    cout << endl << "check mate" << endl;
                    gameInstance.isCheckmate = true;
                }
            }
            highlightBoardMap[i][j] = notActive;
        }
    }
    whiteKing.movePiece(prevPos[1], prevPos[0]);
    
    prevPos[0] = blackKing.getRow();
    prevPos[1] = blackKing.getCol();
    kingLogic(&blackKing, blackKing.getCol(), blackKing.getRow());
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (highlightBoardMap[i][j] == activeMove) {
                blackKing.movePiece(j, i);
                checkCheck(false);
                if (gameInstance.inCheck != gameStats::blackCheck && gameInstance.inCheck != gameStats::both) {
                    gameInstance.isCheckmate = true;
                }
            }
            highlightBoardMap[i][j] = notActive;
        }
    }
    blackKing.movePiece(prevPos[1], prevPos[0]);
    
    draw();
    
    return false;
}

bool checkCheck(bool isDraw) {
    bool isBlackInCheck = false;
    bool isWhiteInCheck = false;
    for (int i = 0; i < 32; i++) {
        if (pieceArray[i]->getIsAlive()) {
            switch (pieceArray[i]->getType()) {
                case rook:
                    rookLogic(pieceArray[i], pieceArray[i]->getCol(), pieceArray[i]->getRow()); break;
                case knight:
                    knightLogic(pieceArray[i], pieceArray[i]->getCol(), pieceArray[i]->getRow()); break;
                case bishop:
                    bishopLogic(pieceArray[i], pieceArray[i]->getCol(), pieceArray[i]->getRow()); break;
                case queen:
                    queenLogic(pieceArray[i], pieceArray[i]->getCol(), pieceArray[i]->getRow()); break;
                case king:
                    kingLogic(pieceArray[i], pieceArray[i]->getCol(), pieceArray[i]->getRow()); break;
                default:
                    pawnLogic(pieceArray[i], pieceArray[i]->getCol(), pieceArray[i]->getRow()); break;
            }
            for (int j = 0; j < 8; j++) {
                for (int k = 0; k < 8; k++) {
                    if (highlightBoardMap[j][k] == activeMove && checkIfPieceIn(k, j) != NULL) {
                        if (checkIfPieceIn(k, j)->getType() == king && pieceArray[i]->getColor() != checkIfPieceIn(k, j)->getColor()) {
                            if (checkIfPieceIn(k, j)->getColor() == black) {
                                isBlackInCheck = true;
                            }
                            else {
                                isWhiteInCheck = true;

                            }
                            highlightBoardMap[j][k] = notActive;
                        }
                    }
                    highlightBoardMap[j][k] = notActive;
                }
            }
        }
    }
    
    if (isDraw) {
        if (isBlackInCheck && isWhiteInCheck) {
            gameInstance.inCheck = gameStats::both;
            cout << "both check";
        }
        else if (isBlackInCheck) {
            gameInstance.inCheck = gameStats::blackCheck;
            cout << "black check";
        }
        else if (isWhiteInCheck) {
            gameInstance.inCheck = gameStats::whiteCheck;
            cout << "white check";
        }
        else {
            gameInstance.inCheck = gameStats::neither;
        }
        
        draw();
    }
    
    if (isWhiteInCheck || isBlackInCheck) {
        return true;
    }
    else {
        return false;
    }
}

void drawPiece(const char* imgName, int xPos, int yPos) {
    SDL_Surface * image = IMG_Load(imgName);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(render, image);
    SDL_Rect pieceRect; pieceRect.x = 50 * xPos; pieceRect.y = 50 * yPos; pieceRect.w = 50; pieceRect.h = 50;
    SDL_RenderCopy(render, texture, NULL, &pieceRect);
}

ChessPiece* checkIfPieceIn(int col, int row) {
    for (int i = 0; i < 32; i++) {
        if (pieceArray[i]->getIsAlive() && pieceArray[i]->getRow() == row && pieceArray[i]->getCol() == col) {
            return pieceArray[i];
        }
    }
    return NULL;
}

void clicked(int col, int row) {
    if (checkIfPieceIn(col, row) != NULL && checkIfPieceIn(col, row)->getColor() == gameInstance.activePlayer) {
        gameInstance.phase = gameStats::choosingMove;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                highlightBoardMap[i][j] = notActive;
            }
        }
        
        if (gameInstance.prevY != row || gameInstance.prevX != col) {
            highlightBoardMap[row][col] = activeNotMove;
            ChessPiece* pieceCheck = checkIfPieceIn(col, row);
            
            switch (pieceCheck->getType()) {
                case rook:
                    rookLogic(pieceCheck, col, row); break;
                case knight:
                    knightLogic(pieceCheck, col, row); break;
                case bishop:
                    bishopLogic(pieceCheck, col, row); break;
                case queen:
                    queenLogic(pieceCheck, col, row); break;
                case king:
                    kingLogic(pieceCheck, col, row); break;
                default:
                    pawnLogic(pieceCheck, col, row); break;
            }
            
            gameInstance.prevX = col; gameInstance.prevY = row;
        }
        else {
            gameInstance.prevX = -1; gameInstance.prevY = -1;
        }
        draw();
    }
}

void chooseMove(int col, int row) {
    if (highlightBoardMap[row][col] == activeMove) {
        ChessPiece* activePiece = checkIfPieceIn(gameInstance.prevX, gameInstance.prevY);
        activePiece->movePiece(col, row);
        
        for (int i = 0; i < 32; i++) {
            if (pieceArray[i]->getCol() == col && pieceArray[i]->getRow() == row && pieceArray[i] != activePiece) {
                pieceArray[i]->toggleIsAlive();
            }
        }
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                highlightBoardMap[i][j] = notActive;
            }
        }
        activePiece->toggleFirstMove();
        gameInstance.activePlayer = (gameInstance.activePlayer == gameStats::black) ? gameStats::white : gameStats::black;
        
        bool result = checkCheck(true);
        if (result) {
            checkCheckMate();
        }
    
        gameInstance.phase = gameStats::choosingPiece;
    }
    else {
        clicked(col, row);
    }
}

void renderText(char *inpText, int x, int y, string color, int h) {
    SDL_Color textColor = { static_cast<Uint8>(colorMap[color][0]), static_cast<Uint8>(colorMap[color][1]), static_cast<Uint8>(colorMap[color][2]) };
    TTF_Font *font = TTF_OpenFont("chess.ttf", 12);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, inpText, textColor);
    SDL_Texture* text = SDL_CreateTextureFromSurface(render, textSurface);
    SDL_Rect r;
    r.x = x; r.y = y; r.w = textSurface->w; r.h = textSurface->h;
    if (h != 0) { r.h = h; }
    SDL_RenderCopy(render, text, NULL, &r);
}

void draw() {
    cout << render;
    SDL_SetRenderDrawColor(render, 241, 221, 206, 255);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 97, 61, 61, 255);
    bool isBlack = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (isBlack) {
                if (highlightBoardMap[i][j] == activeNotMove) {
                    SDL_SetRenderDrawColor(render, 55, 136, 5, 255);
                }
                else if (highlightBoardMap[i][j] == activeMove) {
                    SDL_SetRenderDrawColor(render, 38, 88, 15, 255);
                }
                else {
                    SDL_SetRenderDrawColor(render, 97, 61, 61, 255);
                }
                isBlack = false;
            }
            else {
                if (highlightBoardMap[i][j] == activeNotMove) {
                    SDL_SetRenderDrawColor(render, 55, 136, 5, 255);
                }
                else if (highlightBoardMap[i][j] == activeMove) {
                    SDL_SetRenderDrawColor(render, 38, 88, 15, 255);
                }
                else {
                    SDL_SetRenderDrawColor(render, 241, 221, 206, 255);
                }
                isBlack = true;
            }
            SDL_Rect r;
            r.x = j * 50; r.y = i * 50; r.w = 50; r.h = 50;
            SDL_RenderFillRect(render, &r);
        }
        isBlack = isBlack ? false : true;
    }
    
    for (int i = 0; i < 32; i++) {
        if (pieceArray[i]->getIsAlive()) {
            drawPiece(pieceArray[i]->getImgName(), pieceArray[i]->getCol(), pieceArray[i]->getRow());
        }
    }
    
    SDL_Rect r;
    r.x = 0; r.y = 400; r.w = 400; r.h = 50;
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderFillRect(render, &r);
    
    
    if (gameInstance.activePlayer == gameStats::black) {
        char t[] = "Black's turn";
        renderText(t, 5, 405, "white", 0);
    }
    else {
        char t[] = "White's turn";
        renderText(t, 5, 405, "white", 0);
    }
    
    if (gameInstance.inCheck == gameStats::both) {
        char t[] = "Black and white in check";
        renderText(t, 5, 425, "white", 0);
    }
    else if (gameInstance.inCheck == gameStats::blackCheck) {
        char t[] = "Black in check";
        renderText(t, 5, 425, "white", 0);
    }
    else if (gameInstance.inCheck == gameStats::whiteCheck) {
        char t[] = "White in check";
        renderText(t, 5, 425, "white", 0);
    }
    
    if (gameInstance.isCheckmate) {
        char t[] = "CHECKMATE";
        renderText(t, 305, 402, "red", 50);
    }
    
    
    SDL_RenderPresent(render);
}

void pawnLogic(ChessPiece* piece, int col, int row) {
    if (piece->getColor() == black) {
        if (checkIfPieceIn(col, row - 1) == NULL) {
            highlightBoardMap[row - 1][col] = activeMove;
            if (piece->getIsFirstMove() && checkIfPieceIn(col, row - 2) == NULL) {
                highlightBoardMap[row - 2][col] = activeMove;
            }
        }
        if (checkIfPieceIn(col + 1, row - 1) != NULL && checkIfPieceIn(col + 1, row - 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row - 1][col + 1] = activeMove;
        }
        if (checkIfPieceIn(col - 1, row - 1) != NULL && checkIfPieceIn(col - 1, row - 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row - 1][col - 1] = activeMove;
        }
    }
    else {
        if (checkIfPieceIn(col, row + 1) == NULL) {
            highlightBoardMap[row + 1][col] = activeMove;
            if (piece->getIsFirstMove() && checkIfPieceIn(col, row + 2) == NULL) {
                highlightBoardMap[row + 2][col] = activeMove;
            }
        }
        if (checkIfPieceIn(col + 1, row + 1) != NULL && checkIfPieceIn(col + 1, row + 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row + 1][col + 1] = activeMove;
        }
        if (checkIfPieceIn(col - 1, row + 1) != NULL && checkIfPieceIn(col - 1, row + 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row + 1][col - 1] = activeMove;
        }
    }
}

void rookLogic(ChessPiece* piece, int col, int row) {
    for (int i = row - 1; i >= 0; i--) {
        if (checkIfPieceIn(col, i) == NULL) {
            highlightBoardMap[i][col] = activeMove;
        }
        else if (checkIfPieceIn(col, i)->getColor() != piece->getColor()) {
            highlightBoardMap[i][col] = activeMove;
            break;
        }
        else {
            break;
        }
    }
    
    for (int j = row + 1; j < 8; j++) {
        if (checkIfPieceIn(col, j) == NULL) {
            highlightBoardMap[j][col] = activeMove;
        }
        else if (checkIfPieceIn(col, j)->getColor() != piece->getColor()) {
            highlightBoardMap[j][col] = activeMove;
            break;
        }
        else {
            break;
        }
    }
    
    for (int k = col - 1; k >= 0; k--) {
        if (checkIfPieceIn(k, row) == NULL) {
            highlightBoardMap[row][k] = activeMove;
        }
        else if (checkIfPieceIn(k, row)->getColor() != piece->getColor()) {
            highlightBoardMap[row][k] = activeMove;
            break;
        }
        else {
            break;
        }
    }
    
    for (int l = col + 1; l < 8; l++) {
        if (checkIfPieceIn(l, row) == NULL) {
            highlightBoardMap[row][l] = activeMove;
        }
        else if (checkIfPieceIn(l, row)->getColor() != piece->getColor()) {
            highlightBoardMap[row][l] = activeMove;
            break;
        }
        else {
            break;
        }
    }
}

void knightLogic(ChessPiece* piece, int col, int row) {
    if (row - 2 >= 0 && col - 1 >= 0) {
        if (checkIfPieceIn(col - 1, row - 2) == NULL || checkIfPieceIn(col - 1, row - 2)->getColor() != piece->getColor()) {
            highlightBoardMap[row - 1][col] = activeNotMove;
            highlightBoardMap[row - 2][col] = activeNotMove;
            highlightBoardMap[row - 2][col - 1] = activeMove;
        }
    }
    if (row - 1 >= 0 && col - 2 >= 0) {
        if (checkIfPieceIn(col - 2, row - 1) == NULL || checkIfPieceIn(col - 2, row - 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row - 1][col] = activeNotMove;
            highlightBoardMap[row - 1][col - 1] = activeNotMove;
            highlightBoardMap[row - 1][col - 2] = activeMove;
        }
    }
    if (row - 2 >= 0 && col + 1 < 8) {
        if (checkIfPieceIn(col + 1, row - 2) == NULL || checkIfPieceIn(col + 1, row - 2)->getColor() != piece->getColor()) {
            highlightBoardMap[row - 1][col] = activeNotMove;
            highlightBoardMap[row - 2][col] = activeNotMove;
            highlightBoardMap[row - 2][col + 1] = activeMove;
        }
    }
    if (row - 1 >= 0 && col + 2 < 8) {
        if (checkIfPieceIn(col + 2, row - 1) == NULL || checkIfPieceIn(col + 2, row - 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row - 1][col] = activeNotMove;
            highlightBoardMap[row - 1][col + 1] = activeNotMove;
            highlightBoardMap[row - 1][col + 2] = activeMove;
        }
    }
    if (row + 2 < 8 && col - 1 >= 0) {
        if (checkIfPieceIn(col - 1, row + 2) == NULL || checkIfPieceIn(col - 1, row + 2)->getColor() != piece->getColor()) {
            highlightBoardMap[row + 1][col] = activeNotMove;
            highlightBoardMap[row + 2][col] = activeNotMove;
            highlightBoardMap[row + 2][col - 1] = activeMove;
        }
    }
    if (row + 1 < 8 && col - 2 >= 0) {
        if (checkIfPieceIn(col - 2, row + 1) == NULL || checkIfPieceIn(col - 2, row + 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row + 1][col] = activeNotMove;
            highlightBoardMap[row + 1][col - 1] = activeNotMove;
            highlightBoardMap[row + 1][col - 2] = activeMove;
        }
    }
    if (row + 2 < 8 && col + 1 < 8) {
        if (checkIfPieceIn(col + 1, row + 2) == NULL || checkIfPieceIn(col + 1, row + 2)->getColor() != piece->getColor()) {
            highlightBoardMap[row + 1][col] = activeNotMove;
            highlightBoardMap[row + 2][col] = activeNotMove;
            highlightBoardMap[row + 2][col + 1] = activeMove;
        }
    }
    if (row + 1 < 8 && col + 2 < 8) {
        if (checkIfPieceIn(col + 2, row + 1) == NULL || checkIfPieceIn(col + 2, row + 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row + 1][col] = activeNotMove;
            highlightBoardMap[row + 1][col + 1] = activeNotMove;
            highlightBoardMap[row + 1][col + 2] = activeMove;
        }
    }
}

void bishopLogic(ChessPiece* piece, int col, int row) {
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (checkIfPieceIn(j, i) == NULL) {
            highlightBoardMap[i][j] = activeMove;
        }
        else if (checkIfPieceIn(j, i)->getColor() != piece->getColor()) {
            highlightBoardMap[i][j] = activeMove; break;
        }
        else { break; }
    }
    for (int i = row - 1, j = col + 1; i >= 0 && j < 8; i--, j++) {
        if (checkIfPieceIn(j, i) == NULL) {
            highlightBoardMap[i][j] = activeMove;
        }
        else if (checkIfPieceIn(j, i)->getColor() != piece->getColor()) {
            highlightBoardMap[i][j] = activeMove; break;
        }
        else { break; }
    }
    for (int i = row + 1, j = col + 1; i < 8 && j < 8; i++, j++) {
        if (checkIfPieceIn(j, i) == NULL) {
            highlightBoardMap[i][j] = activeMove;
        }
        else if (checkIfPieceIn(j, i)->getColor() != piece->getColor()) {
            highlightBoardMap[i][j] = activeMove; break;
        }
        else { break; }
    }
    for (int i = row + 1, j = col - 1; i < 8 && j >= 0; i++, j--) {
        if (checkIfPieceIn(j, i) == NULL) {
            highlightBoardMap[i][j] = activeMove;
        }
        else if (checkIfPieceIn(j, i)->getColor() != piece->getColor()) {
            highlightBoardMap[i][j] = activeMove; break;
        }
        else { break; }
    }
}

void queenLogic(ChessPiece* piece, int col, int row) {
    rookLogic(piece, col, row);
    bishopLogic(piece, col, row);
}

void kingLogic(ChessPiece* piece, int col, int row) {
    if (row - 1 >= 0) {
        if (checkIfPieceIn(col, row - 1) == NULL || checkIfPieceIn(col, row - 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row - 1][col] = activeMove;
        }
    }
    if (row + 1 < 8) {
        if (checkIfPieceIn(col, row + 1) == NULL || checkIfPieceIn(col, row + 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row + 1][col] = activeMove;
        }
    }
    if (col - 1 >= 0) {
        if (checkIfPieceIn(col - 1, row) == NULL || checkIfPieceIn(col - 1, row)->getColor() != piece->getColor()) {
            highlightBoardMap[row][col - 1] = activeMove;
        }
    }
    if (col + 1 < 8) {
        if (checkIfPieceIn(col + 1, row) == NULL || checkIfPieceIn(col + 1, row)->getColor() != piece->getColor()) {
            highlightBoardMap[row][col + 1] = activeMove;
        }
    }
    if (row - 1 >= 0 && col - 1 >= 0) {
        if (checkIfPieceIn(col - 1, row - 1) == NULL || checkIfPieceIn(col - 1, row - 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row - 1][col - 1] = activeMove;
        }
    }
    if (row - 1 >= 0 && col + 1 < 8) {
        if (checkIfPieceIn(col + 1, row - 1) == NULL || checkIfPieceIn(col + 1, row - 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row - 1][col + 1] = activeMove;
        }
    }
    if (row + 1 < 8 && col - 1 >= 0) {
        if (checkIfPieceIn(col - 1, row + 1) == NULL || checkIfPieceIn(col - 1, row + 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row + 1][col - 1] = activeMove;
        }
    }
    if (row + 1 < 8 && col + 1 < 8) {
        if (checkIfPieceIn(col + 1, row + 1) == NULL || checkIfPieceIn(col + 1, row + 1)->getColor() != piece->getColor()) {
            highlightBoardMap[row + 1][col + 1] = activeMove;
        }
    }
}

void close() {
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, const char * argv[]) {
    init();
    
    render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    draw();
    
    bool isQuit = false;
    SDL_Event event;
    while (!isQuit) {
        if (SDL_PollEvent( & event)) {
            if (event.type == SDL_QUIT) {
                isQuit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Delay(10);
                if (gameInstance.phase == gameStats::choosingPiece) {
                    clicked(floor(event.motion.x / 50), floor(event.motion.y / 50));
                }
                else if (gameInstance.phase == gameStats::choosingMove) {
                    chooseMove(floor(event.motion.x / 50), floor(event.motion.y / 50));
                }
            }
        }
    }
    
    close();
    
    return 0;
}

// Adding image to C++ projects: https://stackoverflow.com/questions/35093704/how-to-add-an-image-to-a-c-project-in-xcode
