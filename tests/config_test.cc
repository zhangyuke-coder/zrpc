#include "zrpc/config.h"
#include "zrpc/base/logging.h"
#include <thread>
#include <iostream>       // std::cout, std::endl

#include <chrono>         // std::chrono::seconds

int main() {
    zrpc::Config::ptr config(new zrpc::Config("./test_tinypb_server.xml"));
    config->readConf();
    InfoLog << "asdfasdf";
    InfoLog << "qwerqwer";
    InfoLog << "75375375";
    InfoLog << "mjghjm";
    InfoLog << "3453456";
    InfoLog << "vbsb";
    std::this_thread::sleep_for(std::chrono::seconds(3));
}