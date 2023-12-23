#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <algorithm> // 添加此头文件

class EventBus
{
public:
  using EventCallback = std::function<void()>;

  // 注册事件监听器
  void on(const std::string &eventName, const EventCallback &callback)
  {
    eventListeners[eventName].push_back(callback);
  }

  // 移除事件监听器
  void off(const std::string &eventName, const EventCallback &callback)
  {
    auto it = eventListeners.find(eventName);
    if (it != eventListeners.end())
    {
      std::vector<EventCallback> &listeners = it->second;
      listeners.erase(std::remove(listeners.begin(), listeners.end(), callback), listeners.end());
    }
  }

  // 触发事件
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
  std::map<std::string, std::vector<EventCallback>> eventListeners;
};

// 示例用法
int main()
{
  EventBus eventBus;

  // 注册事件监听器
  eventBus.on("event1", []()
              { std::cout << "Event 1 triggered." << std::endl; });

  eventBus.on("event2", []()
              { std::cout << "Event 2 triggered." << std::endl; });

  // 触发事件
  eventBus.emit("event1");
  eventBus.emit("event2");

  // 移除事件监听器
  EventCallback event1Callback = []()
  {
    std::cout << "Event 1 listener removed." << std::endl;
  };
  eventBus.off("event1", event1Callback);

  eventBus.emit("event1"); // 不会触发已移除的事件监听器

  return 0;
}
