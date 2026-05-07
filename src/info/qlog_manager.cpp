#include "altenter/info/qlog_manager.h"

using namespace altenter::quokahttp::detail;

qlog_manager::qlog_manager(): flag(0xF0) {}

qlog_manager::~qlog_manager() {
    qconsole_logger::logger().shutdown();
}

qlog_manager& qlog_manager::manager() { 
    static qlog_manager instance;
    return instance;
}

void qlog_manager::log(const std::string& msg, log_type type) {
    if ((flag & 0xF0) > 0) {
        qconsole_logger::logger().log(msg, type);
    }
}

void qlog_manager::log(const std::string& msg, exception_type type) {
    detail::log_type type_from_exc = (type == exception_type::ERROR) ? detail::log_type::ERROR : detail::log_type::CRITICAL;

    if ((flag & 0xF0) > 0) {
        qconsole_logger::logger().log(msg, type_from_exc);
    }
}

void qlog_manager::set_console(bool status) { this->flag = (status) ? (flag | 0xF0) : (flag & 0x00); }

void qlog_manager::shutdown() {
    if ((flag & 0xF0) > 0) {
        qconsole_logger::logger().shutdown();
    }
}