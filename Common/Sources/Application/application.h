#pragma once
#include "common.h"
#include "Scene/scene.h"
#include "context.h"
#include "Event/input.h"
#include "Time/time.h"

class Application
{
private:
  inline static Application *application;
  Scene scene;
  Context context;
  Time timer;
  Input input;
public:
  const string projectPath, projectResourcesPath, projectShaderPath, commonResourcesPath, commonShaderPath;
  Application(string window_name, int width, int height, bool full_screen = false);
  bool sdl_event_handler();
  void main_loop();
  void exit();
  static Application& instance()
  {
    return *application;
  }
  static Scene& get_scene()
  {
    return application->scene;
  }
  static Context& get_context()
  {
    return application->context;
  }
  static const Time& get_timer()
  {
    return application->timer;
  }
  static Input& get_input()
  {
    return application->input;
  }
};
string project_resources_path(const string &path);
string common_resources_path(const string &path);