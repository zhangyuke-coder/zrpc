#pragma once
#include <sys/epoll.h>
#include <memory>
#include <unordered_map>
#include <vector>

namespace zrpc {

class Channel;
using SP_Channel = std::shared_ptr<Channel>;
class Epoll {
public:

public:
    Epoll();
    ~Epoll();

    void epoll_add(SP_Channel request, int timeout);
    void epoll_mod(SP_Channel request, int timeout);
    void epoll_del(SP_Channel request);

    std::vector<std::shared_ptr<Channel>> poll();
    std::vector<std::shared_ptr<Channel>> getEventsRequest(int events_num);
    int getEpollFd() { return epollFd_; }
    // void handleExpired();
private:
    static const int MAXFDS = 100000;
    int epollFd_;
    std::vector<epoll_event> events_;
    SP_Channel fd2chan_[MAXFDS];

};
}
