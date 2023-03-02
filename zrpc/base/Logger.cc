#include "Logger.h"
#include <semaphore.h>
#include <signal.h>
#include "logging.h"
#include "logutil.h"

#include "logutil.h"

namespace zrpc {

Logger::Logger() {
  // cannot do anything which will call LOG ,otherwise is will coredump

}

Logger::~Logger() {
    flush();
    pthread_join(m_async_rpc_logger->m_thread, NULL);
    pthread_join(m_async_app_logger->m_thread, NULL);
}
void Logger::init(const char* file_name, const char* file_path, int max_size, int sync_inteval) {
    if (!m_is_init) {
        m_sync_inteval = sync_inteval;
        for (int i = 0 ; i < 1000000; ++i) {
            m_app_buffer.push_back("");
            m_buffer.push_back("");
        }
        // m_app_buffer.resize(1000000);
        // m_buffer.resize(1000000);

        m_async_rpc_logger = std::make_shared<AsyncLogger>(file_name, file_path, max_size, RPC_LOG);
        m_async_app_logger = std::make_shared<AsyncLogger>(file_name, file_path, max_size, APP_LOG);

        signal(SIGSEGV, CoredumpHandler);
        signal(SIGABRT, CoredumpHandler);
        signal(SIGTERM, CoredumpHandler);
        signal(SIGKILL, CoredumpHandler);
        signal(SIGINT, CoredumpHandler);
        signal(SIGSTKFLT, CoredumpHandler);

        // ignore SIGPIPE 
        signal(SIGPIPE, SIG_IGN);
        m_is_init = true;
    }
}


void Logger::start() {
    // TimerEvent::ptr event = std::make_shared<TimerEvent>(m_sync_inteval, true, std::bind(&Logger::loopFunc, this));
    // Reactor::GetReactor()->getTimer()->addTimerEvent(event);
    loopFunc();
}


void Logger::loopFunc() {
    std::vector<std::string> app_tmp;
    Mutex::Lock lock1(m_app_buff_mutex);
    app_tmp.swap(m_app_buffer);
    lock1.unlock();

    std::vector<std::string> tmp;
    Mutex::Lock lock2(m_buff_mutex);
    tmp.swap(m_buffer);
    lock2.unlock();

    m_async_rpc_logger->push(tmp);
    m_async_app_logger->push(app_tmp);
}

void Logger::pushRpcLog(const std::string& msg) {
    // Mutex::Lock lock(m_buff_mutex);
    m_buff_mutex.lock();
    m_buffer.push_back(std::move(msg));
    // lock.unlock();
    m_buff_mutex.unlock();
}

void Logger::pushAppLog(const std::string& msg) {
    Mutex::Lock lock(m_app_buff_mutex);
    m_app_buffer.push_back(std::move(msg));
    // lock.unlock();
}

void Logger::flush() {
    loopFunc();
    m_async_rpc_logger->stop();
    m_async_rpc_logger->flush();

    m_async_app_logger->stop();
    m_async_app_logger->flush();
}
}
