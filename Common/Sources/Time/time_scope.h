#pragma once
#include <SDL2/SDL_timer.h>
#include "common.h"
class TimeScope
{
private:
  Uint32 start;
  const string message;
  bool stopped;
public:
  TimeScope(const string &message):
    start(SDL_GetTicks()), message(message), stopped(false)
  { }
  void stop()
  {
    Uint32 delta = SDL_GetTicks() - start;
    debug_log("%s\nspend %f seconds", message.c_str(), delta * 0.001f);
    stopped = true;
  }
  ~TimeScope()
  {
    if (!stopped)
      stop();
  }
};