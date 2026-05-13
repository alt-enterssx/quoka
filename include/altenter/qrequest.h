#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include "altenter/qlog_manager.h"

namespace altenter::quoka
{
    class qrequest 
    {
    public:
        qrequest(std::string& raw_data) noexcept;
        ~qrequest() noexcept;

        void parse() noexcept;

        void add_param(std::string& param, std::string& value) noexcept;
        std::string get_param(const std::string& param) const noexcept;

        std::string get_raw_data() const noexcept;
        std::string get_uri() const noexcept;
        std::string get_header(const std::string& header) const noexcept;
        std::string get_method() const noexcept;
        std::string get_body() const noexcept;
        std::string get_http_version() const noexcept;

    private:
        std::string raw_data;

        std::string http_version;
        std::string uri;
        std::unordered_map<std::string, std::string> headers_map;
        std::unordered_map<std::string, std::string> params_map;
        std::string method;
        std::string body;

        std::vector<std::string> split(std::string text, std::string delimiter) noexcept;
    };
}