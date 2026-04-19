#include "altenter/http/QServer.h"

using namespace altenter::quokahttp;

QServer::QServer(): socket(this->logManager) {
    std::shared_ptr<QConsoleLogger> consoleLogger = std::make_shared<QConsoleLogger>();
    this->logManager.addLogger(consoleLogger);
}

QServer::~QServer() {
    this->isRunning = false;
}

void QServer::run(int port) {
    std::stringstream log_msg;
    if (port < 0x0 || port > 0xFFFF) {
        log_msg << "Argument port is invalid: (" << port << "), gets default port: (" << DEFAULT_PORT_START << ")";
        this->logManager.log(log_msg.str(), detail::LogType::WARNING);
        
        log_msg.str("");
        log_msg.clear();

        this->qs_port = DEFAULT_PORT_START;
    } else { this->qs_port = port; }

    try {
        socket.init_socket();
        socket.bind_address(this->qs_port);
    } catch(QException& ex) {
        detail::LogType typeFromExc = (ex.getType() == ExceptionType::ERROR) ? detail::LogType::ERROR : detail::LogType::CRITICAL;
        this->logManager.log(ex.what(), typeFromExc);

        this->logManager.shutdown();       

        if (ex.getType() == ExceptionType::CRITICAL) { exit(EXIT_FAILURE); }
    }

    this->isRunning = true;
    log_msg << "Server started and running: " "http://" << LOCAL_ADDRESS << ":" << this->qs_port;
    this->logManager.log(log_msg.str(), detail::LogType::INFO);

    log_msg.str("");
    log_msg.clear();    
}