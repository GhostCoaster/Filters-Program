//
// Created by Emmet on 7/19/2020.
//

#ifndef FILTERS_PROGRAM_FILTEREXCEPTION_H
#define FILTERS_PROGRAM_FILTEREXCEPTION_H

#include <exception>
#include <string>

namespace FPP {
    class FilterException final : public std::exception {
    private:
        const char* message;

    public:
	    FilterException(const char*);
	    FilterException(std::string&&);

        auto what();
    };
}

#endif //FILTERS_PROGRAM_FILTEREXCEPTION_H
