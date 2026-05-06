#include "altenter/info/qlog_manager.h"

using namespace altenter::quokahttp::detail;

qlog_manager::qlog_manager() {}

qlog_manager::~qlog_manager() {
    qconsole_logger::logger().shutdown();
}

qlog_manager& qlog_manager::manager() { 
    static qlog_manager instance;
    return instance;
}

void qlog_manager::log(const std::string& msg, log_type type) {
    qconsole_logger::logger().log(msg, type);
}

void qlog_manager::shutdown() {
    qconsole_logger::logger().shutdown();
}