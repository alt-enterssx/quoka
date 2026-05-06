#include "altenter/http/qserver.h"

using namespace altenter::quokahttp;

qserver::qserver(int port): qs_port(port), listener(this->router) {
}

qserver::~qserver() {
    this->is_running = false;
}

qserver::builder::builder(): port_(-1) {}

qserver::builder& qserver::builder::set_port(int port) {
    this->port_ = port;
    return *this;
}

std::unique_ptr<qserver> qserver::builder::build() {
    if (port_ == -1) {
        int conf_port = qconfig::qoption<int>::option("server.port");

        if (conf_port != 0) {
            port_ = conf_port;
        }
    }

    if (port_ <= 0 || port_ > 0xFFFF) {
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

void qserver::run() {
    try {
        this->socket.init_socket();
        this->socket.bind_address(this->qs_port);

        this->is_running = true;
        detail::qlog_manager::manager().logFormat("Server started and running: http://{}:{}", detail::log_type::INFO, LOCAL_ADDRESS, this->qs_port);

        listener.set_socket(this->socket.get_socket());
        listener.listen();
    } catch(qexception& ex) {
        detail::log_type typeFromExc = (ex.get_type() == exception_type::ERROR) ? detail::log_type::ERROR : detail::log_type::CRITICAL;
        detail::qlog_manager::manager().log(ex.what(), typeFromExc);

        detail::qlog_manager::manager().shutdown();       

        if (ex.get_type() == exception_type::CRITICAL) { exit(EXIT_FAILURE); }
    }   
}

void qserver::get_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.get_point(uri, std::move(exec)); }
void qserver::post_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.post_point(uri, std::move(exec)); }
void qserver::put_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.put_point(uri, std::move(exec)); }
void qserver::delete_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.delete_point(uri, std::move(exec)); }
void qserver::patch_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.patch_point(uri, std::move(exec)); }
void qserver::options_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.options_point(uri, std::move(exec)); }
void qserver::set_not_found(std::function<void(qrequest& request, qresponse& response)> not_found) { this->router.set_not_found(not_found); }

