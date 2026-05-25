#include "altenter/qcors.h"

using namespace altenter::quoka;

qcors::qcors(): origin("*"), methods("*"), headers("*") {}
qcors::~qcors() {}

qcors& qcors::cors() {
    static qcors cors;
    return cors;
}


void qcors::set_origin(const std::string& origin) {
    this->origin = std::move(origin);
}

void qcors::set_methods(const std::string& methods) {
    this->methods = std::move(methods); 
}

void qcors::set_headers(const std::string& headers) {
    this->headers = std::move(headers);
}

bool qcors::is_preflight(const qrequest& request) {
    return request.get_method() == "options";
}

void qcors::apply(qresponse& response) {
    response.add_header("Access-Control-Allow-Origin", this->origin);
    response.add_header("Access-Control-Allow-Methods", this->methods);
    response.add_header("Access-Control-Allow-Headers", this->headers);
}