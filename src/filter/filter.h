
#ifndef FPP_FILTER
#define FPP_FILTER

#include <initializer_list>
#include <vector>
#include "parameter.h"
#include "../img/image.h"
#include "../img/buffers.h"

namespace FPP {
	class Filter {
	private:
		std::string name;
		std::vector<Parameter> params;

	public:
		using FilterFunc = void(*)(Buffers &, std::vector<Parameter::Value> &);

		Filter(std::string &&, std::vector<Parameter> &&, FilterFunc);

		auto getName() -> std::string &;
		auto numParams() -> int;

		auto getParam(int) -> Parameter &;
        auto getParamIndex(std::string & paramName) -> int;

		FilterFunc filter;
	};
}

#endif
