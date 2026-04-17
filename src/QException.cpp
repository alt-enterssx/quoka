#include "altenter/QException.h"

using namespace altenter::quokahttp;

QException::QException(const std::string&& msg, altenter::quokahttp::ExceptionType type): std::runtime_error(msg), exc_type(type) {}

ExceptionType QException::getType() { return this->exc_type; }
