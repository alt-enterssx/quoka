#include "altenter/http/dto/qresponse.h"

using namespace altenter::quokahttp;

qresponse::qresponse(detail::qlog_manager& log_manager): log_manager(log_manager), http_version("HTTP/1.1") {}
qresponse::~qresponse() {}

void qresponse::generate() {
    std::string raw_data;
    
    std::string firstLine = this->http_version + " " + std::to_string(this->status_code) + " " + this->status_msg;

    firstLine.append("\r\n");
    raw_data.append(firstLine);

    for(int i = 0; i < this->headers.size(); i++) {
        std::string header_line = this->headers.at(i).key + ": " + this->headers.at(i).value;

        header_line.append("\r\n");
        raw_data.append(header_line);
    }

    raw_data.append("\r\n");

    if (!this->body.empty()) {
        raw_data.append(this->body);
    }

    this->raw_data = std::move(raw_data);
}

void qresponse::set_status_code(int status_code) { this->status_code = status_code; }

void qresponse::set_status_msg(std::string status_msg) { this->status_msg = std::move(status_msg); }

void qresponse::set_http_version(std::string http_version) { 
    http_version.erase(http_version.begin(), std::find_if(http_version.begin(), http_version.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));

    http_version.erase(std::find_if(http_version.rbegin(), http_version.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), http_version.end());

    this->http_version = std::move(http_version);
}

void qresponse::add_header(std::string header_key, std::string header_value) { 
    qheader_item item = qheader_item(header_key, header_value);
    this->headers.push_back(item);
}

void qresponse::set_body(std::string body) { this->body = std::move(body); }

std::string qresponse::get_raw_data() { return this->raw_data; }