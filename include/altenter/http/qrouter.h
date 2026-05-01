#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>
#include "altenter/http/dto/qrequest.h"
#include "altenter/http/dto/qresponse.h"
#include "altenter/info/qlog_manager.h"

namespace altenter::quokahttp
{
    class qrouter
    {
        public:
            qrouter(detail::qlog_manager& log_manager);
            ~qrouter();

            void endpoint(const std::string& method, const std::string uri, qrequest& request, qresponse& response);

            void get_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point);
            void post_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point);
            void put_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point);
            void delete_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point);
            void patch_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point);
            void options_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec_point);

        private:
            detail::qlog_manager& log_manager;

            std::unordered_map<std::string, std::unordered_map<std::string, std::function<void(qrequest& request, qresponse& response)>>*> methods_map;
            std::unordered_map<std::string, std::function<void(qrequest& request, qresponse& response)>> get_map;
            std::unordered_map<std::string, std::function<void(qrequest& request, qresponse& response)>> post_map;
            std::unordered_map<std::string, std::function<void(qrequest& request, qresponse& response)>> put_map;
            std::unordered_map<std::string, std::function<void(qrequest& request, qresponse& response)>> delete_map;
            std::unordered_map<std::string, std::function<void(qrequest& request, qresponse& response)>> patch_map;
            std::unordered_map<std::string, std::function<void(qrequest& request, qresponse& response)>> options_map;
    };
}