#pragma once 
#include <memory>
#include "AsyncLogger.h"

namespace zrpc {
class Logger {
 public:
  typedef std::shared_ptr<Logger> ptr;

	Logger();
	~Logger();

	void init(const char* file_name, const char* file_path, int max_size, int sync_inteval);
	void log();
	void pushRpcLog(const std::string& log_msg);
	void pushAppLog(const std::string& log_msg);
	void loopFunc();

	void flush();

	void start();

	AsyncLogger::ptr getAsyncLogger() {
		return m_async_rpc_logger;
	}

	AsyncLogger::ptr getAsyncAppLogger() {
		return m_async_app_logger;
	}

 public:
	std::vector<std::string> m_buffer;
	std::vector<std::string> m_app_buffer;

 private:
 	Mutex m_app_buff_mutex;
 	Mutex m_buff_mutex;
	bool m_is_init {false};
	AsyncLogger::ptr m_async_rpc_logger;
	AsyncLogger::ptr m_async_app_logger;

	int m_sync_inteval {0};

};
}
