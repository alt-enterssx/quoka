#include "altenter/info/QLogManager.h"

using namespace altenter::quokahttp::detail;

QLogManager::QLogManager() {}

QLogManager::~QLogManager() {
    for (auto& logger : this->loggers) {
        logger->shutdown();
    }
}

void QLogManager::addLogger(std::shared_ptr<detail::QILogger> logger) {
    this->loggers.push_back(logger);
}

void QLogManager::log(const std::string& msg, detail::LogType type) {
    for (auto& logger : this->loggers) {
        logger->log(msg, type);
    }
}

void QLogManager::shutdown() {
    for (auto& logger : this->loggers) {
        logger->shutdown();
    }
}