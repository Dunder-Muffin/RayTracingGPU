#pragma once
#include "common.h"
#include "GameObject/game_object.h"
#include "Light/direction_light.h"
#include <vector>
class Scene
{
private:
  vector<GameObjectPtr> gameObjects;
  DirectionLight sun;
public:
  void init(void (vector<GameObjectPtr>&, DirectionLight&));
  void update();
  void render();
  void render_ui();
  void exit();

};
