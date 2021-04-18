#include <iostream>
#include "Application/application.h"
#include "Application/config.h"
#include "ray_tracing_scene.h"

int main(int argc, char** argv)
{
	int width = 512; 
	int height = width;

/*	parse args and determine build, sources path. cfg source: /Common/Sources/Application */ 
  add_configs(argc, (const char**)argv);
/*	constructor compiles all shaders, creates SDL window and inits ImGui */
  Application application(get_config("project"), width, height,true);
/*  gives scene function Projects/RayTracing/Sources
  to general scene handler Common/Sources/Scene/
  which sets camera and handles events */
  application.get_scene().init(init_scene);

  application.main_loop();
/*	quit SDL, ImGui and Scene */
  application.exit();
  
  return 0;
}