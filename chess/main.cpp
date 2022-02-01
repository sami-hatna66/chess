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

SDL_Window* win = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* render = NULL;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    win = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, SDL_WINDOW_SHOWN);
}

vector<Pawn> whitePawns = {
    Pawn(1, 0, false), Pawn(1, 1, false),
    Pawn(1, 2, false), Pawn(1, 3, false),
    Pawn(1, 4, false), Pawn(1, 5, false),
    Pawn(1, 6, false), Pawn(1, 7, false),
};

Rook whiteRook1 = Rook(0, 0, false);
Rook whiteRook2 = Rook(0, 7, false);
Knight whiteKnight1 = Knight(0, 1, false);
Knight whiteKnight2 = Knight(0, 6, false);
Bishop whiteBishop1 = Bishop(0, 2, false);
Bishop whiteBishop2 = Bishop(0, 5, false);
Queen whiteQueen = Queen(0, 3, false);
King whiteKing = King(0, 4, false);

vector<Pawn> blackPawns = {
    Pawn(6, 0, true), Pawn(6, 1, true),
    Pawn(6, 2, true), Pawn(6, 3, true),
    Pawn(6, 4, true), Pawn(6, 5, true),
    Pawn(6, 6, true), Pawn(6, 7, true),
};

Rook blackRook1 = Rook(7, 0, true);
Rook blackRook2 = Rook(7, 7, true);
Knight blackKnight1 = Knight(7, 1, true);
Knight blackKnight2 = Knight(7, 6, true);
Bishop blackBishop1 = Bishop(7, 2, true);
Bishop blackBishop2 = Bishop(7, 5, true);
Queen blackQueen = Queen(7, 4, true);
King blackKing = King(7, 3, true);

void drawPiece(const char* imgName, int xPos, int yPos) {
    SDL_Surface * image = IMG_Load(imgName);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(render, image);
    SDL_Rect piecRect; piecRect.x = 50 * xPos; piecRect.y = 50 * yPos; piecRect.w = 50; piecRect.h = 50;
    SDL_RenderCopy(render, texture, NULL, &piecRect);
}

void draw() {
    SDL_SetRenderDrawColor(render, 241, 221, 206, 255);
    SDL_RenderClear(render);
    
    SDL_SetRenderDrawColor(render, 97, 61, 61, 255);
    bool isBlack = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (isBlack) {
                SDL_Rect r;
                r.x = j * 50; r.y = i * 50; r.w = 50; r.h = 50;
                SDL_RenderFillRect(render, &r);
                isBlack = false;
            }
            else {
                isBlack = true;
            }
        }
        isBlack = isBlack ? false : true;
    }

//    SDL_Surface * image = IMG_Load("blkQueen.png");
//    SDL_Texture * texture = SDL_CreateTextureFromSurface(render, image);
//    SDL_Rect pieceRect; pieceRect.x = 50; pieceRect.y = 50; pieceRect.w = 50; pieceRect.h = 50;
//    SDL_RenderCopy(render, texture, NULL, &pieceRect);
    
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
                cout << "(" << floor(event.motion.x / 50) << ", " << floor(event.motion.y / 50) << ")" << endl;
            }
        }
    }
    
    close();
    
    return 0;
}

// Adding image to C++ projects: https://stackoverflow.com/questions/35093704/how-to-add-an-image-to-a-c-project-in-xcode
