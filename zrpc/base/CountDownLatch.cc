#include "CountDownLatch.h"
namespace zrpc {
CountDownLatch::CountDownLatch(int count)
    : mutex_(), condition_(mutex_), count_(count) {}

void CountDownLatch::wait() {
  Mutex::Lock lock(mutex_);
  while (count_ > 0) condition_.wait();
}

void CountDownLatch::countDown() {
  Mutex::Lock lock(mutex_);
  --count_;
  if (count_ == 0) condition_.notifyAll();
}
}
