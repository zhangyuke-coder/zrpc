#pragma once
#include "Condition.h"
#include "mutex.h"


// CountDownLatch的主要作用是确保Thread中传进去的func真的启动了以后
// 外层的start才返回
namespace zrpc {
class CountDownLatch  {
 public:
  explicit CountDownLatch(int count);
  void wait();
  void countDown();

 private:
  mutable Mutex mutex_;
  Condition condition_;
  int count_;
};
}