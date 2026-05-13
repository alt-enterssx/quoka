#include "altenter/qrouter.h"

using namespace altenter::quoka;

qrouter::qrouter() noexcept {
    for (auto str : METHODS_STR) {
        this->methods_map.insert({str, std::make_shared<qrouting_node>()});
    }
    
    this->not_found = [this] (qrequest& request, qresponse& response) -> void { this->default_not_found(request, response); };
}
qrouter::~qrouter() noexcept {}

qrouter& qrouter::router() {
    static qrouter router;
    return router;
}

void qrouter::endpoint(const std::string& method, const std::string& uri, qrequest& request, qresponse& response) noexcept {
    if (uri == "/") {
        auto root_node = this->methods_map.at(method);
        try {
            root_node->execute(request, response);
        } catch (qexception& ex) {
            detail::qlog_manager::manager().log(
                ex.what(),
                ex.type()
            );

            this->not_found(request, response);
        }
        return;
    }   
    
    std::vector<std::string> fragments = this->split(uri, '/');
    if (fragments.empty()) {
        detail::qlog_manager::manager().log(
            "uri is empty", 
            detail::log_type::ERROR
        );

        this->not_found(request, response);
        return;
    }

    if (this->methods_map.find(method) == this->methods_map.end()) {
        detail::qlog_manager::manager().logFormat(
            "Cannot find method: {}", 
            detail::log_type::ERROR, 
            method
        );
        return this->not_found(request, response);
    }
    
    std::shared_ptr<qrouter::qrouting_node> node = this->methods_map.at(method);

    for (size_t i = 0; i < fragments.size(); ++i) {
        std::string fragment = fragments[i];

        std::optional<std::shared_ptr<qrouter::qrouting_node>> next_node =
            node->find_child(fragment);
    
        if (!next_node.has_value()) {
            detail::qlog_manager::manager().logFormat(
                "Error in find point with uri: {}",
                detail::log_type::ERROR,
                uri
            );
            return this->not_found(request, response);
        }

        if (next_node.value()->is_parameter()) {
            std::string parameter = next_node.value()->get_param();

            if (next_node.value() == node->get_wildcard()) {
                std::string rest;

                for (size_t j = i; j < fragments.size(); ++j) {
                    if (j != i) {
                        rest += "/";
                    }

                    rest += fragments[j];
                }

                request.add_param(parameter, rest);

                node = next_node.value();
                break;
            }

            request.add_param(parameter, fragment);
        }
        
        node = next_node.value();
    }

    try {
        node->execute(request, response);
    } catch (qexception& ex) {
        detail::qlog_manager::manager().log(
            ex.what(),
            ex.type()
        );

        return this->not_found(request, response);
    }
}

void qrouter::get_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept {
    this->add_route("get", uri, handle);
}

void qrouter::put_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept {
    this->add_route("put", uri, handle);
}

void qrouter::post_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept {
    this->add_route("post", uri, handle);
}

void qrouter::head_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept {
    this->add_route("head", uri, handle);
}

void qrouter::delete_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept {
    this->add_route("delete", uri, handle);
}

void qrouter::patch_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept {
    this->add_route("patch", uri, handle);
}

void qrouter::options_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept {
    this->add_route("options", uri, handle);
}

void qrouter::add_route(const std::string& method, const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept {
    if (uri == "/") {
        if (this->methods_map.find(method) == this->methods_map.end()) {
            detail::qlog_manager::manager().logFormat(
                "Cannot find method: {}", 
                detail::log_type::ERROR, 
                method
            );
            return;
        }

        std::shared_ptr<qrouter::qrouting_node> root_node = this->methods_map.at(method);
        root_node->set_handle(handle);
        return;
    }
    
    std::vector<std::string> fragments = this->split(uri, '/');
    if (fragments.empty()) {
        detail::qlog_manager::manager().log(
            "uri is empty", 
            detail::log_type::ERROR
        );
        return;
    }

    if (this->methods_map.find(method) == this->methods_map.end()) {
        detail::qlog_manager::manager().logFormat(
            "Cannot find method: {}", 
            detail::log_type::ERROR, 
            method
        );
        return;
    }
    
    std::shared_ptr<qrouter::qrouting_node> node = this->methods_map.at(method);
    
    for (auto fragment : fragments) {
        std::optional<std::shared_ptr<qrouter::qrouting_node>> next_node = node->find_child(fragment);
    
        if (!next_node.has_value()) {
            node->add_node(fragment);
            next_node = node->find_child(fragment);
        }

        node = next_node.value();
    }

    node->set_handle(handle);

}

void qrouter::set_not_found(std::function<void(qrequest& request, qresponse& response)> not_found) noexcept { this->not_found = std::move(not_found); }

void qrouter::default_not_found(qrequest& request, qresponse& response) noexcept {
    response = qresponse::builder()
        .set_http_version(request.get_http_version())
        .set_status_code(404)
        .set_status_msg("Not found")
        .add_header("Connection", "close")
        .build();
}

std::vector<std::string> qrouter::split(std::string uri, char delimiter) noexcept {
    std::stringstream fragments_str(uri);
    std::string segment;
    std::vector<std::string> fragments;

    while(std::getline(fragments_str, segment, delimiter)) {
        if (segment.empty()) { continue; }
        fragments.push_back(segment);
    }

    return fragments;
} 

qrouter::qrouting_node::qrouting_node() noexcept: is_param(false), param_node(nullptr), wildcart_node(nullptr) {}
qrouter::qrouting_node::~qrouting_node() noexcept {}

std::optional<std::shared_ptr<qrouter::qrouting_node>> qrouter::qrouting_node::find_child(std::string& fragment) noexcept {
    if (this->static_nodes.find(fragment) != this->static_nodes.end()) {
        return std::make_optional<std::shared_ptr<qrouting_node>>(this->static_nodes.at(fragment));
    } else if (param_node != nullptr) {
        return std::make_optional<std::shared_ptr<qrouting_node>>(this->param_node);
    } else if (wildcart_node != nullptr) {
        return std::make_optional<std::shared_ptr<qrouting_node>>(this->wildcart_node);
    }

    return std::nullopt;
}

void qrouter::qrouting_node::add_node(std::string& fragment) noexcept {
    std::shared_ptr<qrouting_node> new_node =
        std::make_shared<qrouting_node>();

    if (!fragment.empty() && fragment[0] == ':') {
        new_node->set_parameter(true);

        std::string param_name = fragment.substr(1);
        new_node->set_param(param_name);

        this->param_node = new_node;
        return;
    }

    if (!fragment.empty() && fragment[0] == '*') {
        new_node->set_parameter(true);

        std::string param_name = fragment.substr(1);
        new_node->set_param(param_name);

        this->wildcart_node = new_node;
        return;
    }

    this->static_nodes.insert({fragment, new_node});
}

void qrouter::qrouting_node::set_handle(std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept {
    this->handle = std::move(handle);
}

void qrouter::qrouting_node::execute(qrequest& request, qresponse& response) noexcept {
    if (!this->handle) {
        throw qexception("Handle to route not added", exception_type::ERROR);
    }
    this->handle(request, response);
}


bool qrouter::qrouting_node::is_parameter() noexcept { return this->is_param; }
void qrouter::qrouting_node::set_parameter(bool is_param) noexcept { this->is_param = is_param; }

std::string qrouter::qrouting_node::get_param() noexcept { return this->param_name; }
void qrouter::qrouting_node::set_param(std::string& param) noexcept { this->param_name = std::move(param); }

std::shared_ptr<qrouter::qrouting_node> qrouter::qrouting_node::get_wildcard() noexcept { return this->wildcart_node; }