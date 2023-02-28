#include "config.h"
#include "zrpc/base/Logger.h"
#include "zrpc/base/logutil.h"
namespace zrpc {
static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
Logger* gRpcLogger_;

void once_init()
{

    gRpcLogger_  = new Logger();

}

Config::Config(const char* file_path) : m_file_path(std::string(file_path)) {
    m_xml_file = new TiXmlDocument();
    bool rt = m_xml_file->LoadFile(file_path);
    if (!rt) {
        printf("start tinyrpc server error! read conf file [%s] error info: [%s], errorid: [%d], error_row_column:[%d row %d column]\n", 
            file_path, m_xml_file->ErrorDesc(), m_xml_file->ErrorId(), m_xml_file->ErrorRow(), m_xml_file->ErrorCol());
        exit(0);
    }
}


void Config::readLogConfig(TiXmlElement* log_node) {
    TiXmlElement* node = log_node->FirstChildElement("log_path");
    if(!node || !node->GetText()) {
        printf("start tinyrpc server error! read config file [%s] error, cannot read [log_path] xml node\n", m_file_path.c_str());
        exit(0);
    }
    m_log_path = std::string(node->GetText());

    node = log_node->FirstChildElement("log_prefix");
    if(!node || !node->GetText()) {
        printf("start tinyrpc server error! read config file [%s] error, cannot read [log_prefix] xml node\n", m_file_path.c_str());
        exit(0);
    }
    m_log_prefix = std::string(node->GetText());

    node = log_node->FirstChildElement("log_max_file_size");
    if(!node || !node->GetText()) {
        printf("start tinyrpc server error! read config file [%s] error, cannot read [log_max_file_size] xml node\n", m_file_path.c_str());
        exit(0);
    }

    int log_max_size = std::atoi(node->GetText());
    m_log_max_size = log_max_size * 1024 * 1024;


    node = log_node->FirstChildElement("rpc_log_level");
    if(!node || !node->GetText()) {
        printf("start tinyrpc server error! read config file [%s] error, cannot read [rpc_log_level] xml node\n", m_file_path.c_str());
        exit(0);
    }

    std::string log_level = std::string(node->GetText());
    m_log_level = stringToLevel(log_level);

    node = log_node->FirstChildElement("app_log_level");
    if(!node || !node->GetText()) {
    printf("start tinyrpc server error! read config file [%s] error, cannot read [app_log_level] xml node\n", m_file_path.c_str());
    exit(0);
    }

    log_level = std::string(node->GetText());
    m_app_log_level = stringToLevel(log_level);

    node = log_node->FirstChildElement("log_sync_inteval");
    if(!node || !node->GetText()) {
    printf("start tinyrpc server error! read config file [%s] error, cannot read [log_sync_inteval] xml node\n", m_file_path.c_str());
    exit(0);
    }

    m_log_sync_inteval = std::atoi(node->GetText());
    pthread_once(&once_control_, once_init);
    gRpcLogger_->init(m_log_prefix.c_str(), m_log_path.c_str(), m_log_max_size, m_log_sync_inteval);

}










void Config::readConf() {
  TiXmlElement* root = m_xml_file->RootElement();
  TiXmlElement* log_node = root->FirstChildElement("log");
  if (!log_node) {
    printf("start tinyrpc server error! read config file [%s] error, cannot read [log] xml node\n", m_file_path.c_str());
    exit(0);
  }

  readLogConfig(log_node);




}


Config::~Config() {
  if (m_xml_file) {
    delete m_xml_file;
    m_xml_file = NULL;
  }
}
}