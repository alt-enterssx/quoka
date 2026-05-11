#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include "altenter/info/qlog_manager.h"

namespace altenter::quoka
{
    class qrequest 
    {
    public:
        qrequest(std::string& raw_data);
        ~qrequest();

        void parse();

        void add_param(std::string& param, std::string& value);
        std::string get_param(const std::string& param);

        std::string get_raw_data();
        std::string get_uri();
        std::string get_header(const std::string& header);
        std::string get_method();
        std::string get_body();
        std::string get_http_version();

    private:
        std::string raw_data;

        std::string http_version;
        std::string uri;
        std::unordered_map<std::string, std::string> headers_map;
        std::unordered_map<std::string, std::string> params_map;
        std::string method;
        std::string body;

        std::vector<std::string> split(std::string text, std::string delimiter);
    };
}