#pragma once
#include <vector>
#include <memory>
#include "../component.h"
class GameObject
{
private:
  std::vector<ComponentPtr> components;
  template<typename T>
  int find_component()
  {
    for (uint i = 0; i < components.size(); ++i)
    {
      T * t = dynamic_cast<T*>(components[i].get());
      if (t != nullptr)
        return i;
    }
    return -1;
  }
public:
  const std::vector<ComponentPtr> &get_components() const;

  template<typename T>
  T* get_component()
  {
    static_assert(std::is_base_of_v<Component, T>);
    int i = find_component<T>();
    return i < 0 ? nullptr : dynamic_cast<T*>(components[i].get());
  }
  template<typename T, typename... Args>
  std::shared_ptr<T> add_component(Args... args)
  {
    static_assert(std::is_base_of_v<Component, T>);

    std::shared_ptr<T> t = std::make_shared<T>(args...);
    t->gameObject = this;
    int i = find_component<T>();
    if (i < 0)
    {
      components.push_back(t);
    }
    else
    {
      components[i] = t;
    }    
    return t;
  }
  template<typename T>
  std::shared_ptr<T> add_component(std::shared_ptr<T> ptr)
  {
    static_assert(std::is_base_of_v<Component, T>);
    int i = find_component<T>();
    if (ptr)
    {
      ptr->gameObject = this;
      if (i < 0)
      {
        components.push_back(ptr);
      }
      else
      {
        components[i] = ptr;
      }   
    } 
    return ptr;
  }
};

using GameObjectPtr = std::shared_ptr<GameObject>;
GameObjectPtr make_game_object();
