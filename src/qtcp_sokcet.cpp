#include "altenter/qtcp_socket.h"

using namespace altenter::quoka::detail;

qtcp_socket::qtcp_socket() {}
qtcp_socket::~qtcp_socket() {}

void qtcp_socket::init_socket() {
    std::stringstream log_msg;

    this->qs_socket = socket(
        AF_INET, SOCK_STREAM, 0
    );

    if (this->qs_socket == -1) {
        log_msg << "Error in init socket: (" << std::strerror(errno) << ")";
        throw qexception(log_msg.str(), exception_type::CRITICAL);
    
        log_msg.str("");
        log_msg.clear();
    }

    detail::qlog_manager::manager().logFormat(
        "Socket created successfully, fd: ({})", 
        log_type::DEBUG, 
        this->qs_socket
    );
}

void qtcp_socket::bind_address(uint16_t port) {
    std::stringstream log_msg;

    this->qs_address.sin_family = AF_INET;
    this->qs_address.sin_addr.s_addr = htonl(INADDR_ANY);
    this->qs_address.sin_port = htons(port);

    socklen_t address_len = sizeof(this->qs_address);

    if (
        bind(
            this->qs_socket, 
            (struct sockaddr*) &this->qs_address, 
            address_len
        ) == -1
    ) {
        log_msg << "Error in bind address to server: (" << std::strerror(errno) << ")";
        throw qexception(log_msg.str(), exception_type::CRITICAL);

        log_msg.str("");
        log_msg.clear();
    }

    detail::qlog_manager::manager().logFormat(
        "Server binded on port: ({})", 
        log_type::DEBUG, 
        port
    );

    if (listen(this->qs_socket, SOMAXCONN) == -1) {
        log_msg << "Error in listening socket: (" << std::strerror(errno) << ")";
        throw qexception(log_msg.str(), exception_type::CRITICAL);

        log_msg.str("");
        log_msg.clear();
    }

    detail::qlog_manager::manager().log(
        "Server started to listen", 
        log_type::DEBUG
    );
}

int qtcp_socket::get_socket() { return this->qs_socket; }

sockaddr_in qtcp_socket::get_address() { return this->qs_address; }