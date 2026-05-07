#include "altenter/qexception.h"

using namespace altenter::quoka;

qexception::qexception(const std::string&& msg, exception_type type): std::runtime_error(msg), exc_type(type) {}

exception_type qexception::get_type() { return this->exc_type; }
