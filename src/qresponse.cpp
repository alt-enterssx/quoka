#include "altenter/qresponse.h"

using namespace altenter::quoka;

qresponse::qresponse(int status_code, std::string status_msg, std::string http_version, 
                std::vector<qheader> headers, std::string body): status_code(status_code), status_msg(status_msg), http_version(http_version), headers(std::move(headers)), body(body) {}

qresponse::qresponse() {}
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

qresponse::builder& qresponse::builder::set_status_code(int status_code) { 
    this->status_code_ = status_code; 
    return *this;
}

qresponse::builder& qresponse::builder::set_status_msg(std::string status_msg) { 
    this->status_msg_ = std::move(status_msg); 
    return *this;
}

qresponse::builder& qresponse::builder::set_http_version(std::string http_version) { 
    http_version.erase(http_version.begin(), std::find_if(http_version.begin(), http_version.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));

    http_version.erase(std::find_if(http_version.rbegin(), http_version.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), http_version.end());

    this->http_version_ = std::move(http_version);
    return *this;
}

qresponse::builder& qresponse::builder::add_header(std::string header_key, std::string header_value) { 
    qheader header = qheader(header_key, header_value);
    this->headers_.push_back(header);

    return *this;
}

qresponse::builder& qresponse::builder::set_body(std::string body) { 
    this->body_ = std::move(body); 
    return *this;
}

qresponse::builder& qresponse::builder::send_file(const std::string& path) {

    std::unordered_map<std::string, std::string> mime_type = {
        {".html", "text/html"}, {".css", "text/css"}, {".js", "text/javascript"}
    };

    auto send_error = [this] () -> void {
        this->status_code_ = 500;
        this->status_msg_ = "Internal Server Error";
    };

    size_t dot_pos = path.find_last_of('.');
    std::string extension_type = path.substr(dot_pos, path.size() - 1);

    detail::qlog_manager::manager().logFormat("extension type: {}", detail::log_type::WARNING, extension_type);

    std::string static_path;
    try {
        static_path = qconfig::qoption<std::string>().option("static.path").value();
    } catch (qexception& ex) {
        detail::qlog_manager::manager().log(
            ex.what(),
            ex.type()
        );

        detail::qlog_manager::manager().log("Error in find path to static files: static.path", detail::log_type::ERROR);
        send_error();

        return *this;
    }

    static_path.append(path);
    
    std::ifstream file(static_path);
    
    std::string buffer;
    std::string line;

    if (!file.is_open()) {
        detail::qlog_manager::manager().logFormat("Error in open static path file: {}", detail::log_type::ERROR, static_path);
        send_error();
        
        return *this;
    }

    while (std::getline(file, line)) {
        buffer.append(line);
    }
    file.close();

    if (buffer.empty()) {
        detail::qlog_manager::manager().log("File to read is empty", detail::log_type::ERROR);
        send_error();
        
        return *this;
    }

    this->body_ = std::move(buffer);
    this->headers_.push_back(qheader("Content-Type", mime_type.at(extension_type)));
    this->headers_.push_back(qheader("Content-Length", std::to_string(this->body_.size())));
    this->headers_.push_back(qheader("Connection", "close"));

    return *this;
}

qresponse::builder& qresponse::builder::send_text(const std::string& text) {
    this->body_ = std::move(text);
    this->headers_.push_back(qheader("Content-Type", "text/plain"));
    this->headers_.push_back(qheader("Content-Length", std::to_string(this->body_.size())));
    this->headers_.push_back(qheader("Connection", "close"));

    return *this;
}

qresponse qresponse::builder::build() {
    qresponse response = qresponse(this->status_code_, this->status_msg_, this->http_version_, this->headers_, this->body_);
    return response;
}

std::string qresponse::get_raw_data() { return this->raw_data; }