#include "sdl_render.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <chrono>
#include <thread>
#include <iostream>

Notificationwindow::Notificationwindow(const std::string &title,
                                       const std::string &body, int timeout_ms)
    : title_(title), body_(body), timeout_ms_(timeout_ms) {}

Notificationwindow::~Notificationwindow() {
  TTF_Quit();
  SDL_Quit();
}


void Notificationwindow::show(){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    //  Draw the window and render it
    SDL_Window* window = SDL_CreateWindow("Notification", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 100, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);

    if(!window){
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!renderer){
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Choose the font
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSans.ttf",16);
    if(!font){
        SDL_Log("Failed to load font: %s", TTF_GetError());
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    // Colors in RGBA
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};

    SDL_Surface* title_surf = TTF_RenderText_Blended(font, title_.c_str(), white);
    SDL_Surface* body_surf = TTF_RenderText_Blended(font, body_.c_str(), white);
    SDL_Texture* title_tex = SDL_CreateTextureFromSurface(renderer, title_surf);
    SDL_Texture* body_tex = SDL_CreateTextureFromSurface(renderer, body_surf);


    SDL_FreeSurface(title_surf);
    SDL_FreeSurface(body_surf);

    // Set Background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int tw, th, bw, bh; // title and body width and height
    SDL_QueryTexture(title_tex, nullptr, nullptr, &tw, &th);
    SDL_QueryTexture(body_tex, nullptr, nullptr, &bw, &bh);

    SDL_Rect title_rect = {10, 10, tw, th};
    SDL_Rect body_rect = {10, 30, bw, bh};

    SDL_RenderCopy(renderer, title_tex, nullptr, &title_rect);
    SDL_RenderCopy(renderer,body_tex, nullptr,&body_rect);

    // Display on to
    // screen
    SDL_RenderPresent(renderer);

    SDL_Delay(timeout_ms_);
    // std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms_));

    std::cout << "Done Loaded " << std:: endl;

    // Clean Up
    SDL_DestroyTexture(title_tex);
    SDL_DestroyTexture(body_tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
