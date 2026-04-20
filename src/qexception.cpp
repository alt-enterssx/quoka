#include "altenter/qexception.h"

using namespace altenter::quokahttp;

qexception::qexception(const std::string&& msg, altenter::quokahttp::exception_type type): std::runtime_error(msg), exc_type(type) {}

exception_type qexception::get_type() { return this->exc_type; }
