#include "zrpc/base/logging.h"
#include <thread>
#include <iostream>       // std::cout, std::endl

#include <chrono>         // std::chrono::seconds

using namespace zrpc;

int main() {
    InfoLog << "successasdfasdfffffffffffffffffffff";
    InfoLog << "asdf";
    InfoLog << "qwer";
    InfoLog << "azxcvsdf";
    InfoLog << "785876";
    AppInfoLog << "zzzz";

    std::this_thread::sleep_for(std::chrono::seconds(3));
    // InfoLog << "qwerwqerwqerwer";
    // InfoLog << "651651651";
}