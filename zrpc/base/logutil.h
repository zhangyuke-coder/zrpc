#pragma once
#include <memory>
#include <sstream>
namespace zrpc{
void CoredumpHandler(int signal_no);
enum LogLevel {
	DEBUG = 1,
	INFO = 2,
	WARN = 3,
	ERROR = 4,
	NONE = 5    // don't print log
};
enum LogType {
	RPC_LOG = 1,
	APP_LOG = 2,
};	
class LogEvent {

public:

	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent(LogLevel level, const char* file_name, int line, const char* func_name, LogType type);

	~LogEvent();

	std::stringstream& getStringStream();

	void log();


private:
	
	// uint64_t m_timestamp;
	timeval m_timeval;
	LogLevel m_level;
	pid_t m_pid {0};
	pid_t m_tid {0};
	int m_cor_id {0};

	const char* m_file_name;
	int m_line {0};
	const char* m_func_name;
	LogType m_type;
	std::string m_msg_no;
	std::stringstream m_ss;

};
}
