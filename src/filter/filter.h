
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
		std::vector<int> paramTypes;

	public:
		using FilterFunc = void(*)(Image**, Image**, Parameter*);

		Filter(const char*, std::initializer_list<int>, FilterFunc);

		auto getName() -> const char*;
		auto getNumParams() -> int;
		auto getParamType(int) -> int;

		FilterFunc filter;
	};
}

#endif
