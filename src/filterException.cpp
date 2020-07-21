//
// Created by Emmet on 7/19/2020.
//

#include "filterException.h"

namespace FPP {
    FilterException::FilterException(const char* message)
		: message(message) {}

    FilterException::FilterException(std::string&& message)
		: message(message.c_str()) {}

    auto FilterException::what() {
        return message;
    }
}
