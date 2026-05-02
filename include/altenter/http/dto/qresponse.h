#pragma once 

#include <string>
#include <vector>
#include <algorithm> 
#include <cctype>
#include "altenter/info/qlog_manager.h"

namespace altenter::quokahttp 
{
    struct qheader_item 
    {
        std::string key;
        std::string value;

        qheader_item(const std::string& key, const std::string& value): key(std::move(key)), value(std::move(value)) {}
    };

    class qresponse 
    {
        public:
            qresponse();
            ~qresponse();

            void generate();

            void set_status_code(int status_code);
            void set_status_msg(std::string status_msg);
            void set_http_version(std::string http_version);
            void add_header(std::string header_key, std::string header_value);
            void set_body(std::string body);

            std::string get_raw_data();

        private:
            std::string raw_data;
            int status_code;
            std::string status_msg;
            std::string http_version;
            std::string body;

            std::vector<qheader_item> headers;
    };
}