#pragma once
#include <memory>
#include <vector>
#include "EventLoopThread.h"
#include "zrpc/base/logging.h"
namespace zrpc {
class EventLoopThreadPool {
 public:
  EventLoopThreadPool(EventLoop* baseLoop, int numThreads);

  ~EventLoopThreadPool() { InfoLog << "~EventLoopThreadPool()"; }
  void start();

  EventLoop* getNextLoop();

 private:
  EventLoop* baseLoop_;
  bool started_;
  int numThreads_;
  int next_;
  std::vector<std::shared_ptr<EventLoopThread>> threads_;
  std::vector<EventLoop*> loops_;
};
}
