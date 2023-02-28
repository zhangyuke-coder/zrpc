#include "logutil.h"
#include "Logger.h"
#include <time.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <functional>
#include <unistd.h>
#include <signal.h>
namespace zrpc{
static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
zrpc::Logger* gRpcLogger_;

void once_init()
{

  gRpcLogger_  = new zrpc::Logger();
  
}


}
namespace zrpc {
void CoredumpHandler(int signal_no) {
    // ErrorLog << "progress received invalid signal, will exit";
    printf("progress received invalid signal, will exit\n");
    gRpcLogger_->flush();
    pthread_join(gRpcLogger_->getAsyncLogger()->m_thread, NULL);
    pthread_join(gRpcLogger_->getAsyncAppLogger()->m_thread, NULL);

    signal(signal_no, SIG_DFL);
    raise(signal_no);
}


static thread_local pid_t t_thread_id = 0;
static pid_t g_pid = 0;
namespace {



pid_t gettid() {
  if (t_thread_id == 0) {
    t_thread_id = syscall(SYS_gettid);
  }
  return t_thread_id;
}
std::string levelToString(LogLevel level) {
  std::string re = "DEBUG";
  switch(level) {
    case DEBUG:
      re = "DEBUG";
      return re;
    
    case INFO:
      re = "INFO";
      return re;

    case WARN:
      re = "WARN";
      return re;

    case ERROR:
      re = "ERROR";
      return re;
    case NONE:
      re = "NONE";

    default:
      return re;
  }
}
LogLevel stringToLevel(const std::string& str) {
    if (str == "DEBUG")
      return LogLevel::DEBUG;
    
    if (str == "INFO")
      return LogLevel::INFO;

    if (str == "WARN")
      return LogLevel::WARN;

    if (str == "ERROR")
      return LogLevel::ERROR;

    if (str == "NONE")
      return LogLevel::NONE;

    return LogLevel::DEBUG;
}

std::string LogTypeToString(LogType logtype) {
  switch (logtype) {
    case APP_LOG:
      return "app";
    case RPC_LOG:
      return "rpc";
    default:
      return "";
  }
}
}


LogEvent::LogEvent(LogLevel level, const char* file_name, 
                   int line, const char* func_name, LogType type):
                   m_level(level),m_file_name(file_name),
                   m_line(line), m_func_name(func_name), m_type(type){

}


LogEvent::~LogEvent(){

}


void LogEvent::log() {
  m_ss << "\n";

  pthread_once(&once_control_, once_init);

  gRpcLogger_->init(m_file_name, "", 1000000,2);
  if (m_type == zrpc::RPC_LOG) {
    gRpcLogger_->pushRpcLog(m_ss.str());
  } else if (m_type == APP_LOG) {
    gRpcLogger_->pushAppLog(m_ss.str());
  }
  
  gRpcLogger_->start();
}

std::stringstream& LogEvent::getStringStream() {

    // time_t now_time = m_timestamp;

    gettimeofday(&m_timeval, nullptr);

    struct tm time; 
    localtime_r(&(m_timeval.tv_sec), &time);

    const char* format = "%Y-%m-%d %H:%M:%S";
    char buf[128];
    strftime(buf, sizeof(buf), format, &time);

    m_ss << "[" << buf << "." << m_timeval.tv_usec << "]\t"; 

    std::string s_level = levelToString(m_level);
    m_ss << "[" << s_level << "]\t";

    if (g_pid == 0) {
        g_pid = getpid();
    }
    m_pid = g_pid;  

    if (t_thread_id == 0) {
        t_thread_id = gettid();
    }
    m_tid = t_thread_id;
    m_ss << "[" << m_pid << "]\t" 
		<< "[" << m_tid << "]\t"
		<< "[" <<  "]\t"
    << "[" << m_file_name << ":" << m_line << "]\t";
    // << "[" << m_func_name << "]\t";

    return m_ss;


}
}