#include "altenter/http/qlistener.h"

using namespace altenter::quokahttp::detail;

qlistener::qlistener(qrouter& router): is_running(true), router(router), qs_socket(-1), pool() {}

qlistener::~qlistener() { this->is_running = false; }

void qlistener::listen() {
    std::stringstream log_msg;

    if (this->qs_socket == -1) {
        log_msg << "Socket is invalid [" << CLASSNAMELOG << "], sock(" << this->qs_socket << ")";
        throw qexception(log_msg.str(), exception_type::CRITICAL);
    }

    detail::qlog_manager::manager().log("Server started to listening now", log_type::INFO);

    while (this->is_running) {

        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);

        int client_sock = accept(
            this->qs_socket,
            (struct sockaddr*)&client_addr,
            &addr_len
        );

        if (client_sock == -1) {
            detail::qlog_manager::manager().logFormat("Error in accept ({})", log_type::ERROR, std::strerror(errno));
            continue;
        }

        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, INET_ADDRSTRLEN);

        detail::qlog_manager::manager().logFormat("[connection] client={}:{}", log_type::DEBUG, ip_str, ntohs(client_addr.sin_port));

        qrequest_item item([this](int socket) {
            this->process(socket);
        }, client_sock);

        pool.add_task(item);
    }
}  

void qlistener::set_socket(int qs_socket) { this->qs_socket = qs_socket; }

void qlistener::process(int socket) {
    size_t buff_siz = 1024;
    char* buffer = new char[buff_siz];
    
    int recv_out = recv(socket, buffer, buff_siz, 0);

    if (recv_out == -1) {
        detail::qlog_manager::manager().logFormat("Error get data from connection ({})", log_type::ERROR, std::strerror(errno));

        close(socket);
        return; 
    } else if (recv_out == 0) {
        detail::qlog_manager::manager().log("[connection] closed", log_type::DEBUG);

        close(socket);
        return;
    } else {
        std::string buffer_str = buffer;
        qrequest request(buffer_str);
        request.parse();
        detail::qlog_manager::manager().logFormat("[connection] request uri: {}, method: {}, version: {}", log_type::INFO, request.get_uri(), request.get_method(), request.get_http_version());
        
        qresponse response;
        this->router.endpoint(request.get_method(), request.get_uri(), request, response);
        response.generate();
        std::string raw_data = response.get_raw_data();

        if (send(socket, raw_data.c_str(), raw_data.size(), 0) == -1) {
           detail::qlog_manager::manager().logFormat("Error send data: ({})", log_type::ERROR, std::strerror(errno));    
            close(socket);

            return;
        }

        detail::qlog_manager::manager().log("[connection] response send successfully", log_type::INFO);
    }

    delete[] buffer;
    close(socket);
}