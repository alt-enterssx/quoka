#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <type_traits>
#include <memory>
#include <algorithm>
#include "altenter/info/qlog_manager.h"
#include "altenter/qexception.h"

namespace altenter::quoka 
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
                    qoption(): default_val(nullptr) {} 

                    qoption& option(const std::string& key) {
                        this->value_str = qconfig::config().get(key);
                        return *this;
                    }
                    
                    qoption& default_value(T value) {
                        this->default_val = std::make_unique<T>(value);
                        return *this;
                    }

                    T value() {
                        if (this->value_str.empty()) {
                            if (default_val == nullptr) {
                                throw qexception("Error in get config value", exception_type::ERROR);
                            }
                            else {
                                return *default_val;
                            }
                        }

                        if constexpr (std::is_same<T, int>::value) { 
                            return stoi(this->value_str);
                        }
                        else if constexpr (std::is_same<T, bool>::value) {
                            std::transform(this->value_str.begin(), this->value_str.end(), this->value_str.begin(), ::tolower);
                            return this->value_str == "true" || this->value_str == "1";
                        }
                        else if constexpr (std::is_same<T, std::string>::value) {
                            return this->value_str;
                        }
                    }
                
                    private:
                        std::string value_str; 
                        std::unique_ptr<T> default_val;
            };

            qconfig(const qconfig&) = delete;
            void operator=(const qconfig&) = delete;
            
            static qconfig& config();

            void init();
            void set_path(const std::string& path);
            std::string get(const std::string& key);

        private:
            std::vector<std::string> split(std::string text, std::string delimiter);
            bool is_valid_path();

            std::string conf_path;
            bool finalize;
            std::unordered_map<std::string, std::string> conf_map;
    };
}