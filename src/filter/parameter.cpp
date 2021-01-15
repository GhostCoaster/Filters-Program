
#include "parameter.h"

namespace FPP {
	Parameter::Parameter() : type(TYPE_NONE), value(nullptr), parse(nullptr) {}

	Parameter::Parameter(ParameterType type) : type(type), value(nullptr), parse(parserList[type]) {}

	Parameter::~Parameter() {
		delete value;
	}
}
