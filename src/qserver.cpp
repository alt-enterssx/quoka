#include "altenter/qserver.h"

using namespace altenter::quoka;

qserver::qserver(int port) noexcept : qs_port(port) {
}

qserver::~qserver() noexcept {
    this->is_running = false;
}

qserver::builder::builder() noexcept : port_(-1) {}

qserver::builder& qserver::builder::set_port(int port) noexcept {
    this->port_ = port;
    return *this;
}

std::unique_ptr<qserver> qserver::builder::build() noexcept {
    if (port_ == -1) {
        port_ = qconfig::qoption<int>().option("server.port").default_value(DEFAULT_PORT_START).value();
    }

    if (
        port_ <= 0 || port_ > 0xFFFF
    ) {
        detail::qlog_manager::manager().logFormat(
            "Argument port is invalid: ({}), gets default port: ({})",
            detail::log_type::WARNING,
            port_,
            DEFAULT_PORT_START
        );

        port_ = DEFAULT_PORT_START;
    }

    return std::unique_ptr<qserver>(
        new qserver(this->port_)
    );
}

void qserver::run() noexcept {
    try {
        this->socket.init_socket();
        this->socket.bind_address(this->qs_port);

        this->is_running = true;
        detail::qlog_manager::manager().logFormat(
            "Server started and running: http://{}:{}", 
            detail::log_type::INFO, 
            LOCAL_ADDRESS, 
            this->qs_port
        );

        listener.set_socket(this->socket.get_socket());
        listener.listen();
    } catch(qexception& ex) {
        detail::qlog_manager::manager().log(
            ex.what(), 
            ex.type()
        );

        detail::qlog_manager::manager().shutdown();       

        if (ex.type() == exception_type::CRITICAL) { exit(EXIT_FAILURE); }
    }   
}
