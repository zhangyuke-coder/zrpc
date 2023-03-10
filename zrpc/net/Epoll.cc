#include "Epoll.h"
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <deque>
#include <queue>
#include <arpa/inet.h>
#include <iostream>
#include "zrpc/base/logging.h"
#include "Channel.h"
using namespace std;
const int EVENTSNUM = 4096;
const int EPOLLWAIT_TIME = 10000;



namespace zrpc {
Epoll::Epoll() : epollFd_(epoll_create1(EPOLL_CLOEXEC)), events_(EVENTSNUM) {
  assert(epollFd_ > 0);
}

Epoll::~Epoll() {}


// 注册新描述符
void Epoll::epoll_add(SP_Channel request, int timeout) {
    int fd = request->getFd();
    //   if (timeout > 0) {
    //     add_timer(request, timeout);
    //     fd2http_[fd] = request->getHolder();
    //   }
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();
    //*****************************************
    Coroutine* cur_cor = Coroutine::GetCurrentCoroutine() ;

    if (event.events & zrpc::IOEvent::READ) {
		DebugLog << "fd:[" << request->getFd() << "], register read event to epoll";
		// fd_event->setCallBack(tinyrpc::IOEvent::READ, 
		// 	[cur_cor, fd_event]() {
		// 		tinyrpc::Coroutine::Resume(cur_cor);
		// 	}
		// );
		request->setCoroutine(cur_cor);
	}
	if (event.events & zrpc::IOEvent::WRITE) {
		DebugLog << "fd:[" << request->getFd() << "], register write event to epoll";
		// fd_event->setCallBack(tinyrpc::IOEvent::WRITE, 
		// 	[cur_cor]() {
		// 		tinyrpc::Coroutine::Resume(cur_cor);
		// 	}
		// );
		request->setCoroutine(cur_cor);
	}
    //*****************************************
    
    request->EqualAndUpdateLastEvents();

    fd2chan_[fd] = request;
    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        perror("epoll_add error");
        fd2chan_[fd].reset();
    }
}


// 修改描述符状态
void Epoll::epoll_mod(SP_Channel request, int timeout) {
    // if (timeout > 0) add_timer(request, timeout);
    int fd = request->getFd();
    if (!request->EqualAndUpdateLastEvents()) {
        struct epoll_event event;
        event.data.fd = fd;
        event.events = request->getEvents();
        if (epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event) < 0) {
            perror("epoll_mod error");
            fd2chan_[fd].reset();
        }
    }
}

// 从epoll中删除描述符
void Epoll::epoll_del(SP_Channel request) {
    int fd = request->getFd();
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getLastEvents();
    // event.events = 0;
    // request->EqualAndUpdateLastEvents()
    if (epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event) < 0) {
        perror("epoll_del error");
    }
    fd2chan_[fd].reset();
    // fd2http_[fd].reset();
}



// 返回活跃事件数
std::vector<SP_Channel> Epoll::poll() {
    while (true) {
        int event_count =
            epoll_wait(epollFd_, &*events_.begin(), events_.size(), EPOLLWAIT_TIME);
        if (event_count < 0) perror("epoll wait error");
        std::vector<SP_Channel> req_data = getEventsRequest(event_count);
        if (req_data.size() > 0) return req_data;
    }
}


// 分发处理函数
std::vector<SP_Channel> Epoll::getEventsRequest(int events_num) {
    std::vector<SP_Channel> req_data;
    for (int i = 0; i < events_num; ++i) {
        // 获取有事件产生的描述符
        int fd = events_[i].data.fd;

        SP_Channel cur_req = fd2chan_[fd];

        if (cur_req) {
            cur_req->setRevents(events_[i].events);
            cur_req->setEvents(0);
            // 加入线程池之前将Timer和request分离
            // cur_req->seperateTimer();
            req_data.push_back(cur_req);
        } else {
            ErrorLog << "SP cur_req is invalid";

        }
    }
    return req_data;
}
}
