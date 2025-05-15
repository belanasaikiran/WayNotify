#pragma once
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

class Notificationwindow {
public:
  Notificationwindow(const std::string &title, const std::string &body, const std::string &icon,
                     int timeout_ms);
  ~Notificationwindow();
  void show();
  std::string iconSanitizer(const std::string& icon_path);

private:
  std::string title_;
  std::string body_;
  std::string icon_;
  int timeout_ms_;
};
