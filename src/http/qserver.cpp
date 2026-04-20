#include "altenter/http/qserver.h"

using namespace altenter::quokahttp;

qserver::qserver(): socket(this->log_manager), listener(this->log_manager) {
    std::shared_ptr<qconsole_logger> consoleLogger = std::make_shared<qconsole_logger>();
    this->log_manager.add_logger(consoleLogger);
}

qserver::~qserver() {
    this->is_running = false;
}

void qserver::run(int port) {
    std::stringstream log_msg;
    if (port < 0x0 || port > 0xFFFF) {
        log_msg << "Argument port is invalid: (" << port << "), gets default port: (" << DEFAULT_PORT_START << ")";
        this->log_manager.log(log_msg.str(), detail::log_type::WARNING);
        
        log_msg.str("");
        log_msg.clear();

        this->qs_port = DEFAULT_PORT_START;
    } else { this->qs_port = port; }

    try {
        this->socket.init_socket();
        this->socket.bind_address(this->qs_port);

        listener.set_socket(this->socket.get_socket());
        listener.listen();
    } catch(qexception& ex) {
        detail::log_type typeFromExc = (ex.get_type() == exception_type::ERROR) ? detail::log_type::ERROR : detail::log_type::CRITICAL;
        this->log_manager.log(ex.what(), typeFromExc);

        this->log_manager.shutdown();       

        if (ex.get_type() == exception_type::CRITICAL) { exit(EXIT_FAILURE); }
    }

    this->is_running = true;
    log_msg << "Server started and running: " "http://" << LOCAL_ADDRESS << ":" << this->qs_port;
    this->log_manager.log(log_msg.str(), detail::log_type::INFO);

    log_msg.str("");
    log_msg.clear();    
}