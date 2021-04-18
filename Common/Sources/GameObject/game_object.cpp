
#include <map>
#include "game_object.h"

const std::vector<ComponentPtr> &GameObject::get_components() const
{
  return components;
}
GameObjectPtr make_game_object()
{
  return std::make_shared<GameObject>();
}

