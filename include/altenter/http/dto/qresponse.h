#pragma once 

#include <string>
#include <vector>
#include <algorithm> 
#include <cctype>
#include "altenter/info/qlog_manager.h"

namespace altenter::quoka 
{
    struct qheader
    {
        std::string key;
        std::string value;

        qheader(const std::string& key, const std::string& value): key(std::move(key)), value(std::move(value)) {}
    };

    class qresponse 
    {
        protected:
            qresponse(int status_code, std::string status_msg, std::string http_version, 
                std::vector<qheader> headers, std::string body);
        
        public:
            class builder 
            {
                public:
                    builder& set_status_code(int status_code);
                    builder& set_status_msg(std::string status_msg);
                    builder& set_http_version(std::string http_version);
                    builder& add_header(std::string header_key, std::string header_value);
                    builder& set_body(std::string body);

                    qresponse build();
                private:
                    std::string raw_data_;
                    int status_code_;
                    std::string status_msg_;
                    std::string http_version_;
                    std::string body_;

                    std::vector<qheader> headers_;
            };
            
            qresponse();
            ~qresponse();

            void generate();
            std::string get_raw_data();

        private:
            std::string raw_data;
            int status_code;
            std::string status_msg;
            std::string http_version;
            std::string body;

            std::vector<qheader> headers;
    };
}