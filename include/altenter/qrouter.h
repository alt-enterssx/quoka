#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include <optional>
#include "altenter/qrequest.h"
#include "altenter/qresponse.h"
#include "altenter/qlog_manager.h"

namespace altenter::quoka
{
    class qrouter
    {
        protected:
            qrouter() noexcept;
            ~qrouter() noexcept;

        public:
            static qrouter& router();

            void endpoint(const std::string& method, const std::string& uri, qrequest& request, qresponse& reponse) noexcept;
            void get_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept;
            void put_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept;
            void post_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept;
            void head_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept;
            void delete_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept;
            void patch_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept;
            void options_point(const std::string& uri, std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept;

            void set_not_found(std::function<void(qrequest& request, qresponse& response)> not_found) noexcept;


        private:
            void default_not_found(qrequest& request, qresponse& response) noexcept;
            std::function<void(qrequest& request, qresponse& response)> not_found;

            void add_route(const std::string& method, const std::string& uri, 
                std::function<void(qrequest& request, qresponse& respopnse)> handle) noexcept;
                std::vector<std::string> split(std::string uri, char delimiter) noexcept;

            const std::string METHODS_STR[7] = 
            {
                "get", "put", "post", "head", "delete", "patch", "options"
            };

            class qrouting_node 
            {
                public:
                    qrouting_node() noexcept;
                    ~qrouting_node() noexcept;

                    void add_node(std::string& fragment) noexcept;
                    std::optional<std::shared_ptr<qrouting_node>> find_child(std::string& fragment) noexcept;

                    void set_handle(std::function<void(qrequest& request, qresponse& respopnse)> handle)noexcept;
                    void execute(qrequest& request, qresponse& response);

                    bool is_parameter() noexcept;
                    void set_parameter(bool is_param) noexcept;
                    std::string get_param() noexcept;
                    void set_param(std::string& param) noexcept;

                    std::shared_ptr<qrouting_node> get_wildcard() noexcept;

                private:
                    std::unordered_map<std::string, std::shared_ptr<qrouting_node>> static_nodes;
                    std::shared_ptr<qrouting_node> param_node;
                    std::shared_ptr<qrouting_node> wildcart_node;        
    
                    std::string param_name;
                    bool is_param;
                    std::function<void(qrequest& request, qresponse& respopnse)> handle;
            };

            std::unordered_map<std::string, std::shared_ptr<qrouting_node>> methods_map;
    };
}