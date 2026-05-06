#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <type_traits>
#include "altenter/info/qlog_manager.h"

namespace altenter::quokahttp 
{
    class qconfig 
    {
        protected: 
            qconfig();
            ~qconfig();
        
        public:
            template<typename T>
            class qoption
            {
                public:
                    static T option(const std::string& key) {
                        std::string value = qconfig::config().get(key);
                        if (value.empty()) {
                            return T{};
                        }

                        if constexpr (std::is_same<T, int>::value) {
                            int integer = std::stoi(value);
                            return integer;
                        }
                        else if constexpr (std::is_same<T, bool>::value) {
                            bool boolean;
                            std::istringstream(value) >> std::boolalpha >> boolean;
                            return boolean;
                        }
                        else if constexpr (std::is_same<T, std::string>::value) {
                            std::string stroke = value;
                            return stroke;
                        }
                    }
            };

            qconfig(const qconfig&) = delete;
            void operator=(const qconfig&) = delete;
            
            static qconfig& config();

            void init();
            void set_path(const std::string& path);
            std::string get(const std::string& key);

        private:
            std::vector<std::string> split(std::string text, std::string delimiter);

            std::string conf_path;
            bool finalize;
            std::unordered_map<std::string, std::string> conf_map;
    };
}