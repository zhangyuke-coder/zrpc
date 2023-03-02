#pragma once
#include "EventLoop.h"
#include "zrpc/base/Condition.h"
#include "zrpc/base/mutex.h"
#include "zrpc/base/Thread.h"
#include <thread>

namespace zrpc {
class EventLoopThread {
 public:
  EventLoopThread();
  ~EventLoopThread();
  EventLoop* startLoop();

 private:
  void threadFunc();
  EventLoop* loop_;
  bool exiting_;
  Thread thread_;
  Mutex mutex_;
  Condition cond_;
};
}
