#include "altenter/qrequest.h"

using namespace altenter::quoka;

qrequest::qrequest(std::string& raw_data) noexcept: raw_data(std::move(raw_data)) {}

qrequest::~qrequest() noexcept {}

void qrequest::parse() noexcept {
    std::vector<std::string> lines = this->split(this->raw_data, "\n");
    std::vector<std::string> firstLineData;

    if (lines.size() > 0) {
        firstLineData = this->split(lines[0], " ");

        if (firstLineData.size() == 3) {
            this->method = firstLineData.at(0);
            std::transform(this->method.begin(), this->method.end(), this->method.begin(), [] (unsigned char c) -> char {
                if (c >= 'A' && c <= 'Z') {
                    return c - ('Z' - 'z');
                }

                return c;
            });

            this->uri = firstLineData.at(1);
            this->http_version = firstLineData.at(2);
        }

        int pre_body_line;

        for (int i = 0; i < lines.size(); i++) {
            if (
                lines.at(i).size() == 1 && lines.at(i)[0] == '\r'
            ) {
                pre_body_line = i;
            }
        }

        for (int i = 1; i < pre_body_line; i++) {
            std::vector<std::string> header_data = this->split(lines[i], ": ");
            if (header_data.size() == 2) {
                this->headers_map[header_data.at(0)] = header_data.at(1);
            }
        }

        for (int i = pre_body_line; i < lines.size(); i++) {
            this->body.append(lines.at(i));
        }
    }
}

void qrequest::add_param(std::string& param, std::string& value) noexcept {
    this->params_map.insert({param, value});
}

std::string qrequest::get_param(const std::string& param) const noexcept {
    if (
        this->params_map.find(param) == this->params_map.end()
    ) {
        detail::qlog_manager::manager().logFormat(
            "not found parameter: {}", 
            detail::log_type::WARNING, 
            param
        );
        return "";
    } 

    std::string param_str = this->params_map.at(param);
    return param_str;
}

std::string qrequest::get_raw_data() const noexcept { return this->raw_data; }

std::string qrequest::get_uri() const noexcept { return this->uri; }

std::string qrequest::get_header(const std::string& header) const noexcept { 
    if (
        this->headers_map.find(header) == this->headers_map.end()
    ) {
        detail::qlog_manager::manager().logFormat(
            "not found header: {}", 
            detail::log_type::WARNING, 
            header
        );
        return "";
    } 

    std::string header_str = this->headers_map.at(header);
    header_str.erase(std::remove_if(header_str.begin(), header_str.end(), [] (unsigned char c) -> bool {
         return c == '\n' || c == '\r';
    }), header_str.end());

    return header_str;
}

std::string qrequest::get_method() const noexcept { return this->method; }

std::string qrequest::get_body() const noexcept { return this->body; }

std::string qrequest::get_http_version() const noexcept { return this->http_version; }

std::vector<std::string> qrequest::split(std::string text, std::string delimiter) {
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