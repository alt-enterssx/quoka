#include "altenter/qexception.h"

using namespace altenter::quoka;

qexception::qexception(const std::string&& msg, exception_type type) noexcept : std::runtime_error(msg), exc_type(type) {}

exception_type qexception::type() noexcept { return this->exc_type; }
