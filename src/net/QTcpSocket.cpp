#include "altenter/net/QTcpSocket.h"

using namespace altenter::quokahttp::detail;

QTcpSocket::QTcpSocket() {}
QTcpSocket::~QTcpSocket() {}

void QTcpSocket::init_socket() {
    std::stringstream log_msg;

    this->qs_socket = socket(
        AF_INET, SOCK_STREAM, 0
    );

    if (this->qs_socket == -1) {
        log_msg << "Error in init socket: (" << std::strerror(errno) << ")";
        throw QException(log_msg.str(), ExceptionType::CRITICAL);
    
        log_msg.str("");
        log_msg.clear();
    }

    log_msg << "Socket created successfully, fd: (" << this->qs_socket << ")";
    this->logAll(log_msg.str(), LogType::DEBUG);

    log_msg.str("");
    log_msg.clear();
}

void QTcpSocket::bind_address(uint16_t port) {
    std::stringstream log_msg;

    this->qs_address.sin_family = AF_INET;
    this->qs_address.sin_addr.s_addr = htonl(INADDR_ANY);
    this->qs_address.sin_port = htons(port);

    socklen_t address_len = sizeof(this->qs_address);

    if (bind(this->qs_socket, (struct sockaddr*) &this->qs_address, address_len) == -1) {
        log_msg << "Error in bind address to server: (" << std::strerror(errno) << ")";
        throw QException(log_msg.str(), ExceptionType::CRITICAL);

        log_msg.str("");
        log_msg.clear();
    }
}

void QTcpSocket::addLogger(std::shared_ptr<QILogger> logger) { this->loggers.push_back(logger); }
 
int QTcpSocket::getSocket() { return this->qs_socket; }

sockaddr_in QTcpSocket::getAddress() { return this->qs_address; }

void QTcpSocket::logAll(const std::string& msg, LogType type) {
    for (auto& logger : loggers) {
        logger->log(msg, type);
    }
}