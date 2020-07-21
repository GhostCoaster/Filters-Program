
#ifndef FILTERS_PROGRAM_COMMANDLINE_H
#define FILTERS_PROGRAM_COMMANDLINE_H

#include "filter/filter.h"
#include "filter/filters/blur.h"
#include "filter/filters/channelRotate.h"
#include "filter/filters/merge.h"
#include "filter/filters/poster.h"

namespace FPP {
	class CommandLine {
	private:
		static int NUM_FILTERS;
		static FPP::Filter filterList[];

	public:
		struct imageReturn {
			Image image;
			std::string errMessage;
		};

		struct filterReturn {
			Filter* filter;
			std::string errMessage;
		};

		struct parametersReturn {
			std::vector<Parameter> parameters;
			std::string errMessage;
		};

		static auto getImage(int numArguments, char** arguments) -> imageReturn;

		static auto getFilter(int numArguments, char** arguments) -> filterReturn;

		static auto getParameters(int numArguments, char** arguments, Filter*) -> parametersReturn;
	};
}

#endif //FILTERS_PROGRAM_COMMANDLINE_H
