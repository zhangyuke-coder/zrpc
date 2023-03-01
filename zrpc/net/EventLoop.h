#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "Channel.h"
#include "Epoll.h"
#include "zrpc/base/logging.h"
#include "zrpc/base/mutex.h"
#include <iostream>
#include <assert.h>
#include <thread>
using namespace std;
namespace zrpc {
class EventLoop {
public:
    typedef std::function<void()> Functor;  

    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void runInLoop(Functor&& cb);
    void queueInLoop(Functor&& cb);
    bool isInLoopThread() const { return threadId_ == this_thread::get_id(); }
    void assertInLoopThread() { assert(isInLoopThread()); }
    // void shutdown(shared_ptr<Channel> channel) { shutDownWR(channel->getFd()); }
    void removeFromPoller(shared_ptr<Channel> channel) {
        // shutDownWR(channel->getFd());
        poller_->epoll_del(channel);
    }
    void updatePoller(shared_ptr<Channel> channel, int timeout = 0) {
        poller_->epoll_mod(channel, timeout);
    }
    void addToPoller(shared_ptr<Channel> channel, int timeout = 0) {
    poller_->epoll_add(channel, timeout);
    }







 private:
    // 声明顺序 wakeupFd_ > pwakeupChannel_
    bool looping_;
    shared_ptr<Epoll> poller_;
    int wakeupFd_;
    bool quit_;
    bool eventHandling_;
    mutable Mutex mutex_;
    std::vector<Functor> pendingFunctors_;
    bool callingPendingFunctors_;
    const thread::id threadId_;
    shared_ptr<Channel> pwakeupChannel_;

    void wakeup();
    void handleRead();
    void doPendingFunctors();
    void handleConn();

};
}