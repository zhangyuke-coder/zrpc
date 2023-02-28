#include "AsyncLogger.h"
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "zrpc/start.h"
#include "logutil.h"
#include "Logger.h"
namespace {
std::string LogTypeToString(zrpc::LogType logtype) {
  switch (logtype) {
    case zrpc::APP_LOG:
      return "app";
    case zrpc::RPC_LOG:
      return "rpc";
    default:
      return "";
  }
}
}
namespace zrpc {
AsyncLogger::AsyncLogger(const char* file_name, const char* file_path, int max_size, LogType logtype)
    : m_file_name(file_name), m_file_path(file_path), m_max_size(max_size), m_log_type(logtype) {
    int rt = sem_init(&m_semaphore, 0, 0);
    assert(rt == 0);

    rt = pthread_create(&m_thread, nullptr, &AsyncLogger::excute, this);
    assert(rt == 0);
    rt = sem_wait(&m_semaphore);
    assert(rt == 0);

}


AsyncLogger::~AsyncLogger() {

}



void* AsyncLogger::excute(void* arg) {
  AsyncLogger* ptr = reinterpret_cast<AsyncLogger*>(arg);
  int rt = pthread_cond_init(&ptr->m_condition, NULL);
  assert(rt == 0);

  rt = sem_post(&ptr->m_semaphore);
  assert(rt == 0);

  while (1) {
    Mutex::Lock lock(ptr->m_mutex);

    while (ptr->m_tasks.empty() && !ptr->m_stop) {
      pthread_cond_wait(&(ptr->m_condition), ptr->m_mutex.getMutex());
    }
    std::vector<std::string> tmp;
    tmp.swap(ptr->m_tasks.front());
    ptr->m_tasks.pop();
    bool is_stop = ptr->m_stop;
    lock.unlock();

    timeval now;
    gettimeofday(&now, nullptr);

    struct tm now_time;
    localtime_r(&(now.tv_sec), &now_time);

    const char *format = "%Y%m%d";
    char date[32];
    strftime(date, sizeof(date), format, &now_time);
    if (ptr->m_date != std::string(date)) {
      // cross day
      // reset m_no m_date
      ptr->m_no = 0;
      ptr->m_date = std::string(date);
      ptr->m_need_reopen = true;
    }

    if (!ptr->m_file_handle) {
      ptr->m_need_reopen = true;
    }    

    std::stringstream ss;
    ss << ptr->m_file_path << ptr->m_file_name << "_" << ptr->m_date << "_" << LogTypeToString(ptr->m_log_type) << "_" << ptr->m_no << ".log";
    std::string full_file_name = ss.str();

    if (ptr->m_need_reopen) {
      if (ptr->m_file_handle) {
        fclose(ptr->m_file_handle);
      }

      ptr->m_file_handle = fopen(full_file_name.c_str(), "a");
      ptr->m_need_reopen = false;
    }

    if (ftell(ptr->m_file_handle) > ptr->m_max_size) {
      fclose(ptr->m_file_handle);

      // single log file over max size
      ptr->m_no++;
      std::stringstream ss2;
      ss2 << ptr->m_file_path << ptr->m_file_name << "_" << ptr->m_date << "_" << LogTypeToString(ptr->m_log_type) << "_" << ptr->m_no << ".log";
      full_file_name = ss2.str();

      // printf("open file %s", full_file_name.c_str());
      ptr->m_file_handle = fopen(full_file_name.c_str(), "a");
      ptr->m_need_reopen = false;
    }

    if (!ptr->m_file_handle) {
      printf("open log file %s error!", full_file_name.c_str());
    }

    for(auto i : tmp) {
      if (!i.empty()) {
        fwrite(i.c_str(), 1, i.length(), ptr->m_file_handle);
      }
    }
    tmp.clear();
    fflush(ptr->m_file_handle);
    if (is_stop) {
      break;
    }

  }
  if (!ptr->m_file_handle) {
    fclose(ptr->m_file_handle);
  }

  return nullptr;

}



void AsyncLogger::push(std::vector<std::string>& buffer) {
  if (!buffer.empty()) {
    Mutex::Lock lock(m_mutex);
    m_tasks.push(buffer);
    lock.unlock();
    pthread_cond_signal(&m_condition);
  }
}




void AsyncLogger::flush() {
  if (m_file_handle) {
    fflush(m_file_handle);
  }
}


void AsyncLogger::stop() {
  if (!m_stop) {
    m_stop = true;
    pthread_cond_signal(&m_condition);
  }
}


}