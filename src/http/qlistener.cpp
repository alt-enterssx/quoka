#include "altenter/http/qlistener.h"

using namespace altenter::quokahttp::detail;

qlistener::qlistener(qlog_manager& log_manager): is_running(true), log_manager(log_manager), qs_socket(-1) {}

qlistener::~qlistener() { this->is_running = false; }

void qlistener::listen() {
    std::stringstream log_msg;
    
    if (this->qs_socket == -1) {
        log_msg << "Socket is invalid [" << CLASSNAMELOG << "], sock(" << this->qs_socket << ")";
        throw qexception(log_msg.str(), exception_type::CRITICAL);
    }

    while (is_running) {
        struct sockaddr_in* client_addr;
        socklen_t addr_len = sizeof(client_addr);

        if (accept(this->qs_socket, (struct sockaddr*) client_addr, &addr_len) == -1) {
            log_msg << "Error in connect client (" <<  std::strerror(errno) << ")";
            log_manager.log(log_msg.str(), log_type::ERROR);

            log_msg.str("");
            log_msg.clear();
        }

        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr->sin_addr), ip_str, INET_ADDRSTRLEN);
        
        log_msg << "[connection] client=" 
            << ip_str << ":" << ntohs(client_addr->sin_port);
        log_manager.log(log_msg.str(), log_type::DEBUG);

        log_msg.str("");
        log_msg.clear();
    }

    log_msg << "Server started to listening now";
    this->log_manager.log(log_msg.str(), log_type::INFO);

    log_msg.str("");
    log_msg.clear();

}   

void qlistener::set_socket(int qs_socket) { this->qs_socket = qs_socket; }