
#include "filter.h"

namespace FPP {
	Filter::Filter(const char* name, std::initializer_list<int> paramTypes, FilterFunc filter)
		: name(name), paramTypes(paramTypes), filter(filter) {}

	auto Filter::getName() -> const char* {
		return name;
	}

	auto Filter::getNumParams() -> int {
		return paramTypes.size();
	}

	auto Filter::getParamType(int index) -> int {
		return paramTypes[index];
	}
}

