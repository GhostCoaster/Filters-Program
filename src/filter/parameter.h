
#ifndef FILTERS_PROGRAM_PARAMETER_H
#define FILTERS_PROGRAM_PARAMETER_H

#include <cstdlib>
#include <memory>
#include <iostream>

namespace FPP {
	class Parameter {
	public:
        using Value = long long;

        template<typename T>
        static auto toValue(T value) -> Value {
            return *reinterpret_cast<Value *>(&value);
        }

        template<typename T>
        static auto fromValue(Value value) -> T {
            return *reinterpret_cast<T *>(&value);
        }

        enum ParameterType {
			TYPE_NONE  = -1,
			TYPE_INT   = 0,
			TYPE_FLOAT = 1,
			TYPE_BOOL  = 2
		};

        template<typename T>
        auto bound(T value) -> T {
            if (bounded) {
                if (value < getLow<T>()) return getLow<T>();
                if (value > getHigh<T>()) return getHigh<T>();
            }
            return value;
        }

	private:
		auto parseInt(char * input) -> Value {
			char * end;

			errno = 0;
			auto parsed = std::strtol(input, &end, 10);

			if (input == end || errno != 0) throw std::exception("malformed int argument");

			return toValue(bound<int>(parsed));
		}

		auto parseFloat(char * input) -> Value {
			char * end;

			errno = 0;
			auto parsed = std::strtof(input, &end);

			if (input == end || errno != 0) throw std::exception("malformed float argument");

			return toValue(bound<float>(parsed));
		}

		auto parseBool(char * input) -> Value {
			auto isTrue = true;
			auto isFalse = true;
			auto index = 0;

			/* an empty string is invalid */
			if (*input == '\0') throw std::exception("malformed boolean argument");

			/* nullcheck, make sure to not check past null terminator in "true" and "false" */
			/* then compare current input character against character in "true" and "false" */
			while(*input) {
				if (isTrue) isTrue = "true"[index] != '\0' && (*input | 0x20) == "true"[index];

				if (isFalse) isFalse = "false"[index] != '\0' && (*input | 0x20) == "false"[index];

				if (!isTrue && !isFalse) throw std::exception("malformed boolean argument");

				++input;
				++index;
			}

            return toValue(isTrue);
		}

		constexpr static Value (Parameter::*parserList[])(char *) {
			&Parameter::parseInt,
			&Parameter::parseFloat,
			&Parameter::parseBool
		};

	private:
        Value low;
        Value high;

	public:
        Value defaultValue;
        ParameterType type;
        bool bounded;
        std::string name;
        std::string description;

        template<typename T>
        Parameter(
            ParameterType type,
            T defaultValue,
            std::string && name,
            std::string && description
        ) : type(type),
            defaultValue(toValue(defaultValue)),
            name(name),
            description(description),
            bounded(false),
            low(0),
            high(0) {}

        template<typename T>
        Parameter(
            Parameter::ParameterType type,
            T defaultValue,
            T low,
            T high,
            std::string &&name,
            std::string &&description
        ) : type(type),
            defaultValue(toValue(defaultValue)),
            name(name),
            description(description),
            bounded(true),
            low(toValue(low)),
            high(toValue(high)) {}

		Value(Parameter::*parse)(char*) = parserList[type];

        template<typename T>
        auto getLow() -> T {
            return fromValue<T>(low);
        }

        template<typename T>
        auto getHigh() -> T {
            return fromValue<T>(high);
        }
	};
}

#endif //FILTERS_PROGRAM_PARAMETER_H
