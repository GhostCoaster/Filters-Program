
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
		auto parseInt(char* input) -> bool {
			auto end = static_cast<char*>(nullptr);

			errno = 0;
			auto temp = std::strtol(input, &end, 10);

			if (input == end) return false;
			if (errno != 0) return false;

			value = new int(temp);

			return true;
		}

		auto parseFloat(char* input) -> bool {
			auto end = static_cast<char*>(nullptr);

			errno = 0;
			auto temp = std::strtof(input, &end);

			if (input == end) return false;
			if (errno != 0) return false;

			value = new float(temp);

			return true;
		}

		auto parseBool(char* input) -> bool {
			constexpr static char TRUE_STRING[4] = {'t', 'r', 'u', 'e'};
			constexpr static char FALSE_STRING[5] = {'f', 'a', 'l', 's', 'e'};
			constexpr static auto trueLength = sizeof(TRUE_STRING) / sizeof(char);
			constexpr static auto falseLength = sizeof(FALSE_STRING) / sizeof(char);

			auto isTrue = true;
			auto isFalse = true;
			auto index = 0;

			/* an empty string is invalid */
			if (!*input)
				return false;

			while(*input) {
				if (isTrue)
					isTrue = index < trueLength && *input == TRUE_STRING[index];

				if (isFalse)
					isFalse = index < falseLength && *input == FALSE_STRING[index];

				if (!isTrue && !isFalse)
					return false;

				++input;
				++index;
			}

			value = new bool(isTrue);

			return true;
		}

		constexpr static bool(Parameter::*parserList[])(char*) {
			&Parameter::parseInt,
			&Parameter::parseFloat,
			&Parameter::parseBool
		};

	private:
		ParameterType type;
		void* value;

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
