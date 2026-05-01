#include "altenter/http/qrouter.h"

using namespace altenter::quokahttp;

qrouter::qrouter(detail::qlog_manager& log_manager): log_manager(log_manager) {
    this->methods_map = {
        {"get", &this->get_map}, {"post", &this->post_map}, {"put", &this->put_map}, 
        {"delete", &this->delete_map}, {"patch", &this->patch_map}, {"options", &this->options_map} 
    };
}

qrouter::~qrouter() {}

void qrouter::endpoint(const std::string& method, const std::string uri, qrequest& request, qresponse& response) {
    if (this->methods_map.find(method) == this->methods_map.end()) {
        log_manager.logFormat("Error in get endpoint with method: {}", detail::log_type::ERROR, method);
        return;
    }

    std::unordered_map<std::string, std::function<void(qrequest& request, qresponse& response)>> method_map = *this->methods_map.at(method);

    if (method_map.find(uri) == method_map.end()) {
        log_manager.logFormat("Error in get endpoint with uri: {}, method: {}", detail::log_type::ERROR, uri, method);
        return;
    }

    std::function<void(qrequest& request, qresponse& response)> exec = method_map.at(uri);
    exec(request, response); 

}

void qrouter::get_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point) {
    if (this->get_map.find(uri) != this->get_map.end()) {
        log_manager.logFormat("Cannot add route with uri: {}, method: {} (is exists)", detail::log_type::ERROR, uri, "get");
    } else { 
        this->get_map.insert({uri, std::move(exec_point)}); 
        log_manager.logFormat("Add new route with uri: {}, method: {}", detail::log_type::DEBUG, uri, "get");
    }
}

void qrouter::post_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point) {
    if (this->post_map.find(uri) != this->post_map.end()) {
        log_manager.logFormat("Cannot add route with uri: {}, method: {} (is exists)", detail::log_type::ERROR, uri, "post");
    } else { 
        this->post_map.insert({uri, std::move(exec_point)}); 
        log_manager.logFormat("Add new route with uri: {}, method: {}", detail::log_type::DEBUG, uri, "post");
    }
}

void qrouter::put_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point) {
    if (this->put_map.find(uri) != this->put_map.end()) {
        log_manager.logFormat("Cannot add route with uri: {}, method: {} (is exists)", detail::log_type::ERROR, uri, "put");
    } else { 
        this->put_map.insert({uri, std::move(exec_point)}); 
        log_manager.logFormat("Add new route with uri: {}, method: {}", detail::log_type::DEBUG, uri, "put");
    }
}

void qrouter::delete_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point) {
    if (this->delete_map.find(uri) != this->delete_map.end()) {
        log_manager.logFormat("Cannot add route with uri: {}, method: {} (is exists)", detail::log_type::ERROR, uri, "delete");
    } else { 
        this->delete_map.insert({uri, std::move(exec_point)}); 
        log_manager.logFormat("Add new route with uri: {}, method: {}", detail::log_type::DEBUG, uri, "delete");
    }
}

void qrouter::patch_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point) {
    if (this->patch_map.find(uri) != this->patch_map.end()) {
        log_manager.logFormat("Cannot add route with uri: {}, method: {} (is exists)", detail::log_type::ERROR, uri, "patch");
    } else { 
        this->patch_map.insert({uri, std::move(exec_point)}); 
        log_manager.logFormat("Add new route with uri: {}, method: {}", detail::log_type::DEBUG, uri, "patch");
    }
}

void qrouter::options_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point) {
    if (this->options_map.find(uri) != this->options_map.end()) {
        log_manager.logFormat("Cannot add route with uri: {}, method: {} (is exists)", detail::log_type::ERROR, uri, "options");
    } else { 
        this->options_map.insert({uri, std::move(exec_point)}); 
        log_manager.logFormat("Add new route with uri: {}, method: {}", detail::log_type::DEBUG, uri, "options");
    }
}