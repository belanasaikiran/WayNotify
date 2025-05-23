#include "sdl_render.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <chrono>
#include <thread>

Notificationwindow::Notificationwindow(const std::string &title,
                                       const std::string &body, const std::string& icon, int timeout_ms)
    : title_(title), body_(body), icon_(icon), timeout_ms_(timeout_ms) {}

Notificationwindow::~Notificationwindow() {
  TTF_Quit();
  SDL_Quit();
}

std::string Notificationwindow::iconSanitizer(const std::string& icon_path){
    if(icon_path.substr(0,7) == "file://"){
        return icon_path.substr(7);
    }
    return icon_path;
}


void Notificationwindow::show(){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    //  Draw the window and render it
    SDL_Window* window = SDL_CreateWindow("Notification", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 100, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE);

    if(!window){
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_SetWindowInputFocus(window);


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

    // Load the icon
    std::string icon_path = iconSanitizer(icon_); // We need to truncate the text `file://` since dbus generates and uses this for the icon.
    std::cout << "ICON PATH: " << icon_path << std::endl;
    SDL_Surface* icon_surface = IMG_Load(icon_path.c_str());

    if(!icon_surface){
        std::cerr << "Failed to load icon: " << IMG_GetError() << std::endl;
        std::cerr << "Should Load Icon from: " << icon_.c_str() << std::endl;
    }

    SDL_Texture* icon_texture = SDL_CreateTextureFromSurface(renderer, icon_surface);
    if (!icon_texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_FreeSurface(icon_surface);

    // rendering the icon
    int icon_width, icon_height;
    SDL_QueryTexture(icon_texture, nullptr, nullptr, &icon_width, &icon_height);

    // Scale the Image ICon
    int max_width = 48;
    int max_height = 48;

    float aspect_ratio = static_cast<float>(icon_width) / static_cast<float>(icon_height);

    if(icon_width > max_width) {
        icon_width = max_width;
        icon_height = static_cast<int> (max_width / aspect_ratio);
    }
    if(icon_height > max_height){
        icon_height = max_height;
        icon_width = static_cast<int>(max_height * aspect_ratio);
    }



    // current position - top left
    SDL_Rect icon_rect = {10, 10, icon_width, icon_height};

    // Colors in RGBA
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};

    int text_wrap_width = 280;



    // Dismiss Button and place it at top right corner
    int button_width = 30, button_height = 30;
    SDL_Rect button_rect = {300 - button_width - 10, 10, button_width, button_height};
    SDL_Color button_color = {120, 20, 120, 255};

    SDL_SetRenderDrawColor(renderer, button_color.r, button_color.g, button_color.b, button_color.a);
    SDL_RenderFillRect(renderer, &button_rect);



    // Title
    SDL_Surface* title_surf = TTF_RenderText_Blended_Wrapped(font, title_.c_str(), white, text_wrap_width);
    SDL_Texture* title_tex = SDL_CreateTextureFromSurface(renderer, title_surf);

    // Body
    SDL_Surface* body_surf = TTF_RenderText_Blended_Wrapped(font, body_.c_str(), white, text_wrap_width);
    SDL_Texture* body_tex = SDL_CreateTextureFromSurface(renderer, body_surf);


    SDL_FreeSurface(title_surf);
    SDL_FreeSurface(body_surf);

    // Set Background
    SDL_SetRenderDrawColor(renderer, 10, 10, 70, 255);
    SDL_RenderClear(renderer);

    int tw, th, bw, bh; // title and body width and height
    SDL_QueryTexture(title_tex, nullptr, nullptr, &tw, &th);
    SDL_QueryTexture(body_tex, nullptr, nullptr, &bw, &bh);

    // Set text positions relative to the icon
    int icon_x = icon_rect.x + icon_rect.w + 10;
    int title_y = 10;
    int body_y = 30;

    // Set the title and body text positions
    SDL_Rect title_rect = {icon_x, title_y, tw, th};
    SDL_Rect body_rect = {icon_x, body_y, bw, bh};

    // Now draw the button (after clearing the screen)
    SDL_SetRenderDrawColor(renderer, button_color.r, button_color.g, button_color.b, button_color.a);
    SDL_RenderFillRect(renderer, &button_rect);

    SDL_RenderCopy(renderer, icon_texture, nullptr, &icon_rect);
    SDL_RenderCopy(renderer, title_tex, nullptr, &title_rect);
    SDL_RenderCopy(renderer,body_tex, nullptr,&body_rect);
    // Display on to screen
    SDL_RenderPresent(renderer);

     // Persistent event loop
    bool running = true;
    Uint32 start_time = SDL_GetTicks();
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                // check if click is inside the button rectangle
                if(x >= button_rect.x && x  <= button_rect.x + button_rect.w &&
                    y >= button_rect.y && y <= button_rect.y + button_rect.h){
                        running = false;
                }
            }
        }
        // Check for timeout
        if (SDL_GetTicks() - start_time > static_cast<Uint32>(timeout_ms_)) {
            running = false;
        }
        // SDL_Delay(10); // Small delay to avoid busy loop
    }


    // SDL_Delay(timeout_ms_);
    // std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms_));

    std::cout << "Destroyed " << std:: endl;

    // Clean Up
    SDL_DestroyTexture(icon_texture);
    SDL_DestroyTexture(title_tex);
    SDL_DestroyTexture(body_tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
