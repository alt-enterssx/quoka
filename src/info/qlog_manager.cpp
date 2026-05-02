#include "altenter/info/qlog_manager.h"

using namespace altenter::quokahttp::detail;

qlog_manager::qlog_manager() {}

qlog_manager::~qlog_manager() {
    for (auto& logger : this->loggers) {
        logger->shutdown();
    }
}

qlog_manager& qlog_manager::get_instance() { 
    static qlog_manager manager;
    return manager;
}

void qlog_manager::add_logger(std::shared_ptr<q_i_logger> logger) {
    this->loggers.push_back(logger);
}

void qlog_manager::log(const std::string& msg, log_type type) {
    for (int i = 0; i < this->loggers.size(); i++) {
        this->loggers.at(i)->log(msg, type);
    }
}

void qlog_manager::shutdown() {
    for (int i = 0; i < this->loggers.size(); i++) {
        this->loggers.at(i)->shutdown();
    }
}