#pragma once

#include <string>
#include "altenter/qrequest.h"
#include "altenter/qresponse.h"

namespace altenter::quoka {
    class qcors {
        protected:
            qcors();
            ~qcors();

        public:
            void operator=(const qcors& cors) = delete;
            qcors(const qcors& cors) = delete;

            static qcors& cors() noexcept;

            void set_origin(const std::string& origin);
            void set_methods(const std::string& methods);
            void set_headers(const std::string& headers);

            bool is_preflight(const qrequest& request);
            void apply(qresponse& response);

        private:
            std::string origin;
            std::string methods;
            std::string headers;
    };
}
