#include "altenter/info/qlog_manager.h"

using namespace altenter::quokahttp::detail;

qlog_manager::qlog_manager(): flag(0x00) {}

qlog_manager::~qlog_manager() {
    if ((this->flag & 0xF0) > 0) {
        qconsole_logger::logger().shutdown();
    }
}

qlog_manager& qlog_manager::manager() { 
    static qlog_manager instance;
    return instance;
}

void qlog_manager::log(const std::string& msg, log_type type) {
    if ((this->flag & 0xF0) > 0) {
        qconsole_logger::logger().log(msg, type);
    }
}

void qlog_manager::set_flag(uint8_t flag) { this->flag = flag; }

void qlog_manager::shutdown() {
    if ((this->flag & 0xF0) > 0) {
        qconsole_logger::logger().shutdown();
    }
}