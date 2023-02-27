#pragma once 
#include "logutil.h"
#include <memory>
#include "Logger.h"
namespace zrpc {
extern Logger::ptr gRpcLogger;
inline bool OpenLog() {
  if (!gRpcLogger) {
    return false;
  }
  return true;
}
// #define DebugLog \
// 	if (zrpc::OpenLog() && zrpc::LogLevel::DEBUG >= zrpc::gRpcConfig->m_log_level) \
// 		zrpc::logging(zrpc::LogEvent::ptr(new zrpc::LogEvent(zrpc::LogLevel::DEBUG, __FILE__, __LINE__, __func__, zrpc::LogType::RPC_LOG))).getStringStream()

// #define InfoLog \
// 	if (zrpc::OpenLog() && zrpc::LogLevel::INFO >= zrpc::gRpcConfig->m_log_level) \
// 		zrpc::logging(zrpc::LogEvent::ptr(new zrpc::LogEvent(zrpc::LogLevel::INFO, __FILE__, __LINE__, __func__, zrpc::LogType::RPC_LOG))).getStringStream()

// #define WarnLog \
// 	if (zrpc::OpenLog() && zrpc::LogLevel::WARN >= zrpc::gRpcConfig->m_log_level) \
// 		zrpc::logging(zrpc::LogEvent::ptr(new zrpc::LogEvent(zrpc::LogLevel::WARN, __FILE__, __LINE__, __func__, zrpc::LogType::RPC_LOG))).getStringStream()

// #define ErrorLog \
// 	if (zrpc::OpenLog() && zrpc::LogLevel::ERROR >= zrpc::gRpcConfig->m_log_level) \
// 		zrpc::logging(zrpc::LogEvent::ptr(new zrpc::LogEvent(zrpc::LogLevel::ERROR, __FILE__, __LINE__, __func__, zrpc::LogType::RPC_LOG))).getStringStream()


// #define AppDebugLog \
// 	if (zrpc::OpenLog() && zrpc::LogLevel::DEBUG >= zrpc::gRpcConfig->m_app_log_level) \
// 		zrpc::logging(zrpc::LogEvent::ptr(new zrpc::LogEvent(zrpc::LogLevel::DEBUG, __FILE__, __LINE__, __func__, zrpc::LogType::APP_LOG))).getStringStream()

// #define AppInfoLog \
// 	if (zrpc::OpenLog() && zrpc::LogLevel::INFO >= zrpc::gRpcConfig->m_app_log_level) \
// 		zrpc::logging(zrpc::LogEvent::ptr(new zrpc::LogEvent(zrpc::LogLevel::INFO, __FILE__, __LINE__, __func__, zrpc::LogType::APP_LOG))).getStringStream()

// #define AppWarnLog \
// 	if (zrpc::OpenLog() && zrpc::LogLevel::WARN >= zrpc::gRpcConfig->m_app_log_level) \
// 		zrpc::logging(zrpc::LogEvent::ptr(new zrpc::LogEvent(zrpc::LogLevel::WARN, __FILE__, __LINE__, __func__, zrpc::LogType::APP_LOG))).getStringStream()

// #define AppErrorLog \
// 	if (zrpc::OpenLog() && zrpc::LogLevel::ERROR >= zrpc::gRpcConfig->m_app_log_level) \
// 		zrpc::logging(zrpc::LogEvent::ptr(new zrpc::LogEvent(zrpc::LogLevel::ERROR, __FILE__, __LINE__, __func__, zrpc::LogType::APP_LOG))).getStringStream()

#define InfoLog \
		zrpc::logging(zrpc::LogEvent::ptr(new zrpc::LogEvent(zrpc::LogLevel::INFO, __FILE__, __LINE__, __func__, zrpc::LogType::RPC_LOG))).getStringStream()


class logging {
public:
	explicit logging(LogEvent::ptr event);

	~logging();

	std::stringstream& getStringStream();

private:
	LogEvent::ptr m_event;	



};


}