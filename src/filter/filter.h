
#ifndef FPP_FILTER
#define FPP_FILTER

#include <initializer_list>
#include <vector>
#include "parameter.h"
#include "../img/image.h"

namespace FPP {
	class Filter {
	private:
		const char* name;
		std::vector<Parameter::ParameterType> paramTypes;

	public:
		using FilterFunc = void(*)(Image**, Image**, Parameter*);

		Filter(const char*, std::initializer_list<Parameter::ParameterType>, FilterFunc);

		auto getName() -> const char*;
		auto getNumParams() -> int;
		auto getParamType(int) -> Parameter::ParameterType;

		FilterFunc filter;
	};
}

#endif
