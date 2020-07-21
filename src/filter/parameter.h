
#ifndef FILTERS_PROGRAM_PARAMETER_H
#define FILTERS_PROGRAM_PARAMETER_H

#include <cstdlib>
#include <memory>
#include <iostream>

namespace FPP {
	class Parameter {
	public:
		constexpr static int TYPE_INT = 0;
		constexpr static int TYPE_FLOAT = 1;
		constexpr static int TYPE_BOOL = 2;

	private:
		template<typename T>
		auto typeParse(char*) -> bool;

		template<> auto Parameter::typeParse<int>(char* input) -> bool {
			auto end = static_cast<char*>(nullptr);

			errno = 0;
			auto temp = std::strtol(input, &end, 10);

			if (input == end) return false;
			if (errno != 0) return false;

			value = new int(temp);

			return true;
		}

		template<> auto Parameter::typeParse<float>(char* input) -> bool {
			auto end = static_cast<char*>(nullptr);

			errno = 0;
			auto temp = std::strtof(input, &end);

			if (input == end) return false;
			if (errno != 0) return false;

			value = new float(temp);

			return true;
		}

		template<> auto Parameter::typeParse<bool>(char* input) -> bool {
			constexpr static char TRUE_STRING[4] = {'t', 'r', 'u', 'e'};
			constexpr static char FALSE_STRING[5] = {'f', 'a', 'l', 's', 'e'};
			constexpr static auto trueLength = sizeof(TRUE_STRING) / sizeof(decltype(TRUE_STRING));
			constexpr static auto falseLength = sizeof(FALSE_STRING) / sizeof(decltype(FALSE_STRING));

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
			&Parameter::typeParse<int>,
			&Parameter::typeParse<float>,
			&Parameter::typeParse<bool>
		};

	private:
		int type;
		void* value;

	public:
		Parameter();
		explicit Parameter(int);

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
