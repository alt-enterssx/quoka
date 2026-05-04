#include "altenter/http/dto/qrequest.h"

using namespace altenter::quokahttp;

qrequest::qrequest(std::string& raw_data): raw_data(std::move(raw_data)) {}

qrequest::~qrequest() {}

void qrequest::parse() {
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
            if (lines.at(i).size() == 1 && lines.at(i)[0] == '\r') {
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

std::string qrequest::get_raw_data() { return this->raw_data; }

std::string qrequest::get_uri() { return this->uri; }

std::string qrequest::get_header(const std::string& header) { 
    if (this->headers_map.find(header) == this->headers_map.end()) {
        detail::qlog_manager::get_instance().logFormat("not found header: {}", detail::log_type::WARNING, header);
        return "";
    } 

    std::string header_str = this->headers_map.at(header);
    header_str.erase(std::remove_if(header_str.begin(), header_str.end(), [] (unsigned char c) -> bool {
         return c == '\n' || c == '\r';
    }), header_str.end());

    return header_str;
}

std::string qrequest::get_method() { return this->method; }

std::string qrequest::get_body() { return this->body; }

std::string qrequest::get_http_version() { return this->http_version; }

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