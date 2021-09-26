#include "application.h"
#include "Shader/shader_factory.h"

#include "config.h"

Application::Application(string window_name, int width, int height, bool full_screen):
scene(), context(window_name, width, height, full_screen), timer(), input(),
projectPath(string(get_config("projectPath"))),
projectResourcesPath(string(get_config("projectPath")) + "/Resources"),
projectShaderPath(string(get_config("projectPath")) + "/Shaders"),
commonResourcesPath(string(get_config("commonPath")) + "/Resources"),
commonShaderPath(string(get_config("commonPath")) + "/Shaders")
{
  application = this;
  compile_shaders(); 
}
bool Application::sdl_event_handler()
{
  SDL_Event event;
  bool running = true;
  while (SDL_PollEvent(&event))
  {
    
    switch(event.type){
      case SDL_QUIT: running = false; break;
      
      case SDL_KEYDOWN: 
      case SDL_KEYUP: input.event_process(event.key);

      if(event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
      break;

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: input.event_process(event.button); break;

      case SDL_MOUSEMOTION: input.event_process(event.motion); break;

      case SDL_MOUSEWHEEL: input.event_process(event.wheel); break;
      case SDL_WINDOWEVENT: break;
    }
  }
  return running;
}
void Application::main_loop()
{
  bool running = true;
  while(running){
    timer.update();
    
    running = sdl_event_handler();
    if (running)
    {
      scene.update();
      scene.render();

      context.swap_buffer();
    }
  }
}
void Application::exit()
{
  scene.exit();

  SDL_Quit();
}
string project_resources_path(const string &path)
{
  return Application::instance().projectResourcesPath + "/" + path;
}
string common_resources_path(const string &path)
{
  return Application::instance().commonResourcesPath + "/" + path;
}