#pragma once
#include <list>
#include <algorithm>
#include <memory>
template<class ...Params>
class AbstractEventHandler
{
  using Type = AbstractEventHandler<Params...>;
public:
  virtual ~AbstractEventHandler() {}
  virtual void call( Params... params ) = 0;
  bool operator==(const Type &other) const
  {
    return is_equals(other);
  }
  bool operator!=(const Type &other) const
  {
    return !(*this == other);
  }
protected:    
  AbstractEventHandler() {}
  virtual bool is_equals(const Type &other) const = 0;
};


template<class ...Params>
class IEvent
{
  using HandlerPtr = std::shared_ptr<AbstractEventHandler<Params...>>;
public:
  bool operator+= (HandlerPtr ptr)
  {
    return add_handler(ptr);
  }
  bool operator-=(HandlerPtr ptr)
  {
    return remove_handler(ptr);
  }
protected:
  IEvent() {}
  virtual bool add_handler(HandlerPtr eventHandler) = 0;
  virtual bool remove_handler(HandlerPtr eventHandler) = 0;
};

template<class ...Params>
class Event : public IEvent<Params...>
{
  using HandlerPtr = std::shared_ptr<AbstractEventHandler<Params...>>;
  using EventHandlerIt = typename std::list<HandlerPtr>::const_iterator;
public:
  Event():
  handlers(),
  currentItIsRemoved(false),
  currentIt()
  { }
  ~Event()
  { }
  void operator()(Params... params)
  {
    currentItIsRemoved = false;
    currentIt = handlers.begin();
    while (currentIt != handlers.end())
    {
      (*currentIt)->call(params...);
      if (currentItIsRemoved)
      {
        currentItIsRemoved = false;
        EventHandlerIt removedIt = currentIt;
        ++currentIt;
        delete_handler(removedIt);
      }
      else
      {
        ++currentIt;
      }
      
    }
  }
protected:
  virtual bool add_handler(HandlerPtr eventHandler) override
  {
    bool found = find_event_handler(eventHandler) != handlers.end();
    if (!found)
    {
      handlers.emplace_back(eventHandler);
    }
    return !found;
  }
  virtual bool remove_handler(HandlerPtr eventHandler) override
  {
    auto it = find_event_handler(eventHandler);
    bool found = it != handlers.end();
    if (found)
    {
      if (it == currentIt)
        currentItIsRemoved = true;
      else
        delete_handler(it);
    }
    return found;
  }
private:
  inline EventHandlerIt find_event_handler(const HandlerPtr & eventHandler) const
  {
    return std::find_if(handlers.cbegin(), handlers.cend(),
    [&eventHandler](const HandlerPtr& handler)
    {return *handler == *eventHandler;} );
  }
  inline void delete_handler(EventHandlerIt it)
  {
    handlers.erase(it);
  }
  std::list<HandlerPtr> handlers;
  bool currentItIsRemoved = false;
  EventHandlerIt currentIt;
};

template<class Object, class ...Params>
class MethodEventHandler : public AbstractEventHandler<Params...>
{
  using Method = void(Object::*)(Params...);
  using Type = AbstractEventHandler<Params...>;
  using MyType = MethodEventHandler<Object, Params...>;
public:
  MethodEventHandler(Object& object, Method method):
    AbstractEventHandler<Params...>(),
    object(object),
    method(method)
  {
    assert(method != nullptr);
  }
  virtual void call(Params... params) override final
  {
    (object.*method)(params...);
  }
protected:
  virtual bool is_equals(const Type &other) const override
  {
    const MyType * _other = dynamic_cast<const MyType*>(&other);
    return _other != nullptr && &object == &_other->object && method == _other->method;
  }
private:
  Object& object;
  Method method;
};


template<class Object, class ...Params>
shared_ptr<AbstractEventHandler<Params...>> createMethodEventHandler(Object& object, void(Object::*method)(Params...))
{
  return shared_ptr<AbstractEventHandler<Params...>>(new MethodEventHandler<Object, Params...>(object, method));
}


