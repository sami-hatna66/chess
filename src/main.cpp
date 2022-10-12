#include "Board.hpp"
#include "Game.hpp"
#include "Piece.hpp"
#include "Square.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

void close(SDL_Window *win, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void drawPiece(SDL_Renderer *renderer, const char *imgName, int row, int col) {
    SDL_Surface *image = IMG_Load(imgName);
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_Rect pieceRect;
    pieceRect.x = col;
    pieceRect.y = row;
    pieceRect.w = 50;
    pieceRect.h = 50;
    SDL_RenderCopy(renderer, imageTexture, NULL, &pieceRect);
    SDL_DestroyTexture(imageTexture);
}

void drawText(SDL_Renderer *renderer, const char *text, int x, int y,
              std::array<int, 3> color, int height = 0, int width = 0) {
    SDL_Color textColor = {static_cast<Uint8>(color[0]),
                           static_cast<Uint8>(color[1]),
                           static_cast<Uint8>(color[2])};
    TTF_Font *font = TTF_OpenFont("assets/chess.ttf", 12);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = width != 0 ? width : textSurface->w;
    r.h = height != 0 ? height : textSurface->h;
    SDL_RenderCopy(renderer, textTexture, NULL, &r);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

void draw(SDL_Renderer *renderer, std::shared_ptr<Board> board,
          std::shared_ptr<Square> selectedSquare, Game *game,
          bool replayButtonPressed, bool showPromotionMenu) {
    SDL_SetRenderDrawColor(renderer, 241, 221, 206, 255);
    SDL_RenderClear(renderer);

    // Draw squares
    SDL_Rect r;
    bool isBlackSquare = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board->getSquare(7 - i, j) == selectedSquare) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            } else if (isBlackSquare) {
                SDL_SetRenderDrawColor(renderer, 97, 61, 61, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 241, 221, 206, 255);
            }
            isBlackSquare = !isBlackSquare;

            r.x = j * 50;
            r.y = i * 50;
            r.w = 50;
            r.h = 50;
            SDL_RenderFillRect(renderer, &r);
        }
        isBlackSquare = !isBlackSquare;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    r.x = 0;
    r.y = 400;
    r.w = 400, r.h = 50;
    SDL_RenderFillRect(renderer, &r);

    // Draw pieces
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto currentPiece = board->getSquare(i, j)->getPiece();
            if (currentPiece != nullptr) {
                drawPiece(renderer, currentPiece->getImageName(), 50 * (7 - i),
                          50 * j);
            }
        }
    }

    drawText(renderer,
             game->getCurrentTurn() == pieceColor::black ? "Black's turn"
                                                         : "White's turn",
             5, 405, {255, 255, 255});

    if (game->getStatus() == gameStatus::blackCheck) {
        drawText(renderer, "Black in check", 5, 425, {255, 255, 255});
    } else if (game->getStatus() == gameStatus::whiteCheck) {
        drawText(renderer, "White in check", 5, 425, {255, 255, 255});
    }

    if (game->getStatus() == gameStatus::blackCheckmate ||
        game->getStatus() == gameStatus::whiteCheckmate) {
        drawText(renderer,
                 game->getStatus() == gameStatus::blackCheckmate
                     ? "BLACK CHECKMATE"
                     : "WHITE CHECKMATE",
                 255, 402, {255, 0, 0}, 50);

        // Draw replay button
        if (replayButtonPressed) {
            SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
        }
        r.x = 145;
        r.y = 178;
        r.w = 110;
        r.h = 44;
        SDL_RenderFillRect(renderer, &r);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &r);

        drawText(renderer, "REPLAY", 160, 181, {0, 0, 0}, 40, 80);
    }
    // Draw undo move button
    else {
        SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
        r.x = 344;
        r.y = 415;
        r.w = 44;
        r.h = 19;
        SDL_RenderFillRect(renderer, &r);

        drawText(renderer, "UNDO", 346, 417, {0, 0, 0});
    }

    // Draw promotion menu
    if (showPromotionMenu) {
        SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
        r.x = 96;
        r.y = 174;
        r.w = 208;
        r.h = 52;
        SDL_RenderFillRect(renderer, &r);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        drawPiece(
            renderer,
            (std::string("assets/") +
             (game->getCurrentTurn() == pieceColor::white ? "wht" : "blk") +
             "Queen.png")
                .c_str(),
            175, 97);
        drawPiece(
            renderer,
            (std::string("assets/") +
             (game->getCurrentTurn() == pieceColor::white ? "wht" : "blk") +
             "Rook.png")
                .c_str(),
            175, 149);
        drawPiece(
            renderer,
            (std::string("assets/") +
             (game->getCurrentTurn() == pieceColor::white ? "wht" : "blk") +
             "Bishop.png")
                .c_str(),
            175, 201);
        drawPiece(
            renderer,
            (std::string("assets/") +
             (game->getCurrentTurn() == pieceColor::white ? "wht" : "blk") +
             "Knight.png")
                .c_str(),
            175, 253);

        r.x = 96;
        r.y = 174;
        r.w = 208, r.h = 52;
        SDL_RenderDrawRect(renderer, &r);
        SDL_RenderDrawLine(renderer, 148, 174, 148, 226);
        SDL_RenderDrawLine(renderer, 200, 174, 200, 226);
        SDL_RenderDrawLine(renderer, 252, 174, 252, 226);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    int width = 400;
    int height = 450;

    auto game = Game();

    std::shared_ptr<Square> selectedSquare = nullptr;

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window *win = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED, width, height,
                                       SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *iconSurface = IMG_Load("assets/appIcon.png");
    SDL_SetWindowIcon(win, iconSurface);

    SDL_Event event;
    bool isQuit = false;

    bool replayButtonPressed = false;

    draw(renderer, game.getBoard(), selectedSquare, &game, replayButtonPressed,
         game.getStatus() == gameStatus::choosingPromotion);

    while (!isQuit) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isQuit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int clickedRow = 7 - int(event.button.y / 50);
                int clickedCol = int(event.button.x / 50);

                // Cease gameplay and render replay button if in checkmate
                if (game.getStatus() == gameStatus::blackCheckmate ||
                    game.getStatus() == gameStatus::whiteCheckmate) {
                    if (event.button.x >= 145 && event.button.x <= 255 &&
                        event.button.y >= 178 && event.button.y <= 222) {
                        replayButtonPressed = true;
                    }
                    // Click on promotion menu
                } else if (game.getStatus() == gameStatus::choosingPromotion) {
                    if (event.button.y >= 174 && event.button.y <= 226) {
                        if (event.button.x >= 96 && event.button.x <= 148) {
                            game.promote(pieceType::Queen);
                        } else if (event.button.x >= 148 &&
                                   event.button.x <= 200) {
                            game.promote(pieceType::Rook);
                        } else if (event.button.x >= 200 &&
                                   event.button.x <= 252) {
                            game.promote(pieceType::Bishop);
                        } else if (event.button.x >= 252 &&
                                   event.button.x <= 304) {
                            game.promote(pieceType::Knight);
                        }
                    }
                    // Click on undo move button
                } else if (event.button.x >= 344 && event.button.x <= 388 &&
                           event.button.y >= 415 && event.button.y <= 434) {
                    game.undoMove();
                    // Click on chess board
                } else if (clickedCol >= 0 && clickedCol <= 7 &&
                           clickedRow >= 0 && clickedRow <= 7) {
                    auto clickedSquare =
                        game.getBoard()->getSquare(clickedRow, clickedCol);
                    // Deselect square if clicked for second time
                    if (clickedSquare == selectedSquare) {
                        selectedSquare = nullptr;
                    }
                    // Select square if square isn't already selected,
                    // square contains piece and piece is of correct colour
                    else if (selectedSquare == nullptr &&
                             clickedSquare->getPiece() != nullptr &&
                             clickedSquare->getPiece()->getColor() ==
                                 game.getCurrentTurn()) {
                        selectedSquare = clickedSquare;
                    }
                    // If a square has been selected, clicked square is end
                    // square of move
                    else if (selectedSquare != nullptr &&
                             game.turn(selectedSquare, clickedSquare)) {
                        selectedSquare = nullptr;
                    }
                }

                draw(renderer, game.getBoard(), selectedSquare, &game,
                     replayButtonPressed,
                     game.getStatus() == gameStatus::choosingPromotion);
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                // Start new game if replay button is pressed
                if (replayButtonPressed) {
                    replayButtonPressed = false;
                    draw(renderer, game.getBoard(), selectedSquare, &game,
                         replayButtonPressed,
                         game.getStatus() == gameStatus::choosingPromotion);
                    game.resetGame();
                    draw(renderer, game.getBoard(), selectedSquare, &game,
                         replayButtonPressed,
                         game.getStatus() == gameStatus::choosingPromotion);
                }
            }
        }
    }

    close(win, renderer);

    return 0;
}
