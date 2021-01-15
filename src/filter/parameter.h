
#ifndef FILTERS_PROGRAM_PARAMETER_H
#define FILTERS_PROGRAM_PARAMETER_H

#include <cstdlib>
#include <memory>
#include <iostream>

namespace FPP {
	class Parameter {
	public:
		enum ParameterType {
			TYPE_NONE  = -1,
			TYPE_INT   = 0,
			TYPE_FLOAT = 1,
			TYPE_BOOL  = 2
		};

	private:
		using Alloc_Type = long long;

		auto parseInt(char* input) -> bool {
			auto end = static_cast<char*>(nullptr);

			errno = 0;
			auto temp = std::strtol(input, &end, 10);

			if (input == end) return false;
			if (errno != 0) return false;

			value = new Alloc_Type();
			*reinterpret_cast<int*>(value) = temp;

			return true;
		}

		auto parseFloat(char* input) -> bool {
			auto end = static_cast<char*>(nullptr);

			errno = 0;
			auto temp = std::strtof(input, &end);

			if (input == end) return false;
			if (errno != 0) return false;

			value = new Alloc_Type();
			*reinterpret_cast<float*>(value) = temp;

			return true;
		}

		auto parseBool(char* input) -> bool {
			auto isTrue = true;
			auto isFalse = true;
			auto index = 0;

			/* an empty string is invalid */
			if (*input == '\0') return false;

			/* nullcheck, make sure to not check past null terminator in "true" and "false" */
			/* then compare current input character against character in "true" and "false" */
			while(*input) {
				if (isTrue) isTrue = "true"[index] != '\0' && (*input | 0x20) == "true"[index];

				if (isFalse) isFalse = "false"[index] != '\0' && (*input | 0x20) == "false"[index];

				if (!isTrue && !isFalse) return false;

				++input;
				++index;
			}

			value = new Alloc_Type();
			*reinterpret_cast<float*>(value) = isTrue;

			return true;
		}

		constexpr static bool(Parameter::*parserList[])(char*) {
			&Parameter::parseInt,
			&Parameter::parseFloat,
			&Parameter::parseBool
		};

	private:
		ParameterType type;
		Alloc_Type* value;

	public:
		Parameter();
		explicit Parameter(ParameterType);

		~Parameter();

		bool(Parameter::*parse)(char*);

		template<typename T>
		auto as(T def) -> T {
			if (value == nullptr) return def;

			return *reinterpret_cast<T*>(value);
		}

		template<typename T>
		auto as(T low, T high, T def) -> T {
			if (value == nullptr) return def;

			auto ret = *reinterpret_cast<T*>(value);

			if (ret < low)
				return low;

			if (ret > high)
				return high;

			return ret;
		}
	};
}

#endif //FILTERS_PROGRAM_PARAMETER_H
