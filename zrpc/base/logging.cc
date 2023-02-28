#include "logging.h"


namespace zrpc {
logging::logging(LogEvent* event)
    : m_event(event) {

}
logging::~logging() {
    m_event->log();
    delete m_event;

}





std::stringstream& logging::getStringStream() {
  return m_event->getStringStream();
}

}

