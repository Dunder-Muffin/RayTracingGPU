#include "scene.h"

#include "glad/glad.h"
#include "skybox.h"
#include "Camera/cameras.h"
#include "Time/time.h"



void Scene::init(void init_f(vector<GameObjectPtr>&, DirectionLight&))
{
  if (init_f != nullptr)
    init_f(gameObjects, sun);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}
void Scene::update()
{   
  for (auto objects : gameObjects)
  {
    for (auto component : objects->get_components())
    {
      IUpdatable *updatable = dynamic_cast<IUpdatable*>(component.get());
      if (updatable)
        updatable->update();
    }
  }
}
void Scene::render()
{
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  CameraPtr mainCamera = main_camera();
  for (auto objects : gameObjects)
  {
    for (auto component : objects->get_components())
    {
      IRenderable *renderable = dynamic_cast<IRenderable*>(component.get());
      if (renderable)
        renderable->render(*mainCamera, sun);////////////////////////////////////////
    }
  }

  glFlush(); 
}


void Scene::exit()
{ 
  for (auto objects : gameObjects)
  {
    for (auto component : objects->get_components())
    {
      ISaveable *saveable = dynamic_cast<ISaveable*>(component.get());
      if (saveable)
        saveable->save();
    }
  }
}
