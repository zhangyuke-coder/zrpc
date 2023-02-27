#include "logging.h"


namespace zrpc {
logging::logging(LogEvent::ptr event)
    : m_event(event) {

}
logging::~logging() {
    m_event->log();
}





std::stringstream& logging::getStringStream() {
  return m_event->getStringStream();
}

}

