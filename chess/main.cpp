//
//  main.cpp
//  chess
//
//  Created by Sami Hatna on 30/01/2022.
//

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "pieces.hpp"
using namespace std;

void init();
void draw();
void close();
void drawPiece();
void pieceClicked(int x, int y);

SDL_Window* win = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* render = NULL;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    win = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, SDL_WINDOW_SHOWN);
}

int prevX = -1;
int prevY = -1;
bool highlightBoardMap[8][8] = {{0}};

vector<ChessPiece> whitePawns = {
    ChessPiece(1, 0, false, "pawn"), ChessPiece(1, 1, false, "pawn"),
    ChessPiece(1, 2, false, "pawn"), ChessPiece(1, 3, false, "pawn"),
    ChessPiece(1, 4, false, "pawn"), ChessPiece(1, 5, false, "pawn"),
    ChessPiece(1, 6, false, "pawn"), ChessPiece(1, 7, false, "pawn"),
};

ChessPiece whiteRook1 = ChessPiece(0, 0, false, "rook");
ChessPiece whiteRook2 = ChessPiece(0, 7, false, "rook");
ChessPiece whiteKnight1 = ChessPiece(0, 1, false, "knight");
ChessPiece whiteKnight2 = ChessPiece(0, 6, false, "knight");
ChessPiece whiteBishop1 = ChessPiece(0, 2, false, "bishop");
ChessPiece whiteBishop2 = ChessPiece(0, 5, false, "bishop");
ChessPiece whiteQueen = ChessPiece(0, 3, false, "queen");
ChessPiece whiteKing = ChessPiece(0, 4, false, "king");

vector<ChessPiece> blackPawns = {
    ChessPiece(6, 0, true, "pawn"), ChessPiece(6, 1, true, "pawn"),
    ChessPiece(6, 2, true, "pawn"), ChessPiece(6, 3, true, "pawn"),
    ChessPiece(6, 4, true, "pawn"), ChessPiece(6, 5, true, "pawn"),
    ChessPiece(6, 6, true, "pawn"), ChessPiece(6, 7, true, "pawn"),
};

ChessPiece blackRook1 = ChessPiece(7, 0, true, "rook");
ChessPiece blackRook2 = ChessPiece(7, 7, true, "rook");
ChessPiece blackKnight1 = ChessPiece(7, 1, true, "knight");
ChessPiece blackKnight2 = ChessPiece(7, 6, true, "knight");
ChessPiece blackBishop1 = ChessPiece(7, 2, true, "bishop");
ChessPiece blackBishop2 = ChessPiece(7, 5, true, "bishop");
ChessPiece blackQueen = ChessPiece(7, 4, true, "queen");
ChessPiece blackKing = ChessPiece(7, 3, true, "king");

ChessPiece* pieceArray[32] = {
    &whiteRook1, &whiteRook2, &whiteKnight1, &whiteKnight2, &whiteBishop1,
    &whiteBishop2, &whiteQueen, &whiteKing, &whitePawns[0], &whitePawns[1],
    &whitePawns[2], &whitePawns[3], &whitePawns[4], &whitePawns[5], &whitePawns[6],
    &whitePawns[7], &blackRook1, &blackRook2, &blackKnight1, &blackKnight2,
    &blackBishop1, &blackBishop2, &blackQueen, &blackKing, &blackPawns[0],
    &blackPawns[1], &blackPawns[2], &blackPawns[3], &blackPawns[4], &blackPawns[5],
    &blackPawns[6], &blackPawns[7]
};

void drawPiece(const char* imgName, int xPos, int yPos) {
    SDL_Surface * image = IMG_Load(imgName);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(render, image);
    SDL_Rect pieceRect; pieceRect.x = 50 * xPos; pieceRect.y = 50 * yPos; pieceRect.w = 50; pieceRect.h = 50;
    SDL_RenderCopy(render, texture, NULL, &pieceRect);
}

ChessPiece* checkIfPieceClicked(int x, int y) {
    for (int i = 0; i < 32; i++) {
        if (pieceArray[i]->getRow() == y && pieceArray[i]->getCol() == x) {
            return pieceArray[i];
        }
    }
    return NULL;
}

void clicked(int x, int y) {
    if (checkIfPieceClicked(x, y) != NULL) {
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                highlightBoardMap[i][j] = false;
            }
        }
        
        if (prevY != y || prevX != x) {
            highlightBoardMap[y][x] = true;
            ChessPiece* pieceCheck = checkIfPieceClicked(x, y);
            if (pieceCheck->getType() == "pawn") {
                if (pieceCheck->getColor()) {
                    highlightBoardMap[y - 1][x] = true;
                }
                else {
                    highlightBoardMap[y + 1][x] = true;
                }
            }
            prevX = x; prevY = y;
        }
        else {
            prevX = -1; prevY = -1;
        }
        draw();
    }
}

void draw() {
    SDL_SetRenderDrawColor(render, 241, 221, 206, 255);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 97, 61, 61, 255);
    bool isBlack = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (isBlack) {
                if (highlightBoardMap[i][j]) {
                    SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
                }
                else {
                    SDL_SetRenderDrawColor(render, 97, 61, 61, 255);
                }
                isBlack = false;
            }
            else {
                if (highlightBoardMap[i][j]) {
                    SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
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
    
    for (int i = 0; i < 8; i++) {
        drawPiece(blackPawns[i].getImgName(), blackPawns[i].getCol(), blackPawns[i].getRow());
    }
    
    drawPiece(blackRook1.getImgName(), blackRook1.getCol(), blackRook1.getRow());
    drawPiece(blackKnight1.getImgName(), blackKnight1.getCol(), blackKnight1.getRow());
    drawPiece(blackBishop1.getImgName(), blackBishop1.getCol(), blackBishop1.getRow());
    drawPiece(blackQueen.getImgName(), blackQueen.getCol(), blackQueen.getRow());
    drawPiece(blackKing.getImgName(), blackKing.getCol(), blackKing.getRow());
    drawPiece(blackBishop2.getImgName(), blackBishop2.getCol(), blackBishop2.getRow());
    drawPiece(blackKnight2.getImgName(), blackKnight2.getCol(), blackKnight2.getRow());
    drawPiece(blackRook2.getImgName(), blackRook2.getCol(), blackRook2.getRow());
    
    for (int i = 0; i < 8; i++) {
        drawPiece(whitePawns[i].getImgName(), whitePawns[i].getCol(), whitePawns[i].getRow());
    }
    
    drawPiece(whiteRook1.getImgName(), whiteRook1.getCol(), whiteRook1.getRow());
    drawPiece(whiteKnight1.getImgName(), whiteKnight1.getCol(), whiteKnight1.getRow());
    drawPiece(whiteBishop1.getImgName(), whiteBishop1.getCol(), whiteBishop1.getRow());
    drawPiece(whiteQueen.getImgName(), whiteQueen.getCol(), whiteQueen.getRow());
    drawPiece(whiteKing.getImgName(), whiteKing.getCol(), whiteKing.getRow());
    drawPiece(whiteBishop2.getImgName(), whiteBishop2.getCol(), whiteBishop2.getRow());
    drawPiece(whiteKnight2.getImgName(), whiteKnight2.getCol(), whiteKnight2.getRow());
    drawPiece(whiteRook2.getImgName(), whiteRook2.getCol(), whiteRook2.getRow());
    
    SDL_RenderPresent(render);
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
                clicked(floor(event.motion.x / 50), floor(event.motion.y / 50));
            }
        }
    }
    
    close();
    
    return 0;
}

// Adding image to C++ projects: https://stackoverflow.com/questions/35093704/how-to-add-an-image-to-a-c-project-in-xcode
