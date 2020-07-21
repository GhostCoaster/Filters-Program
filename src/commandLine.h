
#ifndef FILTERS_PROGRAM_COMMANDLINE_H
#define FILTERS_PROGRAM_COMMANDLINE_H

#include "filter/filter.h"

namespace FPP {
	class CommandLine {
	private:
		using FlagParser = void(CommandLine::*)(int, char**, int, std::string&);

		static FPP::Filter filterList[];

		static char flags[];
		static FlagParser parsers[];

		std::vector<Image> images;
		std::vector<Filter*> filters;
		std::vector<std::vector<Parameter>> parameters;

		auto getImage(int numArguments, char** arguments, std::string&) -> Image;

		auto getFilter(int numArguments, char** arguments, std::string&) -> Filter*;

		auto getParameters(int numArguments, char** arguments, Filter*, std::string&) -> std::vector<Parameter>;

	public:
		CommandLine(int, char**, std::string&);

		auto getNumImages() -> int;
		auto getImage(int) -> Image&;

		auto getNumFilters() -> int;
		auto getFilter(int) -> Filter*;

		auto getParameters(int) -> std::vector<Parameter>&;
	};
}

#endif //FILTERS_PROGRAM_COMMANDLINE_H
