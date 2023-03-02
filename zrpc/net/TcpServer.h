#pragma once
#include <memory>
#include "Channel.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
namespace zrpc{
class TcpServer {
 public:
  TcpServer(EventLoop *loop, int threadNum, int port);
  ~TcpServer() {}
  EventLoop *getLoop() const { return loop_; }
  void start();
  void handNewConn();
  void handThisConn() { loop_->updatePoller(acceptChannel_); }

 private:
  EventLoop *loop_;
  int threadNum_;
  std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;
  bool started_;
  std::shared_ptr<Channel> acceptChannel_;
  int port_;
  int listenFd_;
  static const int MAXFDS = 100000;
};
}
