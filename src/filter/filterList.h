//
// Created by Emmet on 12/17/2022.
//

#ifndef FILTERS_PROGRAM_FILTERLIST_H
#define FILTERS_PROGRAM_FILTERLIST_H

#include "filter.h"

namespace FPP {
    class FilterList {
    public:
        static std::vector<Filter> list;

        static auto init() -> void;

        static auto getFilter(std::string &) -> Filter *;
    };
}

#endif //FILTERS_PROGRAM_FILTERLIST_H
