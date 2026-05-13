#include "altenter/qconfig.h"

using namespace altenter::quoka;

qconfig::qconfig(): finalize(false), conf_path({}) {}
qconfig::~qconfig() {}

qconfig& qconfig::config() {
    static qconfig config;
    return config;
}

void qconfig::init() noexcept {

    auto is_valid_path = [this] () -> bool {
        size_t dot_pos = this->conf_path.find_last_of('.');
        std::string extension_type = this->conf_path.substr(dot_pos, this->conf_path.size() - 1);

        return extension_type == ".conf";
    };

    if (finalize) {
        detail::qlog_manager::manager().log(
            "Config initialized before", 
            detail::log_type::ERROR
        );
        return;
    }
    else if (this->conf_path.empty()) {
        detail::qlog_manager::manager().log(
            "Path to config file is empty", 
            detail::log_type::ERROR
        );
        return;
    }
    else if (!is_valid_path()) {
        detail::qlog_manager::manager().logFormat(
            "Path to config is invalid: {}", 
            detail::log_type::ERROR,
            this->conf_path
        );
        return;
    }

    std::ifstream conf_file(this->conf_path);
    if (!conf_file.is_open()) {
        detail::qlog_manager::manager().logFormat(
            "Error in open config file: {}", 
            detail::log_type::ERROR, 
            this->conf_path
        );
        return;
    }

    std::string line;
    while (std::getline(conf_file, line)) {
        std::vector<std::string> line_as_values = this->split(line, "=");

        if (line_as_values.size() == 2) {
            std::string key_str = line_as_values.at(0);
            std::string value_str = line_as_values.at(1);

            this->conf_map.insert({key_str, value_str});
        }
    }

    detail::qlog_manager::manager().log(
        "Config initialized successfully", 
        detail::log_type::INFO
    );
    finalize = true;
}

void qconfig::set_path(const std::string& path) { this->conf_path = std::move(path); }
std::string qconfig::get(const std::string& key) {
    if (
        this->conf_map.find(key) == this->conf_map.end()
    ) {
        detail::qlog_manager::manager().logFormat(
            "Error in get conf parameter with key: {}", 
            detail::log_type::WARNING, 
            key
        );
        return {};
    }

    std::string value = this->conf_map.at(key);
    return value;
}

std::vector<std::string> qconfig::split(std::string text, std::string delimiter) {
    std::vector<std::string> seglist;

    size_t pos = 0;
    std::string token;

    while ((pos = text.find(delimiter)) != std::string::npos) {
        token = text.substr(0, pos);
        seglist.push_back(token);
        text.erase(0, pos + delimiter.length());
    }
    
    seglist.push_back(text);

    return seglist;
} 