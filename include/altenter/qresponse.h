#pragma once 

#include <string>
#include <vector>
#include <algorithm> 
#include <cctype>
#include <fstream>
#include "altenter/qlog_manager.h"
#include "altenter/qconfig.h"
#include "altenter/qexception.h"

namespace altenter::quoka 
{
    struct qheader
    {
        std::string key;
        std::string value;

        qheader(const std::string& key, const std::string& value) noexcept : key(std::move(key)), value(std::move(value)) {}
    };

    class qresponse 
    {
        protected:
            qresponse(int status_code, std::string status_msg, std::string http_version, 
                std::vector<qheader> headers, std::string body) noexcept;
        
        public:
            class builder 
            {
                public:
                    builder& set_status_code(int status_code) noexcept;
                    builder& set_status_msg(std::string status_msg) noexcept;
                    builder& set_http_version(std::string http_version) noexcept;
                    builder& add_header(std::string header_key, std::string header_value) noexcept;
                    builder& set_body(std::string body) noexcept;

                    builder& send_file(const std::string& path) noexcept;
                    builder& send_text(const std::string& text) noexcept;

                    qresponse build() noexcept;
                private:
                    std::string raw_data_;
                    int status_code_;
                    std::string status_msg_;
                    std::string http_version_;
                    std::string body_;

                    std::vector<qheader> headers_;
            };

            qresponse() noexcept;
            ~qresponse() noexcept;

            void add_header(std::string header_key, std::string header_value) noexcept;

            void generate() noexcept;
            std::string get_raw_data() const noexcept;

        private:
            std::string raw_data;
            int status_code;
            std::string status_msg;
            std::string http_version;
            std::string body;

            std::vector<qheader> headers;
    };
}