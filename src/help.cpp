//
// Created by Emmet on 12/17/2022.
//

#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>

#include "help.h"
#include "filter/filterList.h"

namespace FPP::Help {
    const char * prettyTypeNames[] = {
    "Int",
    "Float",
    "Bool",
    };

    template <typename T>
    auto prettyBoundedParameter(Parameter & parameter) -> std::string {
        auto str = std::stringstream();
        str.precision(1);
        fixed(str);

        if (parameter.bounded) {
            str << ", range: " << parameter.getLow<T>() << " - " << parameter.getHigh<T>();
        }

        str << ", default: " << Parameter::fromValue<T>(parameter.defaultValue);

        return str.str();
    }

    auto prettyBoolParameter(Parameter & parameter) -> std::string {
        return std::string(", default: ") + (Parameter::fromValue<bool>(parameter.defaultValue) ? "true" : "false");
    }

    /* count [int] (0.0 - 1.0): number of ripples */
    /* count [int]: number of ripples */
    auto prettyParameter(Parameter & parameter) -> std::string {
        return parameter.name + " [" + prettyTypeNames[parameter.type] + "]" + (
                parameter.type == Parameter::TYPE_INT ?
                prettyBoundedParameter<int>(parameter) :
                parameter.type == Parameter::TYPE_FLOAT ?
                prettyBoundedParameter<float>(parameter) :
                prettyBoolParameter(parameter)
            ) + " | " + parameter.description;
    }

    auto prettyFilter(Filter & filter) -> std::string {
        auto str = filter.getName() + ":\n";

        for (auto i = 0; i < filter.numParams(); ++i) {
            str += "* " + prettyParameter(filter.getParam(i));
            if (i != filter.numParams() - 1) str += "\n";
        }

        return str;
    }

    auto filtersList() -> std::string {
        auto str = std::string();

        for (auto i = 0; i < FilterList::list.size(); ++i) {
            str += FilterList::list[i].getName();
            if (i != FilterList::list.size() - 1) str += "\n";
        }

        return str;
    }
}