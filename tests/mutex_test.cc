#include "zrpc/base/mutex.h"
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>
using namespace zrpc;

volatile int var = 10; // 保持变量 var 对内存可见性，防止编译器过度优化
RWMutex rwLock;
void Write() {
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 为了演示效果，先让write线程休眠10ms
    RWMutex::ReadLock readlock(rwLock);
    var += 10;
    std::cout << "write var : " << var << std::endl;

}

void Read() {
    RWMutex::WriteLock writelock(rwLock);
    std::cout << "read var : " << var << std::endl;
}
int main() {
    std::vector<std::thread> writers;
    std::vector<std::thread> readers;

    for (int i = 0; i < 10; i++) {  // 10 个写线程
        writers.push_back(std::thread(Write));  // std::thread t 的写法报错
    }
    for (int i = 0; i < 100; i++) {   // 100 个读线程
        readers.push_back(std::thread(Read));
    }
    for (auto& t : writers) {   // 写线程启动
        t.join();
    }
    for (auto& t : readers) {   // 读线程启动
        t.join();
    }
    std::cin.get();
}