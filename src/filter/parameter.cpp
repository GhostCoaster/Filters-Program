
#include "parameter.h"

namespace FPP {
	Parameter::Parameter() : type(TYPE_NONE), value(nullptr), parse(nullptr) {}

	Parameter::Parameter(ParameterType type) : type(type), value(nullptr), parse(parserList[type]) {}

	Parameter::~Parameter() {
		switch (type) {
			case TYPE_INT: {
				delete reinterpret_cast<int*>(value);
			} break;
			case TYPE_FLOAT: {
				delete reinterpret_cast<float*>(value);
			} break;
			case TYPE_BOOL: {
				delete reinterpret_cast<bool*>(value);
			} break;

		}
	}
}
