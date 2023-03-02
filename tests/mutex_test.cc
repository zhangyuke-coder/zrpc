#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include "zrpc/base/mutex.h"
volatile int counter(0); // non-atomic counter
// std::mutex mtx;           // locks access to counter
 zrpc::Mutex mtx;
void attempt_10k_increases() {
    for (int i=0; i<10000; ++i) {
        zrpc::Mutex::Lock lock(mtx);
    
            ++counter;
      
    }
}
 
int main (int argc, const char* argv[]) {
    std::thread threads[10];
    for (int i=0; i<10; ++i)
        threads[i] = std::thread(attempt_10k_increases);
 
    for (auto& th : threads) th.join();
    std::cout << counter << " successful increases of the counter.\n";
 
    return 0;
}