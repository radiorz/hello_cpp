#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <algorithm>

class EventBus
{
public:
  template <typename T, typename... Args>
  using EventCallback = std::function<void(T arg, Args... args)>;

  // 注册事件监听器
  template <typename T, typename... Args>
  void on(const std::string &eventName, const EventCallback<T, Args...> &callback)
  {
    eventListeners[eventName].push_back(callback);
  }
  // 只监听一次
  template <typename T, typename... Args>
  void once(const std::string &eventName, const EventCallback<T, Args...> &callback)
  {
    EventCallback<T, Args...> wrappedCallback = [this, eventName, callback](T arg, Args... args)
    {
      off(eventName, callback); // 接收到消息后取消监听
      callback(arg, args...);
    };
    eventListeners[eventName].push_back(wrappedCallback);
  }

  // 移除事件监听器
  template <typename T, typename... Args>
  void off(const std::string &eventName, const EventCallback<T, Args...> &callback)
  {
    auto it = eventListeners.find(eventName);
    if (it != eventListeners.end())
    {
      std::vector<EventCallback> &listeners = it->second;
      listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
                                     [&callback](const EventCallback &cb)
                                     { return cb.target<void()>() == callback.target<void()>(); }),
                      listeners.end());
    }
  }
  // 触发事件
  template <typename T, typename... Args>
  void emit(const std::string &eventName)
  {
    auto it = eventListeners.find(eventName);
    if (it != eventListeners.end())
    {
      const std::vector<EventCallback> &listeners = it->second;
      for (const EventCallback &callback : listeners)
      {
        callback();
      }
    }
  }

private:
  template <typename T, typename... Args>
  std::map<std::string, std::vector<EventCallback<T, Args...>>> eventListeners;
};

// 示例用法
int main()
{
  EventBus eventBus;
  EventBus::EventCallback<void> event1Callback = []()
  { std::cout << "Event 1 triggered." << std::endl; };
  // 注册事件监听器
  eventBus.on("event1", event1Callback);

  eventBus.on("event2", []()
              { std::cout << "Event 2 triggered." << std::endl; });
  eventBus.once("event3", []()
                { std::cout << "Event 3 triggered." << std::endl; });
  // 触发事件
  eventBus.emit("event1");
  eventBus.emit("event2");
  eventBus.emit("event3");
  eventBus.emit("event3");

  // 移除事件监听器

  eventBus.off("event1", event1Callback);

  eventBus.emit("event1"); // 不会触发已移除的事件监听器
  eventBus.emit("event2");
  return 0;
}
