#pragma once 
#include "logutil.h"
#include <memory>
#include "Logger.h"
namespace zrpc {
#define InfoLog \
		zrpc::logging(new zrpc::LogEvent(zrpc::LogLevel::INFO, __FILE__, __LINE__, __func__, zrpc::LogType::RPC_LOG)).getStringStream()
#define DebugLog \
		zrpc::logging(new zrpc::LogEvent(zrpc::LogLevel::DEBUG, __FILE__, __LINE__, __func__, zrpc::LogType::RPC_LOG)).getStringStream()
#define WarnLog \
		zrpc::logging(new zrpc::LogEvent(zrpc::LogLevel::WARN, __FILE__, __LINE__, __func__, zrpc::LogType::RPC_LOG)).getStringStream()

#define ErrorLog \
		zrpc::logging(new zrpc::LogEvent(zrpc::LogLevel::ERROR, __FILE__, __LINE__, __func__, zrpc::LogType::RPC_LOG)).getStringStream()

#define AppInfoLog \
		zrpc::logging(new zrpc::LogEvent(zrpc::LogLevel::INFO, __FILE__, __LINE__, __func__, zrpc::LogType::APP_LOG)).getStringStream()

#define AppWarnLog \
		zrpc::logging(new zrpc::LogEvent(zrpc::LogLevel::WARN, __FILE__, __LINE__, __func__, zrpc::LogType::APP_LOG)).getStringStream()
#define AppErrorLog \
		zrpc::logging(new zrpc::LogEvent(zrpc::LogLevel::ERROR, __FILE__, __LINE__, __func__, zrpc::LogType::APP_LOG)).getStringStream()

class logging {
public:
	explicit logging(LogEvent* event);

	~logging();

	std::stringstream& getStringStream();

private:
	LogEvent* m_event;	



};


}