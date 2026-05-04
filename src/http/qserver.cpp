#include "altenter/http/qserver.h"

using namespace altenter::quokahttp;

qserver::qserver(int port, bool console_log_status): qs_port(port), listener(this->router) {
    std::shared_ptr<detail::q_i_logger> c_logger = std::make_shared<qconsole_logger>();
    detail::qlog_manager::get_instance().add_logger(c_logger);
}

qserver::~qserver() {
    this->is_running = false;
}

qserver::builder& qserver::builder::set_port(int port) {
    if (port < 0x0 || port > 0xFFFF) {
        detail::qlog_manager::get_instance().logFormat("Argument port is invalid: ({}), gets default port: ({})", detail::log_type::WARNING,
            port, DEFAULT_PORT_START);
        
        this->port_ = DEFAULT_PORT_START;
    } else { this->port_ = port; }

    return *this;
}

qserver::builder& qserver::builder::set_console_logger(bool status) {
    this->console_log_status_ = status;
    return *this;
}

std::unique_ptr<qserver> qserver::builder::build() {
    return std::unique_ptr<qserver>(
        new qserver(this->port_, this->console_log_status_)
    );
}

void qserver::run() {
    try {
        this->socket.init_socket();
        this->socket.bind_address(this->qs_port);

        this->is_running = true;
        detail::qlog_manager::get_instance().logFormat("Server started and running: http://{}:{}", detail::log_type::INFO, LOCAL_ADDRESS, this->qs_port);

        listener.set_socket(this->socket.get_socket());
        listener.listen();
    } catch(qexception& ex) {
        detail::log_type typeFromExc = (ex.get_type() == exception_type::ERROR) ? detail::log_type::ERROR : detail::log_type::CRITICAL;
        detail::qlog_manager::get_instance().log(ex.what(), typeFromExc);

        detail::qlog_manager::get_instance().shutdown();       

        if (ex.get_type() == exception_type::CRITICAL) { exit(EXIT_FAILURE); }
    }   
}

void qserver::get_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.get_point(uri, std::move(exec)); }
void qserver::post_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.post_point(uri, std::move(exec)); }
void qserver::put_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.put_point(uri, std::move(exec)); }
void qserver::delete_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.delete_point(uri, std::move(exec)); }
void qserver::patch_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.patch_point(uri, std::move(exec)); }
void qserver::options_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.options_point(uri, std::move(exec)); }
