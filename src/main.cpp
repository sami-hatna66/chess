#include "Board.hpp"
#include "Game.hpp"
#include "Piece.hpp"
#include "Square.hpp"
#include "Opponent.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <chrono>

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
          std::shared_ptr<Square> selectedSquare, std::shared_ptr<Game> game,
          bool buttonPressed, bool showPromotionMenu) {
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

    if (game->getOpponent() == opponents::computer && 
        game->getCurrentTurn() == pieceColor::black &&
        (game->getStatus() == gameStatus::inProgress ||
         game->getStatus() == gameStatus::blackCheck ||
         game->getStatus() == gameStatus::whiteCheck)) {
            drawText(renderer, "Thinking", 320, 417, {255, 255, 255});
         }

    if (game->getStatus() == gameStatus::blackCheck) {
        drawText(renderer, "Black in check", 5, 425, {255, 255, 255});
    } else if (game->getStatus() == gameStatus::whiteCheck) {
        drawText(renderer, "White in check", 5, 425, {255, 255, 255});
    }

    // Draw start/replay button
    if (game->getStatus() == gameStatus::blackCheckmate ||
        game->getStatus() == gameStatus::whiteCheckmate || 
        game->getStatus() == gameStatus::startScreen) {
            SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
            r.x = 110;
            r.y = 146;
            r.w = 180;
            r.h = 107;
            SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &r);

            drawText(renderer, "Choose Opponent:", 115, 151, {0, 0, 0});

            r.x = 120; r.y = 171; r.w = 10; r.h = 10;
            if (game->getOpponent() == opponents::player) {
                SDL_RenderFillRect(renderer, &r);
                r.y = 191;
                SDL_RenderDrawRect(renderer, &r);
            }
            else {
                SDL_RenderDrawRect(renderer, &r);
                r.y = 191;
                SDL_RenderFillRect(renderer, &r);
            }

            drawText(renderer, "Player", 135, 169, {0, 0, 0});
            drawText(renderer, "Computer", 135, 189, {0, 0, 0});

            if (buttonPressed) {
                SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
            }
            r.x = 160;
            r.y = 212;
            r.w = 80;
            r.h = 34;
            SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &r);

            if (game->getStatus() == gameStatus::startScreen) {
                drawText(renderer, "START", 167, 215, {0, 0, 0}, 30, 65);
            } else {
                drawText(renderer, "REPLAY", 167, 215, {0, 0, 0}, 30, 65);
                drawText(renderer, game->getStatus() == gameStatus::blackCheckmate ? "BLACK CHECKMATE" : "WHITE CHECKMATE", 
                    255, 402, {255, 0, 0}, 50);
            }
    }
    // Draw undo move button
    else if (!(game->getOpponent() == opponents::computer && 
               game->getCurrentTurn() == pieceColor::black &&
               (game->getStatus() == gameStatus::inProgress ||
                game->getStatus() == gameStatus::blackCheck ||
                game->getStatus() == gameStatus::whiteCheck))) {
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

    std::shared_ptr<Game> game = std::make_shared<Game>(Game());

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

    bool buttonPressed = false;

    draw(renderer, game->getBoard(), selectedSquare, game, buttonPressed,
         game->getStatus() == gameStatus::choosingPromotion);

    while (!isQuit) {
        // If playing computer, call method to determine computer move
        if ((game->getStatus() == gameStatus::inProgress ||
             game->getStatus() == gameStatus::blackCheck ||
             game->getStatus() == gameStatus::whiteCheck) && 
            game->getOpponent() == opponents::computer && 
            game->getCurrentTurn() == pieceColor::black) {
                auto start = std::chrono::high_resolution_clock::now();
                opponentTurn(game);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                std::cout << duration.count() << std::endl;
                
                game->setCurrentTurn(pieceColor::white);
                draw(renderer, game->getBoard(), selectedSquare, game,
                     buttonPressed,
                     game->getStatus() == gameStatus::choosingPromotion);
            }
        else if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isQuit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int clickedRow = 7 - int(event.button.y / 50);
                int clickedCol = int(event.button.x / 50);

                // Click on start/replay button
                if ((game->getStatus() == gameStatus::blackCheckmate ||
                     game->getStatus() == gameStatus::whiteCheckmate ||
                     game->getStatus() == gameStatus::startScreen) &&
                    (event.button.x >= 160 && event.button.x <= 240 &&
                     event.button.y >= 212 && event.button.y <= 246)) {
                        buttonPressed = true;
                }
                // Click on opponent selection
                else if ((game->getStatus() == gameStatus::blackCheckmate ||
                          game->getStatus() == gameStatus::whiteCheckmate ||
                          game->getStatus() == gameStatus::startScreen) &&
                         (event.button.x >= 120 && event.button.x <= 190 &&
                          event.button.y >= 171 && event.button.y <= 181)) {
                            game->setOpponent(opponents::player);
                } else if ((game->getStatus() == gameStatus::blackCheckmate ||
                            game->getStatus() == gameStatus::whiteCheckmate ||
                            game->getStatus() == gameStatus::startScreen) &&
                           (event.button.x >= 120 && event.button.x <= 210 &&
                            event.button.y >= 191 && event.button.y <= 201)) {
                            game->setOpponent(opponents::computer);
                }
                // Click on promotion menu
                else if (game->getStatus() == gameStatus::choosingPromotion) {
                    if (event.button.y >= 174 && event.button.y <= 226) {
                        if (event.button.x >= 96 && event.button.x <= 148) {
                            game->promote(pieceType::Queen);
                        } else if (event.button.x >= 148 &&
                                   event.button.x <= 200) {
                            game->promote(pieceType::Rook);
                        } else if (event.button.x >= 200 &&
                                   event.button.x <= 252) {
                            game->promote(pieceType::Bishop);
                        } else if (event.button.x >= 252 &&
                                   event.button.x <= 304) {
                            game->promote(pieceType::Knight);
                        }
                    }
                }
                // Click on undo move button 
                else if (event.button.x >= 344 && event.button.x <= 388 &&
                           event.button.y >= 415 && event.button.y <= 434) {
                    game->undoMove();
                    if (game->getOpponent() == opponents::computer) {
                        game->undoMove();
                    }
                } 
                // Click on chess board
                else if (clickedCol >= 0 && clickedCol <= 7 &&
                           clickedRow >= 0 && clickedRow <= 7) {
                    auto clickedSquare =
                        game->getBoard()->getSquare(clickedRow, clickedCol);
                    // Deselect square if clicked for second time
                    if (clickedSquare == selectedSquare) {
                        selectedSquare = nullptr;
                    }
                    // Select square if square isn't already selected,
                    // square contains piece and piece is of correct colour
                    else if (selectedSquare == nullptr &&
                             clickedSquare->getPiece() != nullptr &&
                             clickedSquare->getPiece()->getColor() ==
                                 game->getCurrentTurn()) {
                        selectedSquare = clickedSquare;
                    }
                    // If a square has been selected, clicked square is end
                    // square of move
                    else if (selectedSquare != nullptr &&
                             game->turn(selectedSquare, clickedSquare)) {
                        selectedSquare = nullptr;
                    }
                }

                draw(renderer, game->getBoard(), selectedSquare, game,
                     buttonPressed,
                     game->getStatus() == gameStatus::choosingPromotion);
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                // Start new game if start/replay button is pressed
                if (buttonPressed) {
                    buttonPressed = false;
                    draw(renderer, game->getBoard(), selectedSquare, game,
                         buttonPressed,
                         game->getStatus() == gameStatus::choosingPromotion);
                    game->resetGame();
                    draw(renderer, game->getBoard(), selectedSquare, game,
                         buttonPressed,
                         game->getStatus() == gameStatus::choosingPromotion);
                }
            }
        }
    }

    close(win, renderer);

    return 0;
}
