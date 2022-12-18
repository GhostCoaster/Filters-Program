//
// Created by Emmet on 12/17/2022.
//

#ifndef FILTERS_PROGRAM_HELP_H
#define FILTERS_PROGRAM_HELP_H

#include <string>
#include "filter/parameter.h"
#include "filter/filter.h"

namespace FPP::Help {
    auto prettyParameter(Parameter &) -> std::string;

    auto prettyFilter(Filter &) -> std::string;

    auto filtersList() -> std::string;
}

#endif //FILTERS_PROGRAM_HELP_H
