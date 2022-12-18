
#include "filter.h"

#include "util.h"

namespace FPP {
	Filter::Filter(std::string && name, std::vector<Parameter> && params, FilterFunc filter)
		: name(name), params(params), filter(filter) {
    }

	auto Filter::getName() -> std::string & {
		return name;
	}

	auto Filter::numParams() -> int {
		return (int)params.size();
	}

	auto Filter::getParam(int index) -> Parameter & {
		return params.at(index);
	}

    auto Filter::getParam(std::string & paramName) -> Parameter * {
        for (auto & param : params) {
            if (Util::stringEqual(paramName, param.name)) return &param;
        }
        return nullptr;
    }
}

