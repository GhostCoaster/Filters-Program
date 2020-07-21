
#ifndef FILTERS_PROGRAM_COMMANDLINE_H
#define FILTERS_PROGRAM_COMMANDLINE_H

#include "filter/filter.h"

namespace FPP {
	class CommandLine {
	private:
		static int NUM_FILTERS;
		static FPP::Filter filterList[];

	public:
		static auto getImage(int numArguments, char** arguments, std::string&) -> Image;

		static auto getFilter(int numArguments, char** arguments, std::string&) -> Filter*;

		static auto getParameters(int numArguments, char** arguments, Filter*, std::string&) -> std::vector<Parameter>;
	};
}

#endif //FILTERS_PROGRAM_COMMANDLINE_H
