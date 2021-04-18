#pragma once 
#include <SDL2/SDL.h>
#include "common.h"
class Context
{
private:
  SDL_Window * window;
  SDL_GLContext gl_context;
  int width, height;
public:
  Context(string window_name, int width, int height, bool full_screen = false);
  void start_imgui();
  void swap_buffer();
  int get_width() const;
  int get_height() const;
};