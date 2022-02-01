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
using namespace std;

void init();
void draw();
void close();

SDL_Window* win = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* render = NULL;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    win = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, SDL_WINDOW_SHOWN);
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

    SDL_Surface * image = IMG_Load("blkQueen.png");
    SDL_Texture * texture = SDL_CreateTextureFromSurface(render, image);
    SDL_Rect pieceRect; pieceRect.x = 50; pieceRect.y = 50; pieceRect.w = 50; pieceRect.h = 50;
    SDL_RenderCopy(render, texture, NULL, &pieceRect);
    
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
        }
    }
    
    close();
    
    return 0;
}

// Adding image to C++ projects: https://stackoverflow.com/questions/35093704/how-to-add-an-image-to-a-c-project-in-xcode
