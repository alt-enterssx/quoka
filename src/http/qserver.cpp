#include "altenter/http/qserver.h"

using namespace altenter::quokahttp;

qserver::qserver(): socket(this->log_manager), listener(this->router, this->log_manager), router(this->log_manager) {
    std::shared_ptr<qconsole_logger> consoleLogger = std::make_shared<qconsole_logger>();
    this->log_manager.add_logger(consoleLogger);
}

qserver::~qserver() {
    this->is_running = false;
}

void qserver::run(int port) {
    if (port < 0x0 || port > 0xFFFF) {
        this->log_manager.logFormat("Argument port is invalid: ({}), gets default port: ({})", detail::log_type::WARNING,
            port, DEFAULT_PORT_START);
        
        this->qs_port = DEFAULT_PORT_START;
    } else { this->qs_port = port; }

    try {
        this->socket.init_socket();
        this->socket.bind_address(this->qs_port);

        this->is_running = true;
        this->log_manager.logFormat("Server started and running: http://{}:{}", detail::log_type::INFO, LOCAL_ADDRESS, this->qs_port);

        listener.set_socket(this->socket.get_socket());
        listener.listen();
    } catch(qexception& ex) {
        detail::log_type typeFromExc = (ex.get_type() == exception_type::ERROR) ? detail::log_type::ERROR : detail::log_type::CRITICAL;
        this->log_manager.log(ex.what(), typeFromExc);

        this->log_manager.shutdown();       

        if (ex.get_type() == exception_type::CRITICAL) { exit(EXIT_FAILURE); }
    }   
}

void qserver::get_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.get_point(uri, std::move(exec)); }
void qserver::post_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.post_point(uri, std::move(exec)); }
void qserver::put_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.put_point(uri, std::move(exec)); }
void qserver::delete_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.delete_point(uri, std::move(exec)); }
void qserver::patch_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.patch_point(uri, std::move(exec)); }
void qserver::options_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec) { this->router.options_point(uri, std::move(exec)); }
