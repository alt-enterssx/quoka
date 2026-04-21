#include "altenter/info/qlog_manager.h"

using namespace altenter::quokahttp::detail;

qlog_manager::qlog_manager() {}

qlog_manager::~qlog_manager() {
    for (auto& logger : this->loggers) {
        logger->shutdown();
    }
}

void qlog_manager::add_logger(std::shared_ptr<q_i_logger> logger) {
    this->loggers.push_back(logger);
}

void qlog_manager::log(const std::string& msg, log_type type) {
    for (auto& logger : this->loggers) {
        logger->log(msg, type);
    }
}

void qlog_manager::shutdown() {
    for (auto& logger : this->loggers) {
        logger->shutdown();
    }
}