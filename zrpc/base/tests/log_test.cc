#include "../logging.h"

using namespace zrpc;
void test() {
    InfoLog << "success";
    InfoLog << "asdf";
}
int main() {
    test();
    auto logevent = new zrpc::LogEvent(zrpc::LogLevel::INFO, __FILE__, __LINE__, __func__, zrpc::LogType::RPC_LOG);
    logging w(LogEvent::ptr(logevent));
    auto ss = w.getStringStream();
}