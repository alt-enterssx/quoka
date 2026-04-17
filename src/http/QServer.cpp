#include "altenter/http/QServer.h"

using namespace altenter::quokahttp;

QServer::QServer(): socket() {}

QServer::~QServer() {
    this->isRunning = false;
}

void QServer::run(int port) {
    std::stringstream log_msg;
    if (port < 0x0 || port > 0xFFFF) {
        log_msg << "Argument port is invalid: (" << port << "), gets default port: (" << DEFAULT_PORT_START << ")";
        this->logAll(log_msg.str(), detail::LogType::WARNING);
        
        log_msg.str("");
        log_msg.clear();

        this->qs_port = DEFAULT_PORT_START;
    } else { this->qs_port = port; }

    try {
        socket.init_socket();
        socket.bind_address(this->qs_port);
    } catch(QException& ex) {
        detail::LogType typeFromExc = (ex.getType() == ExceptionType::ERROR) ? detail::LogType::ERROR : detail::LogType::CRITICAL;
        this->logAll(ex.what(), typeFromExc);

        this->shutDownAll();       

        if (ex.getType() == ExceptionType::CRITICAL) { exit(EXIT_FAILURE); }
    }

    this->isRunning = true;
    log_msg << "Server started and running on port: (" << this->qs_port << ")";
    this->logAll(log_msg.str(), detail::LogType::INFO);

    log_msg.str("");
    log_msg.clear();
}

void QServer::addLogger(std::shared_ptr<detail::QILogger> logger) {
    this->loggers.push_back(logger);

    {
        this->socket.addLogger(logger);
    }
}

void QServer::shutDownAll() {
    for (auto& logger : this->loggers) {
        logger->shutdown();
    }
}

void QServer::logAll(const std::string& msg, detail::LogType type) {
    for (auto& logger : this->loggers) {
        logger->log(msg, type);
    }
}