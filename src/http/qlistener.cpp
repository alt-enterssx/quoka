#include "altenter/http/qlistener.h"

using namespace altenter::quokahttp::detail;

qlistener::qlistener(qlog_manager& log_manager): is_running(true), log_manager(log_manager), qs_socket(-1), pool() {}

qlistener::~qlistener() { this->is_running = false; }

void qlistener::listen() {
    std::stringstream log_msg;
    
    if (this->qs_socket == -1) {
        log_msg << "Socket is invalid [" << CLASSNAMELOG << "], sock(" << this->qs_socket << ")";
        throw qexception(log_msg.str(), exception_type::CRITICAL);
    }

    log_msg << "Server started to listening now";
    this->log_manager.log(log_msg.str(), log_type::INFO);

    log_msg.str("");
    log_msg.clear();

    while (is_running) {
        struct sockaddr_in* client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_sock;

        if ((client_sock = accept(this->qs_socket, (struct sockaddr*) client_addr, &addr_len)) == -1) {
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

        qrequest_item item([this](int socket) -> void {
            this->process(socket);
        }, client_sock);

        pool.add_task(item);

        log_msg.str("");
        log_msg.clear();
    }
}   

void qlistener::set_socket(int qs_socket) { this->qs_socket = qs_socket; }

void qlistener::process(int socket) {
    std::stringstream log_msg;

    size_t buff_siz = 1024;
    char* buffer = new char[buff_siz];
    
    int recv_out = recv(socket, buffer, buff_siz, 0);

    if (recv_out == -1) {
        log_msg << "Error get data from connection (" << std::strerror(errno) << ")"; 
        this->log_manager.log(log_msg.str(), log_type::ERROR);

        log_msg.str("");
        log_msg.clear();
        
        close(socket);
        return; 
    } else if (recv_out == 0) {
        this->log_manager.log("[connection] closed", log_type::DEBUG);

        close(socket);
        return;
    } else {
        log_msg << "[connection] recived data: " << buffer;
        this->log_manager.log(log_msg.str(), log_type::INFO);
        
        log_msg.str("");
        log_msg.clear();
    }

    delete[] buffer;
    close(socket);
}