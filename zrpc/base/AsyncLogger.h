#pragma once 
#include <queue>
#include <memory>
#include <semaphore.h>
#include <pthread.h>
#include "logutil.h"
#include "mutex.h"
namespace zrpc {


class AsyncLogger {
public:
    typedef std::shared_ptr<AsyncLogger> ptr;

    AsyncLogger(const char* file_name, const char* file_path, int max_size, LogType logtype);
    ~AsyncLogger();

    void push(std::vector<std::string>& buffer);

    void flush();

    static void* excute(void*);

    void stop();

public:
    std::queue<std::vector<std::string>> m_tasks;

private:
    const char* m_file_name;
    const char* m_file_path;
    int m_max_size {0};
    LogType m_log_type;
    int m_no {0};
    bool m_need_reopen {false};
    FILE* m_file_handle {nullptr};
    std::string m_date;

    Mutex m_mutex;
    pthread_cond_t m_condition;
    bool m_stop {false};

public:
    pthread_t m_thread;
    sem_t m_semaphore;

};

}